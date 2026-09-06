#pragma once

#include "nodes_export.h"
#include "yoyo/node_base.h"

namespace yoyo
{
class YOYO_NODES_SHARED_EXPORT data_root final : public node_base
{
  Q_OBJECT

public:
  explicit data_root(boost::uuids::uuid identifier = boost::uuids::uuid {});
  virtual ~data_root() = default;
  data_root(data_root const& other) = delete;
  data_root& operator=(data_root const& other) = delete;
  data_root(data_root&& other) = delete;
  data_root& operator=(data_root&& other) = delete;

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
};
} // namespace yoyo
