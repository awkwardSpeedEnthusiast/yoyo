#pragma once

#include "nodes_export.h"
#include "yoyo/node_base.h"

#include <QVariant>

#include <boost/signals2.hpp>

namespace yoyo
{
class YOYO_NODES_SHARED_EXPORT message final : public node_base
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::patterned_string_t id READ id WRITE setId NOTIFY idChanged)
  Q_PROPERTY(yoyo::properties::limited_float_t interval READ interval WRITE setInterval NOTIFY
               intervalChanged)
  Q_PROPERTY(
    bool sendOnNewData READ sendOnNewData WRITE setSendOnNewData NOTIFY sendOnNewDataChanged)
  Q_PROPERTY(yoyo::properties::transmission_direction_t direction READ direction WRITE setDirection
               NOTIFY directionChanged)
  Q_PROPERTY(bool isStandard READ isStandard WRITE setIsStandard NOTIFY isStandardChanged)
  Q_PROPERTY(yoyo::properties::limited_uint8_t messageLength READ messageLength WRITE
               setMessageLength NOTIFY messageLengthChanged)
  Q_PROPERTY(bool isStream READ isStream WRITE setIsStream NOTIFY isStreamChanged)

public:
  message(boost::uuids::uuid id = boost::uuids::uuid {});
  ~message() override = default;
  message(message const&) = delete;
  message(message&&) = delete;
  message& operator=(message const&) = delete;
  message& operator=(message&&) = delete;

  auto id() const -> yoyo::properties::patterned_string_t;
  auto interval() const -> yoyo::properties::limited_float_t;
  auto sendOnNewData() const -> bool;
  auto direction() const -> yoyo::properties::transmission_direction_t;
  auto isStandard() const -> bool;
  auto messageLength() const -> yoyo::properties::limited_uint8_t;
  auto isStream() const -> bool;

  auto setId(yoyo::properties::patterned_string_t const& v) -> void;
  auto setInterval(yoyo::properties::limited_float_t const& v) -> void;
  auto setSendOnNewData(bool v) -> void;
  auto setDirection(yoyo::properties::transmission_direction_t const& v) -> void;
  auto setIsStandard(bool v) -> void;
  auto setMessageLength(yoyo::properties::limited_uint8_t const& v) -> void;
  auto setIsStream(bool v) -> void;

  auto incomingData(QByteArray const& data) -> void;
  auto dataOutgoing(QByteArray const& data) -> void;

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
  void idChanged(yoyo::properties::patterned_string_t v);
  void intervalChanged(yoyo::properties::limited_float_t v);
  void sendOnNewDataChanged(bool v);
  void directionChanged(yoyo::properties::transmission_direction_t v);
  void isStandardChanged(bool v);
  void messageLengthChanged(yoyo::properties::limited_uint8_t v);
  void isStreamChanged(bool v);
  void outgoingData(QByteArray data);
  void dataReceived(QByteArray data);

private:
  static boost::uuids::uuid const _typeId;

  yoyo::properties::patterned_string_t _id;
  yoyo::properties::limited_float_t _interval;
  bool _sendOnNewData;
  yoyo::properties::transmission_direction_t _direction;
  bool _isStandard;
  yoyo::properties::limited_uint8_t _messageLength;
  bool _isStream;
};

} // namespace yoyo
