#include "uint8node.hpp"

#include <QVariant>

#include <boost/uuid/string_generator.hpp>

namespace yoyo
{
boost::uuids::uuid const uint8node::_typeId =
  boost::uuids::string_generator {}("72caae27-f422-4cb8-8d46-30b01805e313");

uint8node::uint8node(boost::uuids::uuid identifier)
  : data_value_node("uint8", identifier, types::value_t::UINT8)
{
}

auto uint8node::value() const -> types::variant_t
{
  return _bufferedValue;
}

auto uint8node::setValue(QVariant const& value) -> void
{
  if ((access() != types::access_t::READ_WRITE) && (access() != types::access_t::WRITE)) {
    return;
  }

  if (!value.canConvert<uint8_t>()) {
    return;
  }

  _bufferedValue = value.value<uint8_t>();

  emitValueChanged(value);
}

auto uint8node::toolTip() const -> QString
{
  return _toolTip;
}

auto uint8node::setToolTip(QString v) -> void
{
  if (_toolTip == v) {
    return;
  }

  _toolTip = v;
  Q_EMIT toolTipChanged(_toolTip);
}

auto uint8node::min() const -> uint8_t
{
  return _min;
}

auto uint8node::setMin(uint8_t v) -> void
{
  if (_min == v) {
    return;
  }

  _min = v;
  Q_EMIT minChanged(_min);
}

auto uint8node::max() const -> uint8_t
{
  return _max;
}

auto uint8node::setMax(uint8_t v) -> void
{
  if (_max == v) {
    return;
  }

  _max = v;
  Q_EMIT maxChanged(_max);
}

auto uint8node::format() const -> properties::integer_format_t
{
  return _format;
}

auto uint8node::setFormat(properties::integer_format_t v) -> void
{
  if (_format == v) {
    return;
  }

  _format = v;
  Q_EMIT formatChanged(_format);
}

auto uint8node::enumeration() const -> properties::enum_t
{
  return _enum;
}

auto uint8node::setEnumeration(properties::enum_t v) -> void
{
  if (_enum == v) {
    return;
  }

  _enum = v;
  Q_EMIT enumerationChanged(_enum);
}

auto uint8node::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto uint8node::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}
} // namespace yoyo
