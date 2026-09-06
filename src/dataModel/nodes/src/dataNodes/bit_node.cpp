#include "bit_node.hpp"

#include <QVariant>

#include <boost/uuid/string_generator.hpp>

namespace yoyo
{
boost::uuids::uuid const bit_node::_typeId =
  boost::uuids::string_generator {}("f8920b3b-eda3-4a1c-bd94-5a7b998d2cfc");

bit_node::bit_node(boost::uuids::uuid identifier)
  : data_value_node("bit", identifier, types::value_t::BIT)
{
  setUnit({ "", false });
}

auto bit_node::value() const -> types::variant_t
{
  return _bufferedValue;
}

auto bit_node::setValue(QVariant const& value) -> void
{
  if ((access() != types::access_t::READ_WRITE) && (access() != types::access_t::WRITE)) {
    return;
  }

  if (value.metaType() != QMetaType::fromType<bool>()) {
    return;
  }

  if (_bufferedValue == value.toBool()) {
    return;
  }

  _bufferedValue = value.toBool();
  doValueChanged(value);
}

auto bit_node::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto bit_node::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}
} // namespace yoyo
