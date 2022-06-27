#include "configuration.hpp"

#include "data_root.hpp"
#include "factory_provider.hpp"
#include "gui_root.hpp"

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace yoyo
{
boost::uuids::uuid const configuration::_typeId = fundamental::configuration_id;

configuration::configuration(boost::uuids::uuid identifier)
  : node_base("Configuration", identifier)
{
}
auto configuration::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto configuration::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto configuration::acceptsChildren() const -> bool
{
  return true;
}

auto configuration::acceptsChild(std::shared_ptr<node_base> const& child) const -> bool
{
  return child->staticTypeId() == fundamental::data_root_id
      || child->staticTypeId() == fundamental::gui_root_id
      || child->staticTypeId() == fundamental::configuration_data_id;
}

auto configuration::acceptsParent(std::shared_ptr<node_base> const&) const -> bool
{
  return false;
}

auto configuration::version() const -> yoyo::properties::invisible_string_t
{
  return _version;
}

auto configuration::childAboutToBeAdded(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto configuration::childAdded(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto configuration::childAboutToBeRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto configuration::childRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto configuration::childMoved(std::shared_ptr<node_base> const& /*child*/, size_t /*from_index*/,
                               size_t /*to_index*/) -> void
{
}
} // namespace yoyo
