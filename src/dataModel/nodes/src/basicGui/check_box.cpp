#include "check_box.hpp"

#include <QCheckBox>
#include <QMetaType>
#include <QVariant>

#include <boost/signals2.hpp>
#include <boost/uuid/string_generator.hpp>

namespace yoyo
{
boost::uuids::uuid const check_box::_typeId =
  boost::uuids::string_generator {}("6538cef0-6592-470f-91f3-80d4423b46ce");

struct check_box::Impl {
  Impl()
    : _connection { [this](std::function<void(QVariant)> subscriber) {
                     return _signal.connect(subscriber);
                   },
                    [this](QVariant value) {
                      if (value.metaType() == QMetaType::fromType<bool>()) {
                        _box->setChecked(value.toBool());
                      }
                    },
                    [this](types::value_t t) { return supports(t); },
                    "",
                    "",
                    "" }
    , _box(new QCheckBox)
  {
    _box->setObjectName("__checkBox");
  }

  bool supports(types::value_t t)
  {
    return t == types::value_t::BIT;
  }
  yoyo::properties::connected_string_t _title;
  boost::signals2::signal<void(QVariant)> _signal;
  properties::connection_t _connection;
  QCheckBox* _box { nullptr };
};

check_box::check_box(boost::uuids::uuid identifier)
  : leaf_node { "check_box", nullptr, identifier }
  , _p { std::make_unique<Impl>() }
{
  connect(_p->_box, &QCheckBox::toggled, this, [this](bool checked) { _p->_signal(checked); });
}

void check_box::init()
{
  gui_node::init();
  addWidget(_p->_box);
}

check_box::~check_box() = default;
auto check_box::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto check_box::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto check_box::supportsValueType(types::value_t valueType) const -> bool
{
  return _p->supports(valueType);
}

auto check_box::title() const -> yoyo::properties::connected_string_t
{
  return _p->_title;
}

auto check_box::setTitle(yoyo::properties::connected_string_t const& t) -> void
{
  if (_p->_title == t) {
    return;
  }

  _p->_title = t;
  _p->_box->setText(_p->_title.value());
  Q_EMIT titleChanged(_p->_title);
}

auto check_box::connection() const -> properties::connection_t
{
  return _p->_connection;
}

auto check_box::setConnection(properties::connection_t s) -> void
{
  if (_p->_connection == s) {
    return;
  }

  _p->_connection.assign(s);
  Q_EMIT connectionChanged(_p->_connection);
}
} // namespace yoyo
