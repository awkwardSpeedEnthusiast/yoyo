#include "yoyo/data_container_node.h"

namespace yoyo
{
data_container_node::data_container_node(QString type_name, boost::uuids::uuid id,
                                         types::value_t valuetype)
  : data_node(type_name, id, valuetype, false)
{
}

auto data_container_node::setValue(QVariant const& /*value*/) -> void {}

auto data_container_node::acceptsChildren() const -> bool
{
  return true;
}

auto data_container_node::acceptsChild(std::shared_ptr<node_base> const& /*child*/) const -> bool
{
  return true;
}

auto data_container_node::acceptsParent(std::shared_ptr<node_base> const& /*parent*/) const -> bool
{
  return true;
}

auto data_container_node::childAboutToBeAdded(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto data_container_node::childAdded(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto data_container_node::childAboutToBeRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto data_container_node::childRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto data_container_node::childMoved(std::shared_ptr<node_base> const&, size_t, size_t) -> void {}
} // namespace yoyo
