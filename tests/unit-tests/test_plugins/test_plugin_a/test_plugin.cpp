#include "test_plugin.hpp"

#include "yoyo/communication_node.h"
#include "yoyo/data_node.h"
#include "yoyo/gui_node.h"
#include "yoyo/node_factory.h"
#include "yoyo/protocol_node.h"

#include <boost/uuid/random_generator.hpp>

namespace
{
auto const id_data = boost::uuids::random_generator {}();
auto const id_gui = boost::uuids::random_generator {}();
auto const id_communication = boost::uuids::random_generator {}();
auto const id_protocol = boost::uuids::random_generator {}();
} // namespace

namespace yoyo::test
{
class test_gui_node_a : public gui_node
{
  Q_OBJECT
  auto supportsValueType(types::value_t) const -> bool override
  {
    return false;
  }
  auto beforeChildAdd(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto afterChildAdd(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto beforeChildRemove(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto afterChildRemove(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto afterChildMoved(std::shared_ptr<node_base> const&, size_t, size_t) -> void override {}
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return {};
  }
  auto acceptsChildren() const -> bool override
  {
    return false;
  }
  auto acceptsChild(std::shared_ptr<node_base> const&) const -> bool override
  {
    return false;
  }
  auto acceptsParent(std::shared_ptr<node_base> const&) const -> bool override
  {
    return false;
  }

public:
  using gui_node::gui_node;
};

class test_data_node_a : public data_node
{
  Q_OBJECT
  auto setValue(QVariant const&) -> void override {}
  auto value() const -> types::variant_t override
  {
    return {};
  }
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return {};
  }
  auto acceptsChildren() const -> bool override
  {
    return false;
  }
  auto acceptsChild(std::shared_ptr<node_base> const&) const -> bool override
  {
    return false;
  }
  auto acceptsParent(std::shared_ptr<node_base> const&) const -> bool override
  {
    return false;
  }
  auto childAboutToBeAdded(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto childAdded(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto childAboutToBeRemoved(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto childRemoved(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto childMoved(std::shared_ptr<node_base> const&, size_t, size_t) -> void override {}

public:
  using data_node::data_node;
};

class test_protocol_node_a : public protocol_node
{
  Q_OBJECT
  auto serialize(QVariant const&) -> void override {}
  auto deserialize(QVariant const&) -> void override {}
  auto outputSupports(types::value_t) const -> bool override
  {
    return false;
  }
  auto inputSupports(types::value_t) const -> bool override
  {
    return false;
  }
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return {};
  }
  auto acceptsChildren() const -> bool override
  {
    return false;
  }
  auto acceptsChild(std::shared_ptr<node_base> const&) const -> bool override
  {
    return false;
  }
  auto acceptsParent(std::shared_ptr<node_base> const&) const -> bool override
  {
    return false;
  }
  auto childAboutToBeAdded(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto childAdded(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto childAboutToBeRemoved(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto childRemoved(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto childMoved(std::shared_ptr<node_base> const&, size_t, size_t) -> void override {}

public:
  using protocol_node::protocol_node;
};

class test_communication_node_a : public communication_node
{
  Q_OBJECT
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return {};
  }
  auto acceptsChildren() const -> bool override
  {
    return false;
  }
  auto acceptsChild(std::shared_ptr<node_base> const&) const -> bool override
  {
    return false;
  }
  auto acceptsParent(std::shared_ptr<node_base> const&) const -> bool override
  {
    return false;
  }
  auto childAboutToBeAdded(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto childAdded(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto childAboutToBeRemoved(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto childRemoved(std::shared_ptr<node_base> const&) -> bool override
  {
    return false;
  }
  auto childMoved(std::shared_ptr<node_base> const&, size_t, size_t) -> void override {}

public:
  using communication_node::communication_node;
};

auto test_plugin::name() const -> QString
{
  return "test-plugin-a";
}

auto test_plugin::install_gui_nodes(node_factory& gui_factory) -> void
{
  gui_factory.install_node(
    id_gui, "test_gui_node", &test_gui_node_a::staticMetaObject,
    documentation::builder("test_gui_node_a").build(),
    [](auto id) { return std::make_shared<test_gui_node_a>("test_gui_node_a", nullptr, id); },
    [](auto) { return node_factory::node_id_list {}; });
}

auto test_plugin::install_data_nodes(node_factory& data_factory) -> void
{
  data_factory.install_node(
    id_data, "test_data_node_a", &test_data_node_a::staticMetaObject,
    documentation::builder("test_data_node_a").build(),
    [](auto id) {
      return std::make_shared<test_data_node_a>("test_data_node_a", id, types::value_t::CUSTOM,
                                                false);
    },
    [](auto) { return node_factory::node_id_list {}; });
}

auto test_plugin::install_communication_nodes(node_factory& communication_factory) -> void
{
  communication_factory.install_node(
    id_communication, "test_communication_node_a", &test_communication_node_a::staticMetaObject,
    documentation::builder("test_communication_node_a").build(),
    [](auto id) {
      return std::make_shared<test_communication_node_a>("test_communication_node_a", id);
    },
    [](auto) { return node_factory::node_id_list {}; });
}

auto test_plugin::install_protocol_nodes(node_factory& protocol_factory) -> void
{
  protocol_factory.install_node(
    id_protocol, "test_protocol_node_a", &test_protocol_node_a::staticMetaObject,
    documentation::builder("test_protocol_node_a").build(),
    [](auto id) { return std::make_shared<test_protocol_node_a>("test_protocol_node", id); },
    [](auto) { return node_factory::node_id_list {}; });
}

auto test_plugin::uninstall_gui_nodes(node_factory& gui_factory) -> void
{
  gui_factory.uninstall_node(id_gui);
}

auto test_plugin::uninstall_data_nodes(node_factory& data_factory) -> void
{
  data_factory.uninstall_node(id_data);
}

auto test_plugin::uninstall_communication_nodes(node_factory& communication_factory) -> void
{
  communication_factory.uninstall_node(id_communication);
}

auto test_plugin::uninstall_protocol_nodes(node_factory& protocol_factory) -> void
{
  protocol_factory.uninstall_node(id_protocol);
}
} // namespace yoyo::test
#include "test_plugin.moc"
