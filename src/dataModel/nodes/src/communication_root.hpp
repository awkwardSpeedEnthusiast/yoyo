#pragma once

#include "nodes_export.h"
#include "yoyo/node_base.h"

namespace yoyo
{
class YOYO_NODES_SHARED_EXPORT communication_root final : public node_base
{
  Q_OBJECT
public:
  communication_root(boost::uuids::uuid id = boost::uuids::uuid {});
  ~communication_root() override;
  communication_root(communication_root const&) = delete;
  communication_root(communication_root&&) = delete;
  communication_root& operator=(communication_root const&) = delete;
  communication_root& operator=(communication_root&&) = delete;

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto acceptsChildren() const -> bool final;
  auto acceptsChild(std::shared_ptr<node_base> const& child) const -> bool final;
  auto acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool final;
  static auto typeId() -> boost::uuids::uuid;

private:
  auto childAboutToBeAdded(std::shared_ptr<node_base> const& child) -> bool final;
  auto childAdded(std::shared_ptr<node_base> const& child) -> bool final;
  auto childAboutToBeRemoved(std::shared_ptr<node_base> const& child) -> bool final;
  auto childRemoved(std::shared_ptr<node_base> const& child) -> bool final;
  auto childMoved(std::shared_ptr<node_base> const& child, size_t from_index, size_t to_index)
    -> void final;

private:
  static boost::uuids::uuid const _typeId;

  struct impl;
  std::unique_ptr<impl> _p;
};

} // namespace yoyo
