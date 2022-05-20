#include "data_group.hpp"
#include <boost/uuid/string_generator.hpp>

namespace yoyo
{
boost::uuids::uuid const data_group::_typeId =
  boost::uuids::string_generator {}("7c5b4a90-9ce5-46fb-9a76-0255753a8871");
data_group::data_group(boost::uuids::uuid identifier)
  : data_container_node("group", identifier, types::value_t::CUSTOM)
{
}

types::variant_t data_group::value() const
{
  return {};
}

auto data_group::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto data_group::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}
} // namespace yoyo
