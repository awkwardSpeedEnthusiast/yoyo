#include "yoyo/node_base.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/uuid/random_generator.hpp>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;
using testing::WithArg;
using op_t = yoyo::node_base::ChangeOperation;

namespace yoyo
{
class node_mock : public node_base
{
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
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
};
boost::uuids::uuid node_mock::_id;
} // namespace yoyo

TEST(nodeBaseTest, properties)
{
  yoyo::node_mock object("mock_node");
  EXPECT_EQ(object.type(), "mock_node");
  EXPECT_FALSE(object.identifier().is_nil());
  EXPECT_EQ(object.name()._s, "");

  struct Receiver {
    MOCK_METHOD(void, onNameChanged, (yoyo::properties::invisible_string_t name));
  } receiver;

  QObject::connect(&object, &yoyo::node_base::nameChanged,
                   [&receiver](auto name) { receiver.onNameChanged(name); });

  EXPECT_CALL(receiver, onNameChanged(yoyo::properties::invisible_string_t { "foo", true }));
  object.setName({ "foo", true });
  EXPECT_EQ(object.name()._s, "foo");

  EXPECT_CALL(receiver, onNameChanged(yoyo::properties::invisible_string_t { "bar", false }));
  object.setName({ "bar", false });
  EXPECT_EQ(object.name()._s, "bar");

  auto id = boost::uuids::random_generator {}();
  yoyo::node_mock object2 { "mock2", id };
  EXPECT_EQ(object2.identifier(), id);
}

TEST(nodeBaseTest, structure)
{
  auto root = std::make_shared<StrictMock<yoyo::node_mock>>("root-type");
  auto n1 = std::make_shared<StrictMock<yoyo::node_mock>>("child-type");
  auto n2 = std::make_shared<StrictMock<yoyo::node_mock>>("child-type");
  auto n3 = std::make_shared<StrictMock<yoyo::node_mock>>("child-type");
  auto n4 = std::make_shared<StrictMock<yoyo::node_mock>>("child-type");

  struct Receiver {
    MOCK_METHOD(void, onTreeChanged, (std::weak_ptr<yoyo::node_base> node, op_t op));
    MOCK_METHOD(void, onExchangeRequested,
                (std::shared_ptr<yoyo::node_base> node, boost::uuids::uuid id));
    MOCK_METHOD(void, onAddRequested,
                (std::shared_ptr<yoyo::node_base> node, boost::uuids::uuid id, int index));
  } receiver;
  QObject::connect(root.get(), &yoyo::node_base::treeChanged,
                   [&receiver](auto n, auto, auto, auto op) { receiver.onTreeChanged(n, op); });
  QObject::connect(root.get(), &yoyo::node_base::exchangeRequested,
                   [&receiver](auto n, auto id) { receiver.onExchangeRequested(n, id); });
  QObject::connect(root.get(), &yoyo::node_base::addRequested,
                   [&receiver](auto n, auto id, auto i) { receiver.onAddRequested(n, id, i); });

  EXPECT_EQ(root->childCount(), 0);
  auto checkForRoot = [root](auto n) {
    EXPECT_EQ(n.lock(), root) << n.lock()->type().toStdString();
  };
  auto checkForN1 = [n1](auto n) { EXPECT_EQ(n.lock(), n1) << n.lock()->type().toStdString(); };

  // Null objects and self are not accepted as children.
  {
    root->addChild({});
    EXPECT_EQ(root->childCount(), 0);
    root->addChild(root);
    EXPECT_EQ(root->childCount(), 0);
  }
  // Can fail on either parent not accepting children or child not accepting parent.
  {
    testing::Sequence seq;
    EXPECT_CALL(*root, acceptsChild(_)).InSequence(seq).WillOnce(Return(false));
    root->addChild(n1);
    EXPECT_EQ(root->childCount(), 0);
  }
  {
    testing::Sequence seq;
    EXPECT_CALL(*root, acceptsChild(_)).InSequence(seq).WillOnce(Return(true));
    EXPECT_CALL(*n1, acceptsParent(_)).InSequence(seq).WillOnce(Return(false));
    root->addChild(n1);
    EXPECT_EQ(root->childCount(), 0);
  }
  // Parent notification about process of adding child.
  {
    testing::Sequence seq;
    EXPECT_CALL(*root, acceptsChild(std::dynamic_pointer_cast<yoyo::node_base>(n1)))
      .InSequence(seq)
      .WillOnce(Return(true));
    EXPECT_CALL(*n1, acceptsParent(std::dynamic_pointer_cast<yoyo::node_base>(root)))
      .InSequence(seq)
      .WillOnce(Return(true));
    EXPECT_CALL(*root, childAboutToBeAdded(std::dynamic_pointer_cast<yoyo::node_base>(n1)))
      .InSequence(seq);
    EXPECT_CALL(*root, childAdded(std::dynamic_pointer_cast<yoyo::node_base>(n1))).InSequence(seq);
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::PRE_ADD));
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::ADDED)).WillOnce(WithArg<0>(Invoke(checkForRoot)));
    root->addChild(n1);
    EXPECT_EQ(root->childCount(), 1);
    EXPECT_EQ(root->childAt(0), n1);
    EXPECT_EQ(n1->parent().lock(), root);
  }
  // Same child must not be added twice.
  {
    testing::Sequence seq;
    EXPECT_CALL(*root, acceptsChild(std::dynamic_pointer_cast<yoyo::node_base>(n1)))
      .InSequence(seq)
      .WillOnce(Return(true));
    EXPECT_CALL(*n1, acceptsParent(std::dynamic_pointer_cast<yoyo::node_base>(root)))
      .InSequence(seq)
      .WillOnce(Return(true));
    root->addChild(n1);
    EXPECT_EQ(root->childCount(), 1);
  }

  {
    testing::Sequence seq;
    EXPECT_CALL(*root, acceptsChild(std::dynamic_pointer_cast<yoyo::node_base>(n2)))
      .InSequence(seq)
      .WillOnce(Return(true));
    EXPECT_CALL(*n2, acceptsParent(std::dynamic_pointer_cast<yoyo::node_base>(root)))
      .InSequence(seq)
      .WillOnce(Return(true));
    EXPECT_CALL(*root, childAboutToBeAdded(std::dynamic_pointer_cast<yoyo::node_base>(n2)))
      .InSequence(seq);
    EXPECT_CALL(*root, childAdded(std::dynamic_pointer_cast<yoyo::node_base>(n2))).InSequence(seq);
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::PRE_ADD));
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::ADDED)).WillOnce(WithArg<0>(Invoke(checkForRoot)));
    root->addChild(n2);
    EXPECT_EQ(root->childCount(), 2);
    EXPECT_EQ(root->childAt(0), n1);
    EXPECT_EQ(root->childAt(1), n2);
    EXPECT_EQ(n2->parent().lock(), root);
  }
  // Adding child at defined position.
  {
    testing::Sequence seq;
    EXPECT_CALL(*root, acceptsChild(std::dynamic_pointer_cast<yoyo::node_base>(n3)))
      .InSequence(seq)
      .WillOnce(Return(true));
    EXPECT_CALL(*n3, acceptsParent(std::dynamic_pointer_cast<yoyo::node_base>(root)))
      .InSequence(seq)
      .WillOnce(Return(true));
    EXPECT_CALL(*root, childAboutToBeAdded(std::dynamic_pointer_cast<yoyo::node_base>(n3)))
      .InSequence(seq);
    EXPECT_CALL(*root, childAdded(std::dynamic_pointer_cast<yoyo::node_base>(n3))).InSequence(seq);
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::PRE_ADD));
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::ADDED)).WillOnce(WithArg<0>(Invoke(checkForRoot)));
    root->addChild(n3, 1);
    EXPECT_EQ(root->childCount(), 3);
    EXPECT_EQ(root->childAt(0), n1);
    EXPECT_EQ(root->childAt(1), n3);
    EXPECT_EQ(root->childAt(2), n2);
    EXPECT_EQ(n3->parent().lock(), root);
    EXPECT_EQ(root->childIndex(n1), 0);
    EXPECT_EQ(root->childIndex(n2), 2);
    EXPECT_EQ(root->childIndex(n3), 1);
    EXPECT_EQ(root->childIndex(n4), -1);
  }
  // Internal move.
  {
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::PRE_MOVE));
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::MOVED)).WillOnce(WithArg<0>(Invoke(checkForRoot)));
    EXPECT_CALL(*root, childMoved(_, 1, 0));
    root->moveChild(n3, 0);
    EXPECT_EQ(root->childCount(), 3);
    EXPECT_EQ(root->childAt(0), n3);
    EXPECT_EQ(root->childAt(1), n1);
    EXPECT_EQ(root->childAt(2), n2);
  }
  {
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::PRE_MOVE));
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::MOVED)).WillOnce(WithArg<0>(Invoke(checkForRoot)));
    EXPECT_CALL(*root, childMoved(_, 1, 2));
    root->moveChild(n1, 3);
    EXPECT_EQ(root->childCount(), 3);
    EXPECT_EQ(root->childAt(0), n3);
    EXPECT_EQ(root->childAt(1), n2);
    EXPECT_EQ(root->childAt(2), n1);
  }
  // Adding child to subtree also invokes tree changed signal on root
  {
    testing::Sequence seq;
    EXPECT_CALL(*n1, acceptsChild(std::dynamic_pointer_cast<yoyo::node_base>(n4)))
      .InSequence(seq)
      .WillOnce(Return(true));
    EXPECT_CALL(*n4, acceptsParent(std::dynamic_pointer_cast<yoyo::node_base>(n1)))
      .InSequence(seq)
      .WillOnce(Return(true));
    EXPECT_CALL(*n1, childAboutToBeAdded(std::dynamic_pointer_cast<yoyo::node_base>(n4)))
      .InSequence(seq);
    EXPECT_CALL(*n1, childAdded(std::dynamic_pointer_cast<yoyo::node_base>(n4))).InSequence(seq);
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::PRE_ADD));
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::ADDED)).WillOnce(WithArg<0>(Invoke(checkForN1)));
    n1->addChild(n4);
    EXPECT_EQ(root->childCount(), 3);
    EXPECT_EQ(n1->childCount(), 1);
    EXPECT_EQ(n1->childAt(0), n4);
    EXPECT_EQ(n4->parent().lock(), n1);

    // Checking other hierarchy propagating signals
    auto id = boost::uuids::random_generator {}();
    EXPECT_CALL(receiver, onExchangeRequested(_, id)).WillOnce(WithArg<0>(Invoke([n4](auto n) {
      EXPECT_EQ(n, std::dynamic_pointer_cast<yoyo::node_base>(n4));
    })));
    n4->exchangeRequested(n4, id);
    EXPECT_CALL(receiver, onAddRequested(_, id, 3)).WillOnce(WithArg<0>(Invoke([n4](auto n) {
      EXPECT_EQ(n, std::dynamic_pointer_cast<yoyo::node_base>(n4));
    })));
    n4->addRequested(n4, id, 3);
  }
  // Adding child reparents, if child has already parent
  {
    testing::Sequence seq;
    EXPECT_CALL(*n1, acceptsChild(std::dynamic_pointer_cast<yoyo::node_base>(n2)))
      .InSequence(seq)
      .WillOnce(Return(true));
    EXPECT_CALL(*n2, acceptsParent(std::dynamic_pointer_cast<yoyo::node_base>(n1)))
      .InSequence(seq)
      .WillOnce(Return(true));
    EXPECT_CALL(*root, childAboutToBeRemoved(std::dynamic_pointer_cast<yoyo::node_base>(n2)))
      .InSequence(seq);
    EXPECT_CALL(*root, childRemoved(std::dynamic_pointer_cast<yoyo::node_base>(n2)))
      .InSequence(seq);
    EXPECT_CALL(*n1, childAboutToBeAdded(std::dynamic_pointer_cast<yoyo::node_base>(n2)))
      .InSequence(seq);
    EXPECT_CALL(*n1, childAdded(std::dynamic_pointer_cast<yoyo::node_base>(n2))).InSequence(seq);
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::PRE_REMOVE));
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::REMOVED))
      .WillOnce(WithArg<0>(Invoke(checkForRoot)));
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::PRE_ADD));
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::ADDED)).WillOnce(WithArg<0>(Invoke(checkForN1)));
    n1->addChild(n2);
    EXPECT_EQ(root->childCount(), 2);
    EXPECT_EQ(n1->childCount(), 2);
    EXPECT_EQ(n1->childAt(1), n2);
    EXPECT_EQ(n2->parent().lock(), n1);
  }
  // Removing children
  {
    testing::Sequence seq;
    EXPECT_CALL(*n1, childAboutToBeRemoved(std::dynamic_pointer_cast<yoyo::node_base>(n2)))
      .InSequence(seq);
    EXPECT_CALL(*n1, childRemoved(std::dynamic_pointer_cast<yoyo::node_base>(n2))).InSequence(seq);
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::PRE_REMOVE));
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::REMOVED)).WillOnce(WithArg<0>(Invoke(checkForN1)));
    n1->removeChild(n2);
    EXPECT_EQ(root->childCount(), 2);
    EXPECT_EQ(n1->childCount(), 1);
    EXPECT_EQ(n2->parent().lock(), nullptr);
  }
  {
    testing::Sequence seq;
    EXPECT_CALL(*root, childAboutToBeRemoved(std::dynamic_pointer_cast<yoyo::node_base>(n1)))
      .InSequence(seq);
    EXPECT_CALL(*root, childRemoved(std::dynamic_pointer_cast<yoyo::node_base>(n1)))
      .InSequence(seq);
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::PRE_REMOVE));
    EXPECT_CALL(receiver, onTreeChanged(_, op_t::REMOVED))
      .WillOnce(WithArg<0>(Invoke(checkForRoot)));
    root->removeChild(1);
    EXPECT_EQ(root->childCount(), 1);
    EXPECT_EQ(n1->parent().lock(), nullptr);
  }
  // Cannot remove nodes which are no children
  {
    root->removeChild(nullptr);
    EXPECT_EQ(root->childCount(), 1);
    root->removeChild(n1);
    EXPECT_EQ(root->childCount(), 1);
  }

  // Necessary to clear expectations for these objects, because they use shared_ptr to other objects
  // in expectations. This causes the objects not to be destroyed.
  testing::Mock::VerifyAndClear(root.get());
  testing::Mock::VerifyAndClear(n1.get());
  testing::Mock::VerifyAndClear(n2.get());
  testing::Mock::VerifyAndClear(n3.get());
  testing::Mock::VerifyAndClear(n4.get());
}

TEST(aProperties, PropertyTypes)
{
  yoyo::registerAllPropertyTypes();
  EXPECT_TRUE(QMetaTypeId2<yoyo::types::value_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::types::access_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::types::variant_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::types::layout_direction_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::string_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::invisible_string_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::invisible_layout_direction_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::connected_string_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::connected_boolean_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::connection_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::script_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::enum_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::patterned_string_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::limited_uint8_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::limited_uint16_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::limited_uint32_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::limited_uint64_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::limited_int8_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::limited_int16_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::limited_int32_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::limited_int64_t>::Defined);
  EXPECT_TRUE(QMetaTypeId2<yoyo::properties::limited_float_t>::Defined);
}
