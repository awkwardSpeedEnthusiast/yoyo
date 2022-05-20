#pragma once

#include "nodes_export.h"
#include "yoyo/gui_node.h"

namespace yoyo
{
class YOYO_NODES_SHARED_EXPORT gui_root final : public gui_node
{
  Q_OBJECT

public:
  explicit gui_root(boost::uuids::uuid identifier = boost::uuids::uuid {});
  virtual ~gui_root() = default;
  gui_root(gui_root const& other) = delete;
  gui_root& operator=(gui_root const& other) = delete;
  gui_root(gui_root&& other) = delete;
  gui_root& operator=(gui_root&& other) = delete;

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto acceptsChildren() const -> bool final;
  auto acceptsChild(std::shared_ptr<node_base> const& child) const -> bool final;
  auto acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool final;
  auto supportsValueType(types::value_t valueType) const -> bool final;

  static auto typeId() -> boost::uuids::uuid;

private:
  auto beforeChildAdd(std::shared_ptr<node_base> const& child) -> bool final;
  auto afterChildAdd(std::shared_ptr<node_base> const& child) -> bool final;
  auto beforeChildRemove(std::shared_ptr<node_base> const& child) -> bool final;
  auto afterChildRemove(std::shared_ptr<node_base> const& child) -> bool final;
  auto afterChildMoved(std::shared_ptr<node_base> const& child, size_t from_index, size_t to_index)
    -> void final;

private:
  static boost::uuids::uuid const _typeId;
};
} // namespace yoyo
