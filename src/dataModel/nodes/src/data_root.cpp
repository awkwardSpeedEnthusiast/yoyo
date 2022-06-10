#include "data_root.hpp"
#include "factory_provider.hpp"

#include "yoyo/data_node.h"

namespace yoyo
{
boost::uuids::uuid const data_root::_typeId = fundamental::data_root_id;

data_root::data_root(boost::uuids::uuid identifier)
  : node_base("Signals", identifier)
{
}

auto data_root::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto data_root::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto data_root::acceptsChildren() const -> bool
{
  return true;
}

auto data_root::acceptsChild(std::shared_ptr<node_base> const& child) const -> bool
{
  return child->metaObject()->inherits(&data_node::staticMetaObject);
}

auto data_root::acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool
{
  return parent->metaObject()->className() == QString("yoyo::configuration");
}

auto data_root::childAboutToBeAdded(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto data_root::childAdded(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto data_root::childAboutToBeRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto data_root::childRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto data_root::childMoved(std::shared_ptr<node_base> const& /*child*/, size_t /*from_index*/,
                           size_t /*to_index*/) -> void
{
}
} // namespace yoyo
