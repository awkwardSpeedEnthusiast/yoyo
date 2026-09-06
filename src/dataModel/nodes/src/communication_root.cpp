#include "communication_root.hpp"

#include "factory_provider.hpp"
#include "message_container.hpp"

#include "yoyo/communication_node.h"

#include <QtGlobal>

namespace yoyo
{
boost::uuids::uuid const communication_root::_typeId = fundamental::communication_root_id;

struct communication_root::impl {
  QMetaObject::Connection _out_connection;
  QMetaObject::Connection _in_connection;
  QMetaObject::Connection _in_connection_id;
};

communication_root::communication_root(boost::uuids::uuid id)
  : node_base { "ComInterface", id }
  , _p { std::make_unique<impl>() }
{
}

communication_root::~communication_root() = default;

auto communication_root::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto communication_root::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto communication_root::acceptsChildren() const -> bool
{
  return true;
}

auto communication_root::acceptsChild(std::shared_ptr<node_base> const& child) const -> bool
{
  if (child->staticTypeId() == fundamental::message_container_id
      && std::find_if(std::begin(*this), std::end(*this), [](auto const& node) {
           return node->staticTypeId() == fundamental::message_container_id;
         }) == std::end(*this)) {
    return true;
  }
  if (child->metaObject()->inherits(&communication_node::staticMetaObject)
      && std::find_if(std::begin(*this), std::end(*this), [](auto const& node) {
           return node->metaObject()->inherits(&communication_node::staticMetaObject);
         }) == std::end(*this)) {
    return true;
  }
  return false;
}

auto communication_root::acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool
{
  return parent->staticTypeId() == fundamental::configuration_id;
}

auto communication_root::childAboutToBeAdded(std::shared_ptr<node_base> const&) -> bool
{
  return true;
}

auto communication_root::childAdded(std::shared_ptr<node_base> const& child) -> bool
{
  if (childCount() != 2) {
    return true;
  }
  std::shared_ptr<communication_node> com;
  std::shared_ptr<message_container> messages;

  if (auto c = std::dynamic_pointer_cast<communication_node>(child)) {
    com = c;
    messages = std::dynamic_pointer_cast<message_container>(childAt(0));
  } else if (auto m = std::dynamic_pointer_cast<message_container>(child)) {
    messages = m;
    com = std::dynamic_pointer_cast<communication_node>(child);
  }

  if (!messages || !com) {
    return false;
  }

  _p->_out_connection = connect(messages.get(), &message_container::dataReady, com.get(),
                                &communication_node::send_data);
  _p->_in_connection = connect(com.get(), &communication_node::data_received, messages.get(),
                               &message_container::incommingData);
  _p->_in_connection_id = connect(com.get(), &communication_node::data_with_id_received,
                                  messages.get(), &message_container::incommingDataWithId);

  return true;
}

auto communication_root::childAboutToBeRemoved(std::shared_ptr<node_base> const&) -> bool
{
  disconnect(_p->_out_connection);
  disconnect(_p->_in_connection);
  disconnect(_p->_in_connection_id);
  return true;
}

auto communication_root::childRemoved(std::shared_ptr<node_base> const&) -> bool
{
  return true;
}

auto communication_root::childMoved(std::shared_ptr<node_base> const&, size_t, size_t) -> void {}

} // namespace yoyo
