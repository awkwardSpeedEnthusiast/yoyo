#include "gui_root.hpp"
#include "factory_provider.hpp"

namespace yoyo
{
boost::uuids::uuid const gui_root::_typeId = fundamental::gui_root_id;

auto gui_root::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

gui_root::gui_root(boost::uuids::uuid identifier)
  : gui_node("Layout", nullptr, identifier)
{
}

auto gui_root::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto gui_root::acceptsChildren() const -> bool
{
  return true;
}

auto gui_root::acceptsChild(std::shared_ptr<node_base> const& child) const -> bool
{
  return child->metaObject()->inherits(&gui_node::staticMetaObject);
}

auto gui_root::acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool
{
  return parent->metaObject()->className() == QString("yoyo::configuration");
}

auto gui_root::beforeChildAdd(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto gui_root::afterChildAdd(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto gui_root::supportsValueType(types::value_t) const -> bool
{
  return false;
}

auto gui_root::beforeChildRemove(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto gui_root::afterChildRemove(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto gui_root::afterChildMoved(std::shared_ptr<node_base> const& /*child*/, size_t /*from_index*/,
                               size_t /*to_index*/) -> void
{
}
} // namespace yoyo
