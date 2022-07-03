#pragma once

#include "nodes_export.h"
#include "yoyo/node_base.h"

namespace yoyo
{
class YOYO_NODES_SHARED_EXPORT message_container final : public node_base
{
  Q_OBJECT
public:
  message_container(boost::uuids::uuid id = boost::uuids::uuid {});
  ~message_container() override;
  message_container(message_container const&) = delete;
  message_container(message_container&&) = delete;
  message_container& operator=(message_container const&) = delete;
  message_container& operator=(message_container&&) = delete;

  auto incommingData(QByteArray const& data);
  auto incommingData(QByteArray const& data, uint32_t id);

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto acceptsChildren() const -> bool final;
  auto acceptsChild(std::shared_ptr<node_base> const& child) const -> bool final;
  auto acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool final;
  static auto typeId() -> boost::uuids::uuid;

Q_SIGNALS:
  void dataReady(QByteArray data);
  void dataIncomming(QByteArray data);

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
