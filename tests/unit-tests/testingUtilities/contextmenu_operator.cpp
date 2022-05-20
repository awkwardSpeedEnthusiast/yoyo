#include "contextmenu_operator.hpp"
#include "gui_simulator.hpp"

#include <QApplication>
#include <QContextMenuEvent>
#include <QMenu>
#include <QTest>
#include <QThread>
#include <QWidget>

#include <chrono>
#include <future>
#include <vector>

namespace yoyo
{
namespace gui
{
namespace test
{
// each action contains of
//  - waiting time
//  - a number of key-clicks down
//  - a minimum number of children for the next instance to proceed
//  - waiting time
using context_menu_action =
  std::tuple<std::chrono::milliseconds, size_t, size_t, std::chrono::milliseconds>;
class contextmenu_worker : public QObject
{
  Q_OBJECT
public:
  auto setMenu(QMenu* m) -> void
  {
    _menu = m;
  }
  auto setTimeout(std::chrono::milliseconds t)
  {
    _preTimeout = t;
  }

  auto setParameter(std::vector<context_menu_action> actions) -> std::future<bool>;

public Q_SLOTS:
  void start();

private:
  QMenu* _menu { nullptr };
  std::chrono::milliseconds _preTimeout { 100 };
  std::promise<bool> _finished;
  std::vector<context_menu_action> _actions;
};

std::future<bool> contextmenu_worker::setParameter(std::vector<context_menu_action> actions)
{
  _finished = std::promise<bool>();
  _actions = actions;
  return _finished.get_future();
}

void contextmenu_worker::start()
{
  if (!_menu) {
    throw std::runtime_error("no menu set for context menu processing");
  }

  auto findVisibleMenu = [](QWidget* parent, int requiredChildren) {
    return std::find_if(parent->children().begin(), parent->children().end(),
                        [requiredChildren](QObject* c) {
                          if (std::string { c->metaObject()->className() } == "QMenu") {
                            if (c->children().count() < requiredChildren) {
                              return false;
                            } else {
                              return dynamic_cast<QWidget*>(c)->isVisible();
                            }
                          }

                          return false;
                        });
  };

  QTest::qWait(_preTimeout.count());
  auto currentMenu = _menu;

  for (size_t i = 0; i < _actions.size() - 1; i++) {
    auto [pre_timeout, clicks, children, post_timeou] = _actions[i];

    QTest::qWait(pre_timeout.count());
    keyClicks(currentMenu, std::vector<Qt::Key>(clicks, Qt::Key_Down), {}, {});
    keyClick(dynamic_cast<QWidget*>(currentMenu), Qt::Key_Right, {});
    QTest::qWait(post_timeou.count());

    auto menu = findVisibleMenu(currentMenu, children);

    while (menu == currentMenu->children().end()) {
      QTest::qWait(5);
      menu = findVisibleMenu(currentMenu, children);
    }

    currentMenu = dynamic_cast<QMenu*>(*menu);
  }

  auto [pre_timeout, clicks, children, post_timeou] = _actions.back();

  QTest::qWait(pre_timeout.count());
  keyClicks(currentMenu, std::vector<Qt::Key>(clicks, Qt::Key_Down), {}, {});
  keyClick(dynamic_cast<QWidget*>(currentMenu), Qt::Key_Return, {});
  QTest::qWait(post_timeou.count());

  _finished.set_value(true);
}

class contextmenu_operator::impl
{
public:
  QWidget* _widget { nullptr };
  QMenu* _menu { nullptr };
  QPoint _onPoint;
  QPoint _globalPoint;
  QThread* _thread { nullptr };
  Qt::KeyboardModifiers _modifiers;
  std::chrono::milliseconds _preTimeout { 100 };
  std::vector<context_menu_action> _actions;
};

contextmenu_operator::contextmenu_operator(QWidget* w)
  : _p { std::make_unique<impl>() }
{
  _p->_widget = w;
}

contextmenu_operator::~contextmenu_operator() = default;

auto contextmenu_operator::point(QPoint const& p) -> contextmenu_operator&
{
  _p->_onPoint = p;
  return *this;
}

auto contextmenu_operator::menu(QMenu* m) -> contextmenu_operator&
{
  _p->_menu = m;
  return *this;
}

auto contextmenu_operator::modifiers(Qt::KeyboardModifiers modifier) -> contextmenu_operator&
{
  _p->_modifiers = modifier;
  return *this;
}

auto contextmenu_operator::globalPoint(QPoint const& p) -> contextmenu_operator&
{
  _p->_globalPoint = p;
  return *this;
}

auto contextmenu_operator::useThread(QThread* t) -> contextmenu_operator&
{
  _p->_thread = t;
  return *this;
}

auto contextmenu_operator::addMenuInteraction(std::chrono::milliseconds wait_before_action,
                                              size_t menu_item_to_be_selected,
                                              size_t minimum_children_next_menu,
                                              std::chrono::milliseconds wait_after_action)
  -> contextmenu_operator&
{
  _p->_actions.push_back(std::make_tuple(wait_before_action,
                                         (_p->_actions.empty() ? 1 : 0) + menu_item_to_be_selected,
                                         minimum_children_next_menu, wait_after_action));
  return *this;
}

auto contextmenu_operator::timeoutForMenu(std::chrono::milliseconds t) -> contextmenu_operator&
{
  _p->_preTimeout = t;
  return *this;
}

auto contextmenu_operator::operate() -> void
{
  if (!_p->_menu) {
    throw std::runtime_error("No menu set for context menu operation");
  }

  if (!_p->_widget) {
    throw std::runtime_error("No widget set for context menu operation");
  }

  if (_p->_actions.empty()) {
    throw std::runtime_error("No actions set for context menu operation");
  }

  QThread* useThread = _p->_thread;

  if (!useThread) {
    useThread = new QThread;
  }

  contextmenu_worker t;
  t.setMenu(_p->_menu);
  auto meta = t.metaObject();
  t.moveToThread(useThread);
  useThread->start();
  auto f = t.setParameter(_p->_actions);
  meta->method(meta->methodOffset()).invoke(&t, Qt::QueuedConnection);

  auto e = new QContextMenuEvent(QContextMenuEvent::Keyboard, _p->_onPoint, _p->_globalPoint,
                                 _p->_modifiers);
  QApplication::postEvent(_p->_widget, e);
  QApplication::processEvents();

  f.get();
  QTest::qWait(10);
  useThread->terminate();

  if (!_p->_thread) {
    delete useThread;
  }
}
} // namespace test
} // namespace gui
} // namespace yoyo
#include "contextmenu_operator.moc"
