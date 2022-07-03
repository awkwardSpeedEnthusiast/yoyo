#include "message_container.hpp"
#include "message.hpp"

#include "factory_provider.hpp"

#include <string>

using std::string_literals::operator""s;

namespace yoyo
{
boost::uuids::uuid const message_container::_typeId = fundamental::message_container_id;

struct message_container::impl {
};

message_container::message_container(boost::uuids::uuid id)
  : node_base { "DataMessageHandler", id }
{
}

message_container::~message_container() = default;

auto message_container::incommingData(QByteArray const& data)
{
  Q_EMIT dataIncomming(data);
}

auto message_container::incommingData(QByteArray const& data, uint32_t id)
{
  for (auto& child : *this) {
    if (auto m = std::dynamic_pointer_cast<message>(child)) {
      bool ok { false };
      auto child_id = m->id()._s.toUInt(&ok, 16);
      if (ok && id == child_id
          && m->direction() == yoyo::properties::transmission_direction_t::RX) {
        m->incomingData(data);
      }
    }
  }
}

auto message_container::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto message_container::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto message_container::acceptsChildren() const -> bool
{
  return true;
}

auto message_container::acceptsChild(std::shared_ptr<node_base> const& child) const -> bool
{
  return child->staticTypeId() == fundamental::message_id;
}

auto message_container::acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool
{
  return parent->staticTypeId() == fundamental::communication_root_id;
}

auto message_container::childAboutToBeAdded(std::shared_ptr<node_base> const&) -> bool
{
  return true;
}

auto message_container::childAdded(std::shared_ptr<node_base> const& child) -> bool
{
  if (auto field = std::dynamic_pointer_cast<message>(child)) {
    connect(this, &message_container::dataIncomming, field.get(), &message::incomingData);
    connect(field.get(), &message::outgoingData, this, &message_container::dataReady);
  }
  return true;
}

auto message_container::childAboutToBeRemoved(std::shared_ptr<node_base> const& child) -> bool
{
  if (auto field = std::dynamic_pointer_cast<message>(child)) {
    disconnect(this, &message_container::dataIncomming, field.get(), &message::incomingData);
    disconnect(field.get(), &message::outgoingData, this, &message_container::dataReady);
  }
  return true;
}

auto message_container::childRemoved(std::shared_ptr<node_base> const&) -> bool
{
  return true;
}

auto message_container::childMoved(std::shared_ptr<node_base> const&, size_t, size_t) -> void {}

} // namespace yoyo
