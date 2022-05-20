#include "yoyo/data_node.h"

#include <QVariant>

namespace yoyo
{
struct data_node::impl {
  impl(bool c, types::value_t v)
    : _connectable { c }
    , _value_type { v }
  {
  }

  bool const _connectable;
  boost::signals2::signal<void(QVariant)> _valueChanged;
  types::value_t const _value_type;
};

data_node::data_node(QString type_name, boost::uuids::uuid id, types::value_t valuetype,
                     bool connectable)
  : node_base(std::move(type_name), std::move(id))
  , _p { std::make_unique<impl>(connectable, valuetype) }
{
}

data_node::~data_node() = default;

auto data_node::canConnect() const -> bool
{
  return _p->_connectable;
}

auto data_node::valueType() const -> types::value_t
{
  return _p->_value_type;
}

auto data_node::connectToValue(std::function<void(QVariant)> subscriber)
  -> boost::signals2::connection
{
  if (_p->_connectable) {
    return _p->_valueChanged.connect(subscriber);
  }

  return {};
}

auto data_node::emitValueChanged(const QVariant& value) -> void
{
  _p->_valueChanged(value);
}
} // namespace yoyo
