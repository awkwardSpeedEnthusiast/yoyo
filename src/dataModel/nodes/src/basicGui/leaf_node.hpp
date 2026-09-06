#pragma once

#include "nodes_export.h"
#include "yoyo/gui_node.h"

namespace yoyo
{
class YOYO_NODES_SHARED_EXPORT leaf_node : public gui_node
{
  Q_OBJECT

public:
  using gui_node::gui_node;
  ~leaf_node() override = default;
  leaf_node(leaf_node const& other) = delete;
  leaf_node& operator=(leaf_node const& other) = delete;
  leaf_node(leaf_node&& other) = delete;
  leaf_node& operator=(leaf_node&& other) = delete;

  auto staticTypeId() const -> boost::uuids::uuid override;
  auto acceptsChildren() const -> bool final;
  auto acceptsChild(std::shared_ptr<node_base> const& child) const -> bool final;
  auto acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool override;

private:
  auto beforeChildAdd(std::shared_ptr<node_base> const& child) -> bool final;
  auto afterChildAdd(std::shared_ptr<node_base> const& child) -> bool final;
  auto beforeChildRemove(std::shared_ptr<node_base> const& child) -> bool final;
  auto afterChildRemove(std::shared_ptr<node_base> const& child) -> bool final;
  auto afterChildMoved(std::shared_ptr<node_base> const& child, size_t from_index, size_t to_index)
    -> void final;
};
} // namespace yoyo
