#include "check_box.hpp"

#include "guiFixture.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QApplication>
#include <QCheckBox>
#include <QSignalSpy>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <memory>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;
using testing::WithArg;

class checkBoxTest : public yoyo::test::guiFixture
{
};

TEST_F(checkBoxTest, icon)
{
  auto id = boost::uuids::to_string(yoyo::check_box::typeId());
  EXPECT_TRUE(QFile::exists(":/yoyo/nodes/" + QString::fromStdString(id) + ".png"));
}

TEST_F(checkBoxTest, properties)
{
  auto object = std::make_shared<yoyo::check_box>();
  object->init();
  EXPECT_EQ(object->type(), "check_box");
  EXPECT_FALSE(object->identifier().is_nil());
  EXPECT_EQ(object->name(), "");
  EXPECT_EQ(object->layoutDirection()._s, yoyo::types::layout_direction_t::HORIZONTAL);
  EXPECT_EQ(object->layoutDirection()._visible, false);
  EXPECT_EQ(object->styleSheet(), yoyo::properties::text_t { "" });
  EXPECT_EQ(object->xPosition(), 0);
  EXPECT_EQ(object->yPosition(), 0);
  EXPECT_EQ(object->width(), 0);
  EXPECT_EQ(object->height(), 0);
  EXPECT_EQ(object->title()._own_value, "");
  EXPECT_EQ(object->title()._connected_value, "");
  EXPECT_EQ(object->connection(), yoyo::properties::connection_t {});
  EXPECT_GE(object->metaObject()->indexOfProperty("name"),
            yoyo::node_base::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("layoutDirection"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("styleSheet"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("xPosition"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("yPosition"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("width"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("height"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("enabled"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("visible"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("title"),
            yoyo::check_box::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("connection"),
            yoyo::check_box::staticMetaObject.propertyOffset());
  ASSERT_NE(object->widget(), nullptr);

  struct Receiver {
    MOCK_METHOD(void, onTitleChanged, (yoyo::properties::connected_string_t title));
    MOCK_METHOD(void, onConnectionChanged, (yoyo::properties::connection_t s));
  } receiver;

  QObject::connect(object.get(), &yoyo::check_box::titleChanged,
                   [&receiver](auto v) { receiver.onTitleChanged(v); });
  QObject::connect(object.get(), &yoyo::check_box::connectionChanged,
                   [&receiver](auto s) { receiver.onConnectionChanged(s); });

  {
    EXPECT_CALL(receiver, onTitleChanged(yoyo::properties::connected_string_t { "", "foo" }));
    object->setTitle(yoyo::properties::connected_string_t { "", "foo" });
    EXPECT_EQ(object->title()._own_value, "foo");
  }
  {
    using yoyo::properties::connection_t;
    EXPECT_CALL(receiver, onConnectionChanged(connection_t { {}, {}, {}, "", "", "foo" }));
    object->setConnection(connection_t { {}, {}, {}, "", "", "foo" });
    EXPECT_EQ(object->connection()._in, "");
    EXPECT_EQ(object->connection()._out, "");
    EXPECT_EQ(object->connection()._auto, "foo");

    EXPECT_CALL(receiver, onConnectionChanged(connection_t { {}, {}, {}, "foo", "bar", "" }));
    object->setConnection(connection_t { {}, {}, {}, "foo", "bar", "" });
    EXPECT_EQ(object->connection()._in, "foo");
    EXPECT_EQ(object->connection()._out, "bar");
    EXPECT_EQ(object->connection()._auto, "");
  }

  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::BIT));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::BITFIELD));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::STRING));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::FLOAT));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::UINT8));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::UINT16));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::UINT32));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::UINT64));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::INT8));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::INT16));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::INT32));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::INT64));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::REGISTER));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::DEVICE));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::CUSTOM));

  auto connection = object->connection();
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::BIT));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::BITFIELD));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::STRING));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::FLOAT));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::UINT8));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::UINT16));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::UINT32));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::UINT64));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::INT8));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::INT16));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::INT32));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::INT64));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::REGISTER));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::DEVICE));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::CUSTOM));
}

TEST_F(checkBoxTest, checkbox)
{
  auto object = std::make_shared<yoyo::check_box>();
  object->init();
  object->widget()->resize(30, 30);
  object->widget()->show();

  ASSERT_EQ(object->widget()->children().count(), 2);
  auto widget = object->widget()->children().at(1);
  ASSERT_NE(widget, nullptr);
  EXPECT_EQ(widget->metaObject()->className(), std::string { "QCheckBox" });
  auto checkbox = dynamic_cast<QCheckBox*>(widget);
  ASSERT_NE(checkbox, nullptr);
  EXPECT_EQ(checkbox->text(), "");
  EXPECT_TRUE(checkbox->isCheckable());

  object->setTitle(yoyo::properties::connected_string_t { "", "foo" });
  EXPECT_EQ(checkbox->text(), "foo");
  object->setTitle(yoyo::properties::connected_string_t { "bar", "" });
  EXPECT_EQ(checkbox->text(), "bar");
  object->setTitle(yoyo::properties::connected_string_t { "bar", "foo" });
  EXPECT_EQ(checkbox->text(), "foo");

  struct Receiver {
    MOCK_METHOD(void, setValue, (QVariant s));
  } receiver;

  object->connection()._setter(false);
  EXPECT_FALSE(checkbox->isChecked());

  auto connect = object->connection()._connector([&receiver](auto v) { receiver.setValue(v); });

  EXPECT_EQ(checkbox->isChecked(), false);
  EXPECT_CALL(receiver, setValue(QVariant(true)));
  //    QTest::mouseClick(checkbox, Qt::LeftButton);
  checkbox->setChecked(true);
  QApplication::processEvents();
  EXPECT_TRUE(checkbox->isChecked());

  EXPECT_CALL(receiver, setValue(QVariant(false)));
  //    QTest::mouseClick(checkbox, Qt::LeftButton);
  checkbox->setChecked(false);
  QApplication::processEvents();
  EXPECT_FALSE(checkbox->isChecked());
}
