#include "line_edit.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QMetaType>
#include <QVariant>

#include <boost/signals2.hpp>
#include <boost/uuid/string_generator.hpp>

#include <set>

namespace yoyo
{
boost::uuids::uuid const line_edit::_typeId =
  boost::uuids::string_generator {}("81c2df83-2452-422c-8846-d8bfa2d756df");

struct line_edit::Impl {
  Impl()
    : _connection { [this](std::function<void(QVariant)> subscriber) {
                     return _signal.connect(subscriber);
                   },
                    [this](QVariant value) {
                      if (value.isValid()) {
                        _edit->setText(value.toString());
                      }
                    },
                    [this](types::value_t t) { return supports(t); },
                    "",
                    "",
                    "" }
    , _edit(new QLineEdit)
    , _title_widget(new QLabel)
    , _unit_widget(new QLabel)
  {
    _edit->setObjectName("__lineEdit");
    _title_widget->setObjectName("__title");
    _unit_widget->setObjectName("__unit");
  }
  yoyo::properties::connected_string_t _title;
  yoyo::properties::connected_string_t _unit;
  boost::signals2::signal<void(QVariant)> _signal;
  properties::connection_t _connection;
  QLineEdit* _edit { nullptr };
  QLabel* _title_widget { nullptr };
  QLabel* _unit_widget { nullptr };
  bool _capture_key { false };

  auto supports(types::value_t t) -> bool
  {
    static std::set<types::value_t> const supported {
      types::value_t::BITFIELD, types::value_t::STRING, types::value_t::FLOAT,
      types::value_t::UINT8,    types::value_t::UINT16, types::value_t::UINT32,
      types::value_t::UINT64,   types::value_t::INT8,   types::value_t::INT16,
      types::value_t::INT32,    types::value_t::INT64,  types::value_t::REGISTER,
    };
    return supported.find(t) != supported.end();
  }
};

line_edit::line_edit(boost::uuids::uuid identifier)
  : leaf_node { "line_edit", nullptr, identifier }
  , _p { std::make_unique<Impl>() }
{
  connect(_p->_edit, &QLineEdit::editingFinished, this,
          [this]() { _p->_signal(_p->_edit->text()); });
}

void line_edit::init()
{
  gui_node::init();
  addWidget(_p->_title_widget);
  addWidget(_p->_edit);
  addWidget(_p->_unit_widget);
  _p->_title_widget->hide();
  _p->_unit_widget->hide();
}

line_edit::~line_edit() = default;
auto line_edit::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto line_edit::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto line_edit::supportsValueType(types::value_t valueType) const -> bool
{
  return _p->supports(valueType);
}

auto line_edit::title() const -> yoyo::properties::connected_string_t
{
  return _p->_title;
}

auto line_edit::setTitle(yoyo::properties::connected_string_t const& t) -> void
{
  if (_p->_title == t) {
    return;
  }

  _p->_title = t;
  _p->_title_widget->setText(_p->_title.value());

  if (_p->_title.value().isEmpty()) {
    _p->_title_widget->hide();
  } else {
    _p->_title_widget->show();
  }

  Q_EMIT titleChanged(_p->_title);
}

auto line_edit::unit() const -> yoyo::properties::connected_string_t
{
  return _p->_unit;
}

auto line_edit::setUnit(yoyo::properties::connected_string_t const& t) -> void
{
  if (_p->_unit == t) {
    return;
  }

  _p->_unit = t;
  _p->_unit_widget->setText(_p->_unit.value());

  if (_p->_unit.value().isEmpty()) {
    _p->_unit_widget->hide();
  } else {
    _p->_unit_widget->show();
  }

  Q_EMIT unitChanged(_p->_unit);
}

auto line_edit::connection() const -> properties::connection_t
{
  return _p->_connection;
}

auto line_edit::setConnection(properties::connection_t s) -> void
{
  if (_p->_connection == s) {
    return;
  }

  _p->_connection.assign(s);
  Q_EMIT connectionChanged(_p->_connection);
}

auto line_edit::captureKey() const -> bool
{
  return _p->_capture_key;
}

auto line_edit::setCaptureKey(bool v) -> void
{
  if (_p->_capture_key == v) {
    return;
  }

  _p->_capture_key = v;

  if (_p->_capture_key) {
    _p->_edit->clear();
    disconnect(_p->_edit, &QLineEdit::editingFinished, this, nullptr);
    connect(_p->_edit, &QLineEdit::textEdited, this, [this](QString v) {
      _p->_signal(v);
      _p->_edit->clear();
    });
  } else {
    disconnect(_p->_edit, &QLineEdit::textEdited, this, nullptr);
    connect(_p->_edit, &QLineEdit::editingFinished, this,
            [this]() { _p->_signal(_p->_edit->text()); });
  }

  Q_EMIT captureKeyChanged(_p->_capture_key);
}
} // namespace yoyo
