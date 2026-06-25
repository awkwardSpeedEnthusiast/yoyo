#pragma once

#include "nodes_export.h"
#include "yoyo/gui_node.h"

#include <memory>

namespace yoyo
{
class YOYO_NODES_SHARED_EXPORT group final : public gui_node
{
  Q_OBJECT

  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
public:
  explicit group(boost::uuids::uuid identifier = boost::uuids::uuid {});
  ~group() override;
  group(group const& other) = delete;
  group& operator=(group const& other) = delete;
  group(group&& other) = delete;
  group& operator=(group&& other) = delete;

  auto init() -> void final;
  auto staticTypeId() const -> boost::uuids::uuid final;
  auto acceptsChildren() const -> bool final;
  auto acceptsChild(std::shared_ptr<node_base> const& child) const -> bool final;
  auto acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool final;
  auto supportsValueType(types::value_t valueType) const -> bool final;

  auto title() const -> QString;
  auto setTitle(QString const& t) -> void;

  static auto typeId() -> boost::uuids::uuid;

Q_SIGNALS:
  void titleChanged(QString t);

private:
  auto beforeChildAdd(std::shared_ptr<node_base> const& child) -> bool final;
  auto afterChildAdd(std::shared_ptr<node_base> const& child) -> bool final;
  auto beforeChildRemove(std::shared_ptr<node_base> const& child) -> bool final;
  auto afterChildRemove(std::shared_ptr<node_base> const& child) -> bool final;
  auto afterChildMoved(std::shared_ptr<node_base> const& child, size_t from_index, size_t to_index)
    -> void final;

private:
  static boost::uuids::uuid const _typeId;
  struct Impl;
  std::unique_ptr<Impl> _p;
};
} // namespace yoyo
