#include "bit_node.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QVariant>

using testing::_;

TEST(BitNodeTest, properties)
{
  auto object = std::make_shared<yoyo::bit_node>();
  EXPECT_EQ(object->type(), "bit");
  EXPECT_FALSE(object->identifier().is_nil());
  EXPECT_EQ(object->name(), "");

  auto meta = object->metaObject();
  EXPECT_GE(meta->indexOfProperty("name"), yoyo::node_base::staticMetaObject.propertyOffset());
  EXPECT_GE(meta->indexOfProperty("valueType"), yoyo::data_node::staticMetaObject.propertyOffset());
  EXPECT_GE(meta->indexOfProperty("title"),
            yoyo::data_value_node::staticMetaObject.propertyOffset());
  EXPECT_GE(meta->indexOfProperty("unit"),
            yoyo::data_value_node::staticMetaObject.propertyOffset());
  EXPECT_GE(meta->indexOfProperty("access"),
            yoyo::data_value_node::staticMetaObject.propertyOffset());
  EXPECT_GE(meta->indexOfProperty("onChange"),
            yoyo::data_value_node::staticMetaObject.propertyOffset());

  EXPECT_EQ(object->valueType(), yoyo::types::value_t::BIT);
  EXPECT_FALSE(object->unit()._visible);

  EXPECT_TRUE(object->canConnect());
}

TEST(BitNodeTest, values)
{
  auto object = std::make_shared<yoyo::bit_node>();

  struct Receiver {
    MOCK_METHOD(void, valueChanged, (QVariant v));
    MOCK_METHOD(void, scriptExecutionRequested, (yoyo::properties::script_t s));
  } receiver;

  auto connection = object->connectToValue([&receiver](auto v) { receiver.valueChanged(v); });
  QObject::connect(object.get(), &yoyo::node_base::scriptExecutionRequested,
                   [&receiver](auto s) { receiver.scriptExecutionRequested(s); });

  // valid values
  EXPECT_CALL(receiver, valueChanged(QVariant(true)));
  EXPECT_CALL(receiver, scriptExecutionRequested(_)).Times(0);
  object->setValue(true);
  ASSERT_EQ(object->value().index(), 0);
  EXPECT_EQ(std::get<bool>(object->value()), true);

  yoyo::properties::script_t script { "foo", yoyo::properties::script_t::type::NATIVE };
  object->setOnChange(script);

  EXPECT_CALL(receiver, valueChanged(QVariant(false)));
  EXPECT_CALL(receiver, scriptExecutionRequested(script));
  object->setValue(false);
  ASSERT_EQ(object->value().index(), 0);
  EXPECT_EQ(std::get<bool>(object->value()), false);

  // unchanged value
  EXPECT_CALL(receiver, valueChanged(_)).Times(0);
  EXPECT_CALL(receiver, scriptExecutionRequested(_)).Times(0);
  object->setValue(false);
  ASSERT_EQ(object->value().index(), 0);
  EXPECT_EQ(std::get<bool>(object->value()), false);

  // invalid values
  EXPECT_CALL(receiver, valueChanged(_)).Times(0);
  EXPECT_CALL(receiver, scriptExecutionRequested(_)).Times(0);
  object->setValue(QVariant {});
  ASSERT_EQ(object->value().index(), 0);
  EXPECT_EQ(std::get<bool>(object->value()), false);

  EXPECT_CALL(receiver, valueChanged(_)).Times(0);
  EXPECT_CALL(receiver, scriptExecutionRequested(_)).Times(0);
  object->setValue("foo/bar");
  ASSERT_EQ(object->value().index(), 0);
  EXPECT_EQ(std::get<bool>(object->value()), false);
}
