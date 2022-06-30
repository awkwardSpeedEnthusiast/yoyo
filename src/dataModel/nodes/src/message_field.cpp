#include "message_field.hpp"

#include "factory_provider.hpp"

#include "yoyo/documentation_utilities.h"

#include <bitset>

namespace yoyo
{
boost::uuids::uuid const message_field::_typeId = fundamental::message_field_id;

message_field::message_field(boost::uuids::uuid id)
  : node_base { "Message field", id }
  , _connection { [this](std::function<void(QVariant)> subscriber) {
                   return _signal.connect(subscriber);
                 },
                  [this](QVariant value) { dataOutgoing(value); },
                  [](types::value_t) { return true; },
                  "",
                  "",
                  "" }
{
  setName({ "Message field", false });
}

auto message_field::setStreaming(bool isStreaming) -> void
{
  _isStreaming = isStreaming;
}

auto message_field::incommingData(QByteArray const& v) -> void
{
  if (_isStreaming) {
    _signal(v);
  } else {
    auto pos = _bitPos._value;
    if (v.count() * 8 < pos + _bufferLength) {
      return;
    }
    QByteArray out_data;
    if (pos % 8 == 0) {
      for (auto i = pos / 8; i < (pos + _bufferLength) / 8; i++) {
        out_data.append(v[i]);
      }
      if (_bufferLength % 8) {
        auto tail = v[(pos + _bufferLength) / 8];
        std::bitset<8> set = tail;
        for (auto i = _bufferLength % 8; i < 8; i++) {
          set.reset(i);
        }
        out_data.append(static_cast<char>(set.to_ulong()));
      }
    } else {
      auto index = pos / 8;
      auto bit = pos % 8;
      std::bitset<8> set;

      for (auto i = 0; i < _bufferLength / 8; i++) {
        set = v[index + i];
        set <<= bit;
        set = (std::bitset<8>(v[index + i + 1]) << bit);
        out_data.append(static_cast<char>(set.to_ulong()));
      }
      if (_bufferLength % 8) {
        set = v[index + _bufferLength / 8];
        set <<= bit;
        for (auto i = _bufferLength % 8; i < 8; i++) {
          set.reset(i);
        }
        out_data.append(static_cast<char>(set.to_ulong()));
      }
    }
    _signal(out_data);
  }
}

auto message_field::dataOutgoing(QVariant const& v) -> void
{
  Q_EMIT outgoingData(v.toByteArray());
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
