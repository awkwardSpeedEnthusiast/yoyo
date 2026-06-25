#include "message.hpp"
#include "message_field.hpp"

#include "factory_provider.hpp"

#include <QRegularExpression>
#include <QTimer>

namespace yoyo
{
boost::uuids::uuid const message::_typeId = fundamental::message_id;

struct message::impl {
  yoyo::properties::patterned_string_t _id { {}, QRegularExpression { "0[xX][0-9a-fA-F]+" } };
  yoyo::properties::limited_float_t _interval { 0, -1, 1000 };
  bool _sendOnNewData { false };
  yoyo::properties::transmission_direction_t _direction {
    yoyo::properties::transmission_direction_t::RX
  };
  bool _isStandard { true };
  yoyo::properties::limited_uint8_t _messageLength { 0, 0, 63 };
  bool _isStream { false };
  std::vector<QByteArray> _buffer;
  QTimer _timer;
};

message::message(boost::uuids::uuid id)
  : node_base { "Message", id }
  , _p { std::make_unique<impl>() }
{
  setName({ "Message", false });
  _p->_timer.setSingleShot(false);
  connect(&_p->_timer, &QTimer::timeout, this, [this]() {
    std::for_each(_p->_buffer.begin(), _p->_buffer.end(),
                  [this](auto data) { Q_EMIT outgoingData(data); });
  });
}

message::~message() = default;

auto message::id() const -> properties::patterned_string_t
{
  return _p->_id;
}

auto message::interval() const -> yoyo::properties::limited_float_t
{
  return _p->_interval;
}

auto message::sendOnNewData() const -> bool
{
  return _p->_sendOnNewData;
}

auto message::direction() const -> yoyo::properties::transmission_direction_t
{
  return _p->_direction;
}

auto message::isStandard() const -> bool
{
  return _p->_isStandard;
}

auto message::messageLength() const -> yoyo::properties::limited_uint8_t
{
  return _p->_messageLength;
}

auto message::isStream() const -> bool
{
  return _p->_isStream;
}

auto message::setId(yoyo::properties::patterned_string_t const& v) -> void
{
  if (_p->_id == v) {
    return;
  }
  _p->_id.assign(v);
  Q_EMIT idChanged(_p->_id);
}

auto message::setInterval(yoyo::properties::limited_float_t const& v) -> void
{
  if (_p->_interval == v) {
    return;
  }
  _p->_interval.assign(v);
  Q_EMIT intervalChanged(_p->_interval);

  _p->_timer.stop();
  if (_p->_interval._value > 0) {
    _p->_timer.setInterval(_p->_interval._value * 1000);
    _p->_timer.start();
  }
}

auto message::setSendOnNewData(bool v) -> void
{
  if (_p->_sendOnNewData == v) {
    return;
  }
  _p->_sendOnNewData = v;
  Q_EMIT sendOnNewDataChanged(_p->_sendOnNewData);
}

auto message::setDirection(yoyo::properties::transmission_direction_t const& v) -> void
{
  if (_p->_direction == v) {
    return;
  }
  _p->_direction = v;
  Q_EMIT directionChanged(_p->_direction);

  if (_p->_direction == properties::transmission_direction_t::RX) {
    for (auto& child : *this) {
      if (auto field = std::dynamic_pointer_cast<message_field>(child)) {
        disconnect(field.get(), &message_field::outgoingData, this, &message::dataOutgoing);
        connect(this, &message::dataReceived, field.get(), &message_field::incommingData);
      }
    }
  } else {
    for (auto& child : *this) {
      if (auto field = std::dynamic_pointer_cast<message_field>(child)) {
        connect(field.get(), &message_field::outgoingData, this, &message::dataOutgoing);
        disconnect(this, &message::dataReceived, field.get(), &message_field::incommingData);
      }
    }
  }
}

auto message::setIsStandard(bool v) -> void
{
  if (_p->_isStandard == v) {
    return;
  }
  _p->_isStandard = v;
  Q_EMIT isStandardChanged(_p->_isStandard);
}

auto message::setMessageLength(yoyo::properties::limited_uint8_t const& v) -> void
{
  if (_p->_messageLength == v) {
    return;
  }
  _p->_messageLength.assign(v);
  Q_EMIT messageLengthChanged(_p->_messageLength);
}

auto message::setIsStream(bool v) -> void
{
  if (_p->_isStream == v) {
    return;
  }
  _p->_isStream = v;
  Q_EMIT isStreamChanged(_p->_isStream);
}

auto message::incomingData(QByteArray const& data) -> void
{
  if (_p->_direction == properties::transmission_direction_t::TX) {
    return;
  }
  Q_EMIT dataReceived(data);
}

auto message::dataOutgoing(QByteArray const& data) -> void
{
  if (_p->_direction == properties::transmission_direction_t::RX) {
    return;
  }
  if (_p->_sendOnNewData) {
    Q_EMIT outgoingData(data);
  } else {
    _p->_buffer.push_back(data);
  }
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
    if (_p->_direction == properties::transmission_direction_t::RX) {
      connect(this, &message::dataReceived, field.get(), &message_field::incommingData);
    } else {
      connect(field.get(), &message_field::outgoingData, this, &message::dataOutgoing);
    }
  }
  return true;
}

auto message::childAboutToBeRemoved(std::shared_ptr<node_base> const& child) -> bool
{
  if (auto field = std::dynamic_pointer_cast<message_field>(child)) {
    if (_p->_direction == properties::transmission_direction_t::RX) {
      disconnect(this, &message::dataReceived, field.get(), &message_field::incommingData);
    } else {
      disconnect(field.get(), &message_field::outgoingData, this, &message::dataOutgoing);
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
