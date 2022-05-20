#include "yoyo/protocol_node.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QMetaProperty>

#include <boost/uuid/random_generator.hpp>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::SizeIs;
using testing::StrictMock;

namespace yoyo
{
class protocol_node_mock : public protocol_node
{
  Q_OBJECT
public:
  using protocol_node::protocol_node;
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
  MOCK_METHOD(void, serialize, (QVariant const& v), (override));
  MOCK_METHOD(void, deserialize, (QVariant const& v), (override));
  MOCK_METHOD(bool, outputSupports, (types::value_t t), (const, override));
  MOCK_METHOD(bool, inputSupports, (types::value_t t), (const, override));
  using protocol_node::notify_value_on_input_changed;
  using protocol_node::notify_value_on_output_changed;
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
};
boost::uuids::uuid protocol_node_mock::_id;
} // namespace yoyo

TEST(ProtocolTests, properties)
{
  auto object = std::make_shared<StrictMock<yoyo::protocol_node_mock>>("mock");
  auto meta = object->metaObject();
  EXPECT_EQ(meta->propertyCount() - QObject::staticMetaObject.propertyCount(), 4);
  EXPECT_GE(meta->indexOfProperty("name"), yoyo::node_base::staticMetaObject.propertyOffset());
  EXPECT_GE(meta->indexOfProperty("input"), yoyo::protocol_node::staticMetaObject.propertyOffset());
  EXPECT_GE(meta->indexOfProperty("output"),
            yoyo::protocol_node::staticMetaObject.propertyOffset());

  struct Receiver {
    MOCK_METHOD(void, nameChanged, (QString n));
    MOCK_METHOD(void, inputChanged, (yoyo::properties::connection_t c));
    MOCK_METHOD(void, outputChanged, (yoyo::properties::connection_t c));
  } receiver;

  QObject::connect(object.get(), &yoyo::protocol_node::nameChanged,
                   [&receiver](auto n) { receiver.nameChanged(n); });
  QObject::connect(object.get(), &yoyo::protocol_node::inputChanged,
                   [&receiver](auto n) { receiver.inputChanged(n); });
  QObject::connect(object.get(), &yoyo::protocol_node::outputChanged,
                   [&receiver](auto n) { receiver.outputChanged(n); });

  {
    auto nameProperty = meta->property(meta->indexOfProperty("name"));
    EXPECT_EQ(object->name().toStdString(), "");
    EXPECT_CALL(receiver, nameChanged(QString("myObject")));
    object->setName("myObject");
    EXPECT_EQ(object->name().toStdString(), "myObject");
    EXPECT_CALL(receiver, nameChanged(QString("foo")));
    nameProperty.write(object.get(), "foo");
    EXPECT_EQ(nameProperty.read(object.get()).toString().toStdString(), "foo");
    EXPECT_CALL(receiver, nameChanged(_)).Times(0);
    object->setName("foo");
  }

  {
    auto inputProperty = meta->property(meta->indexOfProperty("input"));
    auto c = object->input();
    c._in = "foo.bar";
    EXPECT_CALL(receiver, inputChanged(c));
    object->setInput(c);
    EXPECT_EQ(object->input(), c);

    c._out = "bar.bar.foo";
    EXPECT_CALL(receiver, inputChanged(c));
    inputProperty.write(object.get(), QVariant::fromValue(c));
    EXPECT_EQ(inputProperty.read(object.get()).value<yoyo::properties::connection_t>(), c);
    EXPECT_CALL(receiver, inputChanged(_)).Times(0);
    object->setInput(c);
  }

  {
    auto outputProperty = meta->property(meta->indexOfProperty("output"));
    auto c = object->output();
    c._in = "foo.bar";
    EXPECT_CALL(receiver, outputChanged(c));
    object->setOutput(c);
    EXPECT_EQ(object->output(), c);

    c._out = "bar.bar.foo";
    EXPECT_CALL(receiver, outputChanged(c));
    outputProperty.write(object.get(), QVariant::fromValue(c));
    EXPECT_EQ(outputProperty.read(object.get()).value<yoyo::properties::connection_t>(), c);
    EXPECT_CALL(receiver, outputChanged(_)).Times(0);
    object->setOutput(c);
  }
}

TEST(ProtocolTests, input)
{
  auto object = std::make_shared<StrictMock<yoyo::protocol_node_mock>>("mock");
  auto c = object->input();

  struct Receiver {
    MOCK_METHOD(void, valueReceived, (QVariant v));
  } receiver;

  auto connection = c._connector([&receiver](auto v) { receiver.valueReceived(v); });

  EXPECT_CALL(receiver, valueReceived(QVariant(5)));
  object->notify_value_on_input_changed(5);

  EXPECT_CALL(*object, serialize(QVariant(42)));
  c._setter(42);

  EXPECT_CALL(*object, inputSupports(yoyo::types::value_t::INT8)).WillOnce(Return(true));
  EXPECT_TRUE(c._supports(yoyo::types::value_t::INT8));

  EXPECT_CALL(*object, inputSupports(yoyo::types::value_t::STRING)).WillOnce(Return(false));
  EXPECT_FALSE(c._supports(yoyo::types::value_t::STRING));
}

TEST(ProtocolTests, output)
{
  auto object = std::make_shared<StrictMock<yoyo::protocol_node_mock>>("mock");
  auto c = object->output();

  struct Receiver {
    MOCK_METHOD(void, valueReceived, (QVariant v));
  } receiver;

  auto connection = c._connector([&receiver](auto v) { receiver.valueReceived(v); });

  EXPECT_CALL(receiver, valueReceived(QVariant(5)));
  object->notify_value_on_output_changed(5);

  EXPECT_CALL(*object, deserialize(QVariant(42)));
  c._setter(42);

  EXPECT_CALL(*object, outputSupports(yoyo::types::value_t::INT8)).WillOnce(Return(true));
  EXPECT_TRUE(c._supports(yoyo::types::value_t::INT8));

  EXPECT_CALL(*object, outputSupports(yoyo::types::value_t::STRING)).WillOnce(Return(false));
  EXPECT_FALSE(c._supports(yoyo::types::value_t::STRING));
}

#include "protocol_node_tests.moc"
