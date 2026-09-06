#include "yoyo/data_value_node.h"

#include <QVariant>

namespace yoyo
{
struct data_value_node::impl {
  types::access_t _access { types::access_t::READ_WRITE };
  QString _title;
  yoyo::properties::invisible_string_t _unit;
  properties::script_t _onChange;
};

data_value_node::data_value_node(QString type_name, boost::uuids::uuid id, types::value_t valuetype)
  : data_node(type_name, id, valuetype, true)
  , _p { std::make_unique<impl>() }
{
}

data_value_node::~data_value_node() = default;

auto data_value_node::setValue(QVariant const& value) -> void
{
  if ((_p->_access != types::access_t::READ_WRITE) && (_p->_access != types::access_t::WRITE)) {
    return;
  }

  doValueChanged(value);
}

void yoyo::data_value_node::doValueChanged(QVariant v)
{
  emitValueChanged(std::move(v));

  if (!_p->_onChange._text.isEmpty()) {
    Q_EMIT scriptExecutionRequested(_p->_onChange);
  }
}

auto data_value_node::access() const -> types::access_t
{
  return _p->_access;
}

auto data_value_node::setAccess(types::access_t acc) -> void
{
  if (acc == _p->_access) {
    return;
  }

  _p->_access = acc;
  Q_EMIT accessChanged(_p->_access);
}

auto data_value_node::title() const -> QString
{
  return _p->_title;
}

auto data_value_node::setTitle(QString const& t) -> void
{
  if (t == _p->_title) {
    return;
  }

  _p->_title = t;
  Q_EMIT titleChanged(_p->_title);
}

auto data_value_node::unit() const -> yoyo::properties::invisible_string_t
{
  return _p->_unit;
}

auto data_value_node::setUnit(yoyo::properties::invisible_string_t const& u) -> void
{
  if (u == _p->_unit) {
    return;
  }

  _p->_unit = u;
  Q_EMIT unitChanged(_p->_unit);
}

auto data_value_node::onChange() const -> yoyo::properties::script_t
{
  return _p->_onChange;
}

auto data_value_node::setOnChange(yoyo::properties::script_t s) -> void
{
  if (s == _p->_onChange) {
    return;
  }

  _p->_onChange = s;
  Q_EMIT onChangeChanged(_p->_onChange);
}

auto data_value_node::acceptsChildren() const -> bool
{
  return false;
}

auto data_value_node::acceptsChild(std::shared_ptr<node_base> const& /*child*/) const -> bool
{
  return false;
}

auto data_value_node::acceptsParent(std::shared_ptr<node_base> const& /*parent*/) const -> bool
{
  return true;
}

auto data_value_node::childAboutToBeAdded(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto data_value_node::childAdded(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto data_value_node::childAboutToBeRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto data_value_node::childRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

void data_value_node::childMoved(std::shared_ptr<node_base> const&, size_t, size_t) {}
} // namespace yoyo
