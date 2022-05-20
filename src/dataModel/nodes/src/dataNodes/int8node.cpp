#include "int8node.hpp"

#include <QVariant>

#include <boost/uuid/string_generator.hpp>

namespace yoyo
{
boost::uuids::uuid const int8node::_typeId =
  boost::uuids::string_generator {}("c219f763-f52f-4517-82c7-780981e99fc5");
int8node::int8node(boost::uuids::uuid identifier)
  : data_value_node("int8", identifier, types::value_t::INT8)
{
}

auto int8node::value() const -> types::variant_t
{
  return _bufferedValue;
}

auto int8node::setValue(QVariant const& value) -> void
{
  if ((access() != types::access_t::READ_WRITE) && (access() != types::access_t::WRITE)) {
    return;
  }

  if (!value.canConvert<int8_t>()) {
    return;
  }

  _bufferedValue = value.value<int8_t>();

  emitValueChanged(value);
}

auto int8node::toolTip() const -> QString
{
  return _toolTip;
}

auto int8node::setToolTip(QString v) -> void
{
  if (_toolTip == v) {
    return;
  }

  _toolTip = v;
  Q_EMIT toolTipChanged(_toolTip);
}

auto int8node::min() const -> int8_t
{
  return _min;
}

auto int8node::setMin(int8_t v) -> void
{
  if (_min == v) {
    return;
  }

  _min = v;
  Q_EMIT minChanged(_min);
}

auto int8node::max() const -> int8_t
{
  return _max;
}

auto int8node::setMax(int8_t v) -> void
{
  if (_max == v) {
    return;
  }

  _max = v;
  Q_EMIT maxChanged(_max);
}

auto int8node::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto int8node::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}
} // namespace yoyo
