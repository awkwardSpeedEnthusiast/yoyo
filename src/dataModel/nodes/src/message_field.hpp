#pragma once

#include "nodes_export.h"
#include "yoyo/node_base.h"

#include <QVariant>

#include <boost/signals2.hpp>

namespace yoyo
{
class YOYO_NODES_SHARED_EXPORT message_field final : public node_base
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::connection_t connection READ connection WRITE setConnection NOTIFY
               connectionChanged)
  Q_PROPERTY(
    yoyo::properties::limited_uint8_t bitPos READ bitPos WRITE setBitPos NOTIFY bitPosChanged)
  Q_PROPERTY(
    uint16_t bufferLength READ bufferLength WRITE setBufferLength NOTIFY bufferLengthChanged)
  Q_PROPERTY(bool bufferAsPackage READ bufferAsPackage WRITE setBufferAsPackage NOTIFY
               bufferAsPackageChanged)

public:
  message_field(boost::uuids::uuid id = boost::uuids::uuid {});
  ~message_field() override = default;
  message_field(message_field const&) = delete;
  message_field(message_field&&) = delete;
  message_field& operator=(message_field const&) = delete;
  message_field& operator=(message_field&&) = delete;

  auto propagateValue(QVariant v) -> void;

  auto connection() const -> yoyo::properties::connection_t;
  auto bitPos() const -> yoyo::properties::limited_uint8_t;
  auto bufferLength() const -> uint16_t;
  auto bufferAsPackage() const -> bool;

  auto setConnection(yoyo::properties::connection_t const& v) -> void;
  auto setBitPos(yoyo::properties::limited_uint8_t const& v) -> void;
  auto setBufferLength(uint16_t v) -> void;
  auto setBufferAsPackage(bool v) -> void;

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

Q_SIGNALS:
  void connectionChanged(yoyo::properties::connection_t v);
  void bitPosChanged(yoyo::properties::limited_uint8_t v);
  void bufferLengthChanged(uint16_t v);
  void bufferAsPackageChanged(bool v);

  void valueReceived(QVariant v);

private:
  static boost::uuids::uuid const _typeId;

  yoyo::properties::connection_t _connection;
  yoyo::properties::limited_uint8_t _bitPos { 0, 0, 64 };
  uint16_t _bufferLength { 0 };
  bool _bufferAsPackage { false };

  boost::signals2::signal<void(QVariant)> _signal;
};

} // namespace yoyo
