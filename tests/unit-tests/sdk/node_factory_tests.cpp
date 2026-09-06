#include "yoyo/node_factory.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/uuid/random_generator.hpp>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::SizeIs;
using testing::StrictMock;

namespace yoyo
{
class node_mock : public node_base
{
  Q_OBJECT
public:
  using node_base::node_base;
  MOCK_METHOD(bool, acceptsChildren, (), (const, override));
  MOCK_METHOD(bool, acceptsChild, (std::shared_ptr<node_base> const& child), (const, override));
  MOCK_METHOD(bool, acceptsParent, (std::shared_ptr<node_base> const& parent), (const, override));
  MOCK_METHOD(bool, childAboutToBeAdded, (std::shared_ptr<node_base> const& child), (override));
  MOCK_METHOD(bool, childAdded, (std::shared_ptr<node_base> const& child), (override));
  MOCK_METHOD(bool, childAboutToBeRemoved, (std::shared_ptr<node_base> const& child), (override));
  MOCK_METHOD(bool, childRemoved, (std::shared_ptr<node_base> const& child), (override));
  MOCK_METHOD(void, childMoved,
              (std::shared_ptr<node_base> const& child, size_t from_index, size_t to_index),
              (override));
};
class node_mock1 : public node_mock
{
  Q_OBJECT
public:
  using node_mock::node_mock;
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
};
class node_mock2 : public node_mock
{
  Q_OBJECT
public:
  using node_mock::node_mock;
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
};
boost::uuids::uuid node_mock1::_id;
boost::uuids::uuid node_mock2::_id;

TEST(nodeFactoryTests, installUninstall)
{
  node_factory factory(&QObject::staticMetaObject);
  auto id1 = boost::uuids::random_generator {}();
  auto id2 = boost::uuids::random_generator {}();
  auto doc = documentation::builder { "test" }.build();
  node_mock1::_id = id1;
  node_mock2::_id = id2;

  EXPECT_TRUE(factory.installed_nodes().empty());

  EXPECT_FALSE(factory.install_node(
    id1, "mock1", &node_base::staticMetaObject, doc,
    [](auto) { return std::shared_ptr<node_base> {}; },
    [](auto) { return node_factory::node_id_list {}; }));
  EXPECT_FALSE(factory.install_node(
    id1, "mock1", &node_mock1::staticMetaObject, {},
    [](auto id) { return std::make_shared<node_mock1>("", id); },
    [](auto) { return node_factory::node_id_list {}; }));
  EXPECT_TRUE(factory.installed_nodes().empty());
  EXPECT_FALSE(factory.install_node(
    id1, "", &node_mock1::staticMetaObject, doc,
    [](auto id) { return std::make_shared<node_mock1>("", id); },
    [](auto) { return node_factory::node_id_list {}; }));
  EXPECT_TRUE(factory.installed_nodes().empty());
  EXPECT_FALSE(factory.install_node(
    {}, "mock1", &node_mock1::staticMetaObject, doc,
    [](auto id) { return std::make_shared<node_mock1>("", id); },
    [](auto) { return node_factory::node_id_list {}; }));
  EXPECT_TRUE(factory.installed_nodes().empty());
  // Cannot uninstall what hasn't been installed.
  EXPECT_FALSE(factory.uninstall_node(id1));

  EXPECT_TRUE(factory.install_node(
    id1, "mock1", &node_mock1::staticMetaObject, doc,
    [](auto id) { return std::make_shared<node_mock1>("", id); },
    [](auto) { return node_factory::node_id_list {}; }));
  EXPECT_THAT(factory.installed_nodes(), SizeIs(1));
  EXPECT_FALSE(factory.install_node(
    id1, "mock1", &node_mock1::staticMetaObject, doc,
    [](auto id) { return std::make_shared<node_mock1>("", id); },
    [](auto) { return node_factory::node_id_list {}; }));
  EXPECT_THAT(factory.installed_nodes(), SizeIs(1));
  EXPECT_TRUE(factory.install_node(
    id2, "mock2", &node_mock2::staticMetaObject, doc,
    [](auto id) { return std::make_shared<node_mock2>("", id); },
    [](auto) { return node_factory::node_id_list {}; }));
  ASSERT_THAT(factory.installed_nodes(), SizeIs(2));
  EXPECT_EQ(std::get<0>(factory.installed_nodes()[0]), id1);
  EXPECT_EQ(std::get<0>(factory.installed_nodes()[1]), id2);
  EXPECT_EQ(std::get<2>(factory.installed_nodes()[0]), "mock1");
  EXPECT_EQ(std::get<2>(factory.installed_nodes()[1]), "mock2");

  EXPECT_TRUE(factory.uninstall_node(id1));
  ASSERT_THAT(factory.installed_nodes(), SizeIs(1));
  EXPECT_EQ(std::get<0>(factory.installed_nodes()[0]), id2);
  EXPECT_EQ(std::get<2>(factory.installed_nodes()[0]), "mock2");

  node_factory restricted_factory(&node_mock1::staticMetaObject);

  EXPECT_FALSE(restricted_factory.install_node(
    id2, "mock2", &node_mock2::staticMetaObject, doc,
    [](auto id) { return std::make_shared<node_mock2>("", id); },
    [](auto) { return node_factory::node_id_list {}; }));
}

TEST(nodeFactoryTests, nodeCreation)
{
  auto id1 = boost::uuids::random_generator {}();
  auto id2 = boost::uuids::random_generator {}();
  auto test_id = boost::uuids::random_generator {}();
  auto doc1 = documentation::builder { "test mock 1" }.build();
  auto doc2 = documentation::builder { "test mock 2" }.build();

  node_factory factory { &QObject::staticMetaObject };
  EXPECT_TRUE(factory.installed_nodes().empty());

  EXPECT_TRUE(factory.install_node(
    id1, "mock1", &node_mock1::staticMetaObject, doc1,
    [](auto id) -> std::shared_ptr<node_base> { return std::make_shared<node_mock1>("ba", id); },
    [](auto) { return node_factory::node_id_list {}; }));
  EXPECT_TRUE(factory.install_node(
    id2, "mock2", &node_mock2::staticMetaObject, doc2,
    [](auto id) -> std::shared_ptr<node_base> { return std::make_shared<node_mock2>("ba2", id); },
    [id1](auto) {
      return node_factory::node_id_list { { id1, &node_mock1::staticMetaObject, "mock1" } };
    }));

  auto n1 = factory.createNode(id1, {});
  ASSERT_NE(n1, nullptr);
  EXPECT_FALSE(n1->identifier().is_nil());
  EXPECT_EQ(n1->type(), "ba");
  EXPECT_EQ(factory.node_documentation(id1), doc1);
  EXPECT_TRUE(factory.child_list(id1).empty());
  auto n11 = factory.createNode(id1, test_id);
  ASSERT_NE(n11, nullptr);
  EXPECT_EQ(n11->identifier(), test_id);
  EXPECT_EQ(n11->type(), "ba");

  auto n2 = factory.createNode(id2, {});
  ASSERT_NE(n2, nullptr);
  EXPECT_FALSE(n2->identifier().is_nil());
  EXPECT_EQ(n2->type(), "ba2");
  EXPECT_EQ(factory.node_documentation(id2), doc2);
  EXPECT_THAT(factory.child_list(id2), testing::Contains(std::make_tuple(
                                         id1, &node_mock1::staticMetaObject, QString { "mock1" })));
  auto n21 = factory.createNode(id2, test_id);
  ASSERT_NE(n21, nullptr);
  EXPECT_EQ(n21->identifier(), test_id);
  EXPECT_EQ(n21->type(), "ba2");

  // Invalid id returns invalid objects:
  EXPECT_TRUE(factory.child_list({}).empty());
  EXPECT_EQ(factory.node_documentation({}), nullptr);
  EXPECT_EQ(factory.createNode({}, {}), nullptr);
}
} // namespace yoyo
#include "node_factory_tests.moc"
