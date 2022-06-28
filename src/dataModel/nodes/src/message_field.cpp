#include "message_field.hpp"

#include "factory_provider.hpp"

#include "yoyo/documentation_utilities.h"

namespace yoyo
{
boost::uuids::uuid const message_field::_typeId = fundamental::message_field_id;

message_field::message_field(boost::uuids::uuid id)
  : node_base { "Message field", id }
  , _connection { [this](std::function<void(QVariant)> subscriber) {
                   return _signal.connect(subscriber);
                 },
                  [this](QVariant value) { Q_EMIT valueReceived(value); },
                  [](types::value_t) { return true; },
                  "",
                  "",
                  "" }
{
  setName({ "Message field", false });
}

auto message_field::propagateValue(QVariant v) -> void
{
  _signal(v);
}

auto message_field::connection() const -> yoyo::properties::connection_t
{
  return _connection;
}

auto message_field::bitPos() const -> yoyo::properties::limited_uint8_t
{
  return _bitPos;
}

auto message_field::bufferLength() const -> uint16_t
{
  return _bufferLength;
}

auto message_field::bufferAsPackage() const -> bool
{
  return _bufferAsPackage;
}

auto message_field::setConnection(yoyo::properties::connection_t const& v) -> void
{
  if (_connection == v) {
    return;
  }
  _connection.assign(v);
  Q_EMIT connectionChanged(_connection);
}

auto message_field::setBitPos(yoyo::properties::limited_uint8_t const& v) -> void
{
  if (_bitPos == v) {
    return;
  }
  _bitPos.assign(v);
  Q_EMIT bitPosChanged(_bitPos);
}

auto message_field::setBufferLength(uint16_t v) -> void
{
  if (_bufferLength == v) {
    return;
  }
  _bufferLength = v;
  Q_EMIT bufferLengthChanged(_bufferLength);
}

auto message_field::setBufferAsPackage(bool v) -> void
{
  if (_bufferAsPackage == v) {
    return;
  }
  _bufferAsPackage = v;
  Q_EMIT bufferAsPackageChanged(_bufferAsPackage);
}

auto message_field::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto message_field::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto message_field::acceptsChildren() const -> bool
{
  return false;
}
auto message_field::acceptsChild(std::shared_ptr<node_base> const&) const -> bool
{
  return false;
}
auto message_field::acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool
{
  return parent->staticTypeId() == fundamental::message_id;
}
auto message_field::childAboutToBeAdded(std::shared_ptr<node_base> const&) -> bool
{
  return false;
}
auto message_field::childAdded(std::shared_ptr<node_base> const&) -> bool
{
  return false;
}
auto message_field::childAboutToBeRemoved(std::shared_ptr<node_base> const&) -> bool
{
  return false;
}
auto message_field::childRemoved(std::shared_ptr<node_base> const&) -> bool
{
  return false;
}
auto message_field::childMoved(std::shared_ptr<node_base> const&, size_t, size_t) -> void {}

} // namespace yoyo
