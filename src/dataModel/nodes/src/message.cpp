#include "message.hpp"
#include "message_field.hpp"

#include "factory_provider.hpp"

namespace yoyo
{
boost::uuids::uuid const message::_typeId = fundamental::message_id;

message::message(boost::uuids::uuid id)
  : node_base { "Message", id }
  , _id { {}, QRegExp { "0[xX][0-9a-fA-F]+" } }
{
}

auto message::id() const -> properties::patterned_string_t
{
  return _id;
}

auto message::interval() const -> yoyo::properties::limited_float_t
{
  return _interval;
}

auto message::sendOnNewData() const -> bool
{
  return _sendOnNewData;
}

auto message::direction() const -> yoyo::properties::transmission_direction_t
{
  return _direction;
}

auto message::isStandard() const -> bool
{
  return _isStandard;
}

auto message::messageLength() const -> yoyo::properties::limited_uint8_t
{
  return _messageLength;
}

auto message::isStream() const -> bool
{
  return _isStream;
}

auto message::setId(yoyo::properties::patterned_string_t const& v) -> void
{
  if (_id == v) {
    return;
  }
  _id.assign(v);
  Q_EMIT idChanged(_id);
}

auto message::setInterval(yoyo::properties::limited_float_t const& v) -> void
{
  if (_interval == v) {
    return;
  }
  _interval.assign(v);
  Q_EMIT intervalChanged(_interval);
}

auto message::setSendOnNewData(bool v) -> void
{
  if (_sendOnNewData == v) {
    return;
  }
  _sendOnNewData = v;
  Q_EMIT sendOnNewDataChanged(_sendOnNewData);
}

auto message::setDirection(yoyo::properties::transmission_direction_t const& v) -> void
{
  if (_direction == v) {
    return;
  }
  _direction = v;
  Q_EMIT directionChanged(_direction);

  if (_direction == properties::transmission_direction_t::RX) {
    for (auto& child : *this) {
      if (auto field = std::dynamic_pointer_cast<message_field>(child)) {
        disconnect(field.get(), &message_field::outgoingData, this, &message::outgoingData);
        connect(this, &message::dataReceived, field.get(), &message_field::incommingData);
      }
    }
  } else {
    for (auto& child : *this) {
      if (auto field = std::dynamic_pointer_cast<message_field>(child)) {
        connect(field.get(), &message_field::outgoingData, this, &message::outgoingData);
        disconnect(this, &message::dataReceived, field.get(), &message_field::incommingData);
      }
    }
  }
}

auto message::setIsStandard(bool v) -> void
{
  if (_isStandard == v) {
    return;
  }
  _isStandard = v;
  Q_EMIT isStandardChanged(_isStandard);
}

auto message::setMessageLength(yoyo::properties::limited_uint8_t const& v) -> void
{
  if (_messageLength == v) {
    return;
  }
  _messageLength.assign(v);
  Q_EMIT messageLengthChanged(_messageLength);
}

auto message::setIsStream(bool v) -> void
{
  if (_isStream == v) {
    return;
  }
  _isStream = v;
  Q_EMIT isStreamChanged(_isStream);
}

auto message::incomingData(QByteArray const& data) -> void
{
  if (_direction == properties::transmission_direction_t::TX) {
    return;
  }
  Q_EMIT dataReceived(data);
}

auto message::dataOutgoing(QByteArray const& data) -> void
{
  if (_direction == properties::transmission_direction_t::RX) {
    return;
  }
  Q_EMIT outgoingData(data);
}

auto message::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto message::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto message::acceptsChildren() const -> bool
{
  return true;
}

auto message::acceptsChild(std::shared_ptr<node_base> const& child) const -> bool
{
  return child->staticTypeId() == fundamental::message_field_id;
}

auto message::acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool
{
  return parent->staticTypeId() == fundamental::message_container_id;
}

auto message::childAboutToBeAdded(std::shared_ptr<node_base> const&) -> bool
{
  return true;
}

auto message::childAdded(std::shared_ptr<node_base> const& child) -> bool
{
  if (auto field = std::dynamic_pointer_cast<message_field>(child)) {
    connect(this, &message::isStreamChanged, field.get(), &message_field::setStreaming);
    if (_direction == properties::transmission_direction_t::RX) {
      connect(this, &message::dataReceived, field.get(), &message_field::incommingData);
    } else {
      connect(field.get(), &message_field::outgoingData, this, &message::outgoingData);
    }
  }
  return true;
}

auto message::childAboutToBeRemoved(std::shared_ptr<node_base> const& child) -> bool
{
  if (auto field = std::dynamic_pointer_cast<message_field>(child)) {
    if (_direction == properties::transmission_direction_t::RX) {
      disconnect(this, &message::dataReceived, field.get(), &message_field::incommingData);
    } else {
      disconnect(field.get(), &message_field::outgoingData, this, &message::outgoingData);
    }
    connect(this, &message::isStreamChanged, field.get(), &message_field::setStreaming);
  }
  return true;
}

auto message::childRemoved(std::shared_ptr<node_base> const&) -> bool
{
  return true;
}

auto message::childMoved(std::shared_ptr<node_base> const&, size_t, size_t) -> void {}

} // namespace yoyo
