#include "dialog_operator.hpp"
#include "gui_simulator.hpp"

#include <QApplication>
#include <QContextMenuEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMenu>
#include <QTest>
#include <QThread>
#include <QWidget>

#include <boost/filesystem.hpp>

#include <chrono>
#include <future>
#include <vector>

using std::chrono_literals::operator""ms;
using std::string_literals::operator""s;

namespace
{
template <typename T, bool N>
auto getChild(std::string const& name, QObject* parent, std::chrono::milliseconds timeout) -> T*
{
  auto start = std::chrono::steady_clock::now();
  auto stop = std::chrono::steady_clock::now();
  while (stop - start < timeout) {
    for (auto child : parent->children()) {
      if constexpr (N) {
        if (child->isWidgetType() && child->objectName().toStdString() == name) {
          return dynamic_cast<T*>(child);
        }
      } else {
        if (child->isWidgetType() && child->metaObject()->className() == name) {
          return dynamic_cast<T*>(child);
        }
      }
    }
    QTest::qWait(1);
    stop = std::chrono::steady_clock::now();
  }
  return nullptr;
}

} // namespace

namespace yoyo::gui::test
{
using interaction_t = std::variant<QPoint, Qt::Key, std::vector<Qt::Key>>;
enum class i_t { CLICK, DCLICK, KEY, KEYS };

struct single_dialog_interaction::impl {
  impl(std::string const& dialogTypeName)
    : _dialogTypeName { dialogTypeName }
  {
  }
  impl(impl const&) = default;
  impl(impl&&) = default;
  impl& operator=(impl const&) = default;
  impl& operator=(impl&&) = default;

  std::string _dialogTypeName;
  std::vector<std::tuple<i_t, QString, interaction_t, std::chrono::milliseconds>> _interactions;
};

single_dialog_interaction::single_dialog_interaction(std::string const& dialogTypeName)
  : _p { std::make_unique<impl>(dialogTypeName) }
{
}

single_dialog_interaction::~single_dialog_interaction() = default;
single_dialog_interaction::single_dialog_interaction(single_dialog_interaction const& other)
  : _p { std::make_unique<impl>(*other._p) }
{
}

single_dialog_interaction&
single_dialog_interaction::operator=(single_dialog_interaction const& other)
{
  (*_p) = (*other._p);
  return *this;
}

auto single_dialog_interaction::addMouseClick(QString const& childname,
                                              QPoint const& childCoordinate,
                                              std::chrono::milliseconds timeout)
  -> single_dialog_interaction&
{
  _p->_interactions.push_back({ i_t::CLICK, childname, childCoordinate, timeout });
  return *this;
}

auto single_dialog_interaction::addMouseDClick(QString const& childname,
                                               QPoint const& childCoordinate,
                                               std::chrono::milliseconds timeout)
  -> single_dialog_interaction&
{
  _p->_interactions.push_back({ i_t::DCLICK, childname, childCoordinate, timeout });
  return *this;
}

auto single_dialog_interaction::addKeyClick(QString const& childname, Qt::Key key,
                                            std::chrono::milliseconds timeout)
  -> single_dialog_interaction&
{
  _p->_interactions.push_back({ i_t::KEY, childname, key, timeout });
  return *this;
}

auto single_dialog_interaction::addKeyClicks(QString const& childname, std::vector<Qt::Key> keys,
                                             std::chrono::milliseconds timeout)
  -> single_dialog_interaction&
{
  _p->_interactions.push_back({ i_t::KEYS, childname, keys, timeout });
  return *this;
}

auto single_dialog_interaction::interact(QWidget* parent) -> bool
{
  auto dialog = getChild<QDialog, false>(_p->_dialogTypeName, parent, 100ms);

  if (!dialog) {
    return false;
  }

  if (_p->_interactions.empty()) {
    dialog->accept();
    return true;
  }
  for (auto const& interaction : _p->_interactions) {
    auto widget = getChild<QWidget, true>(std::get<QString>(interaction).toStdString(), dialog,
                                          std::get<std::chrono::milliseconds>(interaction));
    if (!widget) {
      dialog->accept();
      return false;
    }
    switch (std::get<i_t>(interaction)) {
    case i_t::CLICK:
      yoyo::gui::test::mouseClick(widget, Qt::LeftButton, {},
                                  std::get<QPoint>(std::get<interaction_t>(interaction)));
      break;
    case i_t::DCLICK:
      yoyo::gui::test::mouseDClick(widget, Qt::LeftButton, {},
                                   std::get<QPoint>(std::get<interaction_t>(interaction)));
      break;
    case i_t::KEY:
      yoyo::gui::test::keyClick(widget, std::get<Qt::Key>(std::get<interaction_t>(interaction)));
      break;
    case i_t::KEYS:
      yoyo::gui::test::keyClicks(
        widget, std::get<std::vector<Qt::Key>>(std::get<interaction_t>(interaction)));
      break;
    }
    QApplication::processEvents();
  }
  return true;
}

class file_dialog_handler
{
public:
  file_dialog_handler(QString const& filename)
    : _filename { filename.toStdString() }
  {
  }
  ~file_dialog_handler() = default;
  file_dialog_handler(file_dialog_handler const&) = default;
  file_dialog_handler(file_dialog_handler&&) = default;
  file_dialog_handler& operator=(file_dialog_handler const&) = default;
  file_dialog_handler& operator=(file_dialog_handler&&) = default;

  auto interact(QWidget* parent) -> bool
  {
    auto dialog = getChild<QDialog, false>("QFileDialog", parent, 100ms);
    if (!dialog) {
      return false;
    }
    if (auto d = dynamic_cast<QFileDialog*>(dialog)) {
      QTest::qWait(1000);
      QDir dir { QString::fromStdString(_filename.parent_path().string()) };
      d->setDirectory(dir.path());
      d->selectUrl("file://" + QString::fromStdString(_filename.string()));
      yoyo::gui::test::keyClick(dialog, Qt::Key_O, Qt::AltModifier);
      return true;
    }
    return false;
  }

private:
  boost::filesystem::path _filename;
};

///
/// \brief Dialog interactions for a single dialog
///
/// The class single_dialog_interaction provides a general interface to define interactions to a
/// dev defined dialog. In some cases - in particular when handling Qt dialogs - this is not really
/// possible or feasible to use. A special handler is necessary.
///
/// Instead of the more commonly used interface (abstract base class), the type erasure pattern was
/// choosen for this case in order not to confuse the dev using the tool-set.
///
/// Type erasure provides a kind of wrapper class, which can wrap totally unrelated classes in a
/// single object of a single type (not templated, though it uses templates internally, because that
/// would create different types). The only requirement of the classes to use is, that they defined
/// a set of methods of a certain signature (where the match doesn't need to be absolute).
/// In our case, we only need a single method: interact.
///
class a_single_dialog
{
private:
  class impl_interface
  {
  public:
    virtual ~impl_interface() = default;
    virtual auto interact(QWidget* parent) -> bool = 0;
    virtual auto clone() -> std::unique_ptr<impl_interface> = 0;
  };

  template <typename T>
  class impl_impl : public impl_interface
  {
  public:
    impl_impl(T const& from)
      : _inst { from }
    {
    }
    impl_impl(impl_impl const&) = default;
    impl_impl(impl_impl&&) = default;
    impl_impl& operator=(impl_impl const&) = default;
    impl_impl& operator=(impl_impl&&) = default;
    ~impl_impl() override = default;
    auto clone() -> std::unique_ptr<impl_interface> override
    {
      return std::make_unique<impl_impl<T>>(_inst);
    }
    auto interact(QWidget* parent) -> bool override
    {
      return _inst.interact(parent);
    }

  private:
    T _inst;
  };

public:
  template <typename T>
  a_single_dialog(T const& from)
    : _impl { std::make_unique<impl_impl<T>>(from) }
  {
  }
  ~a_single_dialog() = default;
  a_single_dialog(a_single_dialog const& other)
    : _impl { other._impl->clone() }
  {
  }
  a_single_dialog(a_single_dialog&&) = default;
  a_single_dialog& operator=(a_single_dialog const& other)
  {
    _impl = other._impl->clone();
    return *this;
  }
  a_single_dialog& operator=(a_single_dialog&&) = default;

  auto interact(QWidget* parent) -> bool
  {
    return _impl->interact(parent);
  }

private:
  std::unique_ptr<impl_interface> _impl;
};

class dialog_worker : public QObject
{
  Q_OBJECT
public:
  auto setWidget(QWidget* m) -> void
  {
    _parent = m;
  }
  auto setTimeout(std::chrono::milliseconds t)
  {
    _preTimeout = t;
  }

  auto setParameter(std::vector<a_single_dialog> actions) -> std::future<bool>;

public Q_SLOTS:
  void start();

private:
  QWidget* _parent { nullptr };
  std::chrono::milliseconds _preTimeout { 100 };
  std::promise<bool> _finished;
  std::vector<a_single_dialog> _dialogs;
};

std::future<bool> dialog_worker::setParameter(std::vector<a_single_dialog> actions)
{
  _finished = std::promise<bool>();
  _dialogs = actions;
  return _finished.get_future();
}

void dialog_worker::start()
{
  if (!_parent) {
    throw std::runtime_error("no widget set for dialog processing");
  }

  QTest::qWait(_preTimeout.count());

  for (auto dialog : _dialogs) {
    if (!dialog.interact(_parent)) {
      _finished.set_value(false);
      return;
    }
  }

  _finished.set_value(true);
}

struct dialog_operator::impl {
  impl(QWidget* parent)
    : _widget(parent)
  {
  }

  std::vector<a_single_dialog> _dialogs;
  QThread* _thread { nullptr };
  std::chrono::milliseconds _preTimeout { 100 };
  QWidget* _widget { nullptr };
};

dialog_operator::dialog_operator(QWidget* w)
  : _p { std::make_unique<impl>(w) }
{
}

dialog_operator::~dialog_operator() = default;

auto dialog_operator::openFileDialog(QString const& filename) -> dialog_operator&
{
  _p->_dialogs.push_back(file_dialog_handler { filename });
  return *this;
}

auto dialog_operator::addDialog(single_dialog_interaction const& dia) -> dialog_operator&
{
  _p->_dialogs.push_back(dia);
  return *this;
}

auto dialog_operator::useThread(QThread* t) -> dialog_operator&
{
  _p->_thread = t;
  return *this;
}

auto dialog_operator::timeoutForStart(std::chrono::milliseconds t) -> dialog_operator&
{
  _p->_preTimeout = t;
  return *this;
}

auto dialog_operator::operate(std::function<void()> trigger) -> void
{
  if (!_p->_widget) {
    throw std::runtime_error("No widget set for dialog operation");
  }

  if (_p->_dialogs.empty()) {
    throw std::runtime_error("No dialogs set to operate on");
  }

  QThread* useThread = _p->_thread;

  if (!useThread) {
    useThread = new QThread;
  }

  dialog_worker t;
  t.setWidget(_p->_widget);
  auto meta = t.metaObject();
  t.moveToThread(useThread);
  useThread->start();
  auto f = t.setParameter(_p->_dialogs);
  meta->method(meta->methodOffset()).invoke(&t, Qt::QueuedConnection);

  trigger();
  QApplication::processEvents();

  f.get();
  QTest::qWait(10);
  useThread->terminate();

  if (!_p->_thread) {
    delete useThread;
  }
}

} // namespace yoyo::gui::test
#include "dialog_operator.moc"
