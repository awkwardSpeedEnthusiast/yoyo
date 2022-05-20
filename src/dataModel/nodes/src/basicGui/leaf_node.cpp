#include "leaf_node.hpp"

#include <boost/uuid/string_generator.hpp>
namespace yoyo
{
auto leaf_node::staticTypeId() const -> boost::uuids::uuid
{
  return {};
}

auto leaf_node::acceptsChildren() const -> bool
{
  return false;
}

auto leaf_node::acceptsChild(std::shared_ptr<node_base> const& /*child*/) const -> bool
{
  return false;
}

auto leaf_node::acceptsParent(std::shared_ptr<node_base> const& /*parent*/) const -> bool
{
  return true;
}

auto leaf_node::beforeChildAdd(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return false;
}

auto leaf_node::afterChildAdd(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return false;
}

auto leaf_node::beforeChildRemove(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return false;
}

auto leaf_node::afterChildRemove(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return false;
}

auto leaf_node::afterChildMoved(std::shared_ptr<node_base> const& /*child*/, size_t /*from_index*/,
                                size_t /*to_index*/) -> void
{
}
} // namespace yoyo
