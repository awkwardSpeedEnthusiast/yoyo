#include "combo_box.hpp"

#include "guiFixture.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QApplication>
#include <QComboBox>
#include <QLabel>
#include <QSignalSpy>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <memory>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;
using testing::WithArg;

class comboBoxTest : public yoyo::test::guiFixture
{
};

TEST_F(comboBoxTest, icon)
{
  auto id = boost::uuids::to_string(yoyo::combo_box::typeId());
  EXPECT_TRUE(QFile::exists(":/yoyo/nodes/" + QString::fromStdString(id) + ".png"));
}

TEST_F(comboBoxTest, properties)
{
  auto object = std::make_shared<yoyo::combo_box>();
  object->init();
  EXPECT_EQ(object->type(), "combo_box");
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
            yoyo::combo_box::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("unit"),
            yoyo::combo_box::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("connection"),
            yoyo::combo_box::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("enumValues"),
            yoyo::combo_box::staticMetaObject.propertyOffset());
  ASSERT_NE(object->widget(), nullptr);

  struct Receiver {
    MOCK_METHOD(void, onTitleChanged, (yoyo::properties::connected_string_t title));
    MOCK_METHOD(void, onUnitChanged, (yoyo::properties::connected_string_t unit));
    MOCK_METHOD(void, onConnectionChanged, (yoyo::properties::connection_t s));
    MOCK_METHOD(void, onEnumValuesChanged, (yoyo::properties::enum_t v));
  } receiver;

  QObject::connect(object.get(), &yoyo::combo_box::titleChanged,
                   [&receiver](auto v) { receiver.onTitleChanged(v); });
  QObject::connect(object.get(), &yoyo::combo_box::unitChanged,
                   [&receiver](auto v) { receiver.onUnitChanged(v); });
  QObject::connect(object.get(), &yoyo::combo_box::connectionChanged,
                   [&receiver](auto s) { receiver.onConnectionChanged(s); });
  QObject::connect(object.get(), &yoyo::combo_box::enumValuesChanged,
                   [&receiver](auto s) { receiver.onEnumValuesChanged(s); });

  {
    EXPECT_CALL(receiver, onTitleChanged(yoyo::properties::connected_string_t { "", "foo" }));
    object->setTitle(yoyo::properties::connected_string_t { "", "foo" });
    EXPECT_EQ(object->title()._own_value, "foo");

    EXPECT_CALL(receiver, onTitleChanged(_)).Times(0);
    object->setTitle(yoyo::properties::connected_string_t { "", "foo" });
  }
  {
    EXPECT_CALL(receiver, onUnitChanged(yoyo::properties::connected_string_t { "", "bar" }));
    object->setUnit(yoyo::properties::connected_string_t { "", "bar" });
    EXPECT_EQ(object->unit()._own_value, "bar");

    EXPECT_CALL(receiver, onUnitChanged(_)).Times(0);
    object->setUnit(yoyo::properties::connected_string_t { "", "bar" });
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

    EXPECT_CALL(receiver, onConnectionChanged(_)).Times(0);
    object->setConnection(connection_t { {}, {}, {}, "foo", "bar", "" });
  }
  {
    yoyo::properties::enum_t values { true, {} };
    EXPECT_CALL(receiver, onEnumValuesChanged(values));
    object->setEnumValues(values);
    EXPECT_EQ(object->enumValues(), values);

    values._values[0] =
      std::make_tuple(QString("foo"), QString("bar"), yoyo::properties::script_t {});
    EXPECT_CALL(receiver, onEnumValuesChanged(values));
    object->setEnumValues(values);
    EXPECT_EQ(object->enumValues(), values);

    EXPECT_CALL(receiver, onEnumValuesChanged(_)).Times(0);
    object->setEnumValues(values);
  }

  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::BIT));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::BITFIELD));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::STRING));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::FLOAT));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::UINT8));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::UINT16));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::UINT32));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::UINT64));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::INT8));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::INT16));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::INT32));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::INT64));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::REGISTER));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::DEVICE));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::CUSTOM));

  auto connection = object->connection();
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::BIT));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::BITFIELD));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::STRING));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::FLOAT));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::UINT8));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::UINT16));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::UINT32));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::UINT64));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::INT8));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::INT16));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::INT32));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::INT64));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::REGISTER));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::DEVICE));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::CUSTOM));
}

TEST_F(comboBoxTest, combobox)
{
  auto object = std::make_shared<yoyo::combo_box>();
  object->init();
  object->widget()->resize(30, 30);
  object->widget()->show();

  ASSERT_EQ(object->widget()->children().count(), 4);
  auto widget1 = object->widget()->children().at(1);
  ASSERT_NE(widget1, nullptr);
  EXPECT_EQ(widget1->metaObject()->className(), std::string { "QLabel" });
  auto title_label = dynamic_cast<QLabel*>(widget1);
  ASSERT_NE(title_label, nullptr);

  auto widget2 = object->widget()->children().at(2);
  ASSERT_NE(widget2, nullptr);
  EXPECT_EQ(widget2->metaObject()->className(), std::string { "QComboBox" });
  auto combobox = dynamic_cast<QComboBox*>(widget2);
  ASSERT_NE(combobox, nullptr);

  auto widget3 = object->widget()->children().at(3);
  ASSERT_NE(widget3, nullptr);
  EXPECT_EQ(widget3->metaObject()->className(), std::string { "QLabel" });
  auto unit_label = dynamic_cast<QLabel*>(widget3);
  ASSERT_NE(unit_label, nullptr);

  EXPECT_EQ(title_label->text(), "");
  EXPECT_FALSE(title_label->isVisible());
  object->setTitle(yoyo::properties::connected_string_t { "", "foo" });
  EXPECT_EQ(title_label->text(), "foo");
  EXPECT_TRUE(title_label->isVisible());
  object->setTitle(yoyo::properties::connected_string_t { "bar", "" });
  EXPECT_EQ(title_label->text(), "bar");
  object->setTitle(yoyo::properties::connected_string_t { "bar", "foo" });
  EXPECT_EQ(title_label->text(), "foo");
  object->setTitle(yoyo::properties::connected_string_t { "", "" });
  EXPECT_EQ(title_label->text(), "");
  EXPECT_FALSE(title_label->isVisible());

  EXPECT_EQ(unit_label->text(), "");
  EXPECT_FALSE(unit_label->isVisible());
  object->setUnit(yoyo::properties::connected_string_t { "", "foo" });
  EXPECT_EQ(unit_label->text(), "foo");
  EXPECT_TRUE(unit_label->isVisible());
  object->setUnit(yoyo::properties::connected_string_t { "bar", "" });
  EXPECT_EQ(unit_label->text(), "bar");
  object->setUnit(yoyo::properties::connected_string_t { "bar", "foo" });
  EXPECT_EQ(unit_label->text(), "foo");
  object->setUnit(yoyo::properties::connected_string_t { "", "" });
  EXPECT_EQ(unit_label->text(), "");
  EXPECT_FALSE(unit_label->isVisible());

  yoyo::properties::enum_t values;
  EXPECT_EQ(combobox->count(), 0);

  values._values[0] =
    std::make_tuple(QString("foo"), QString("bar"), yoyo::properties::script_t {});
  object->setEnumValues(values);
  EXPECT_EQ(combobox->count(), 1);

  values._values[3] =
    std::make_tuple(QString("hello"), QString("world"), yoyo::properties::script_t {});
  object->setEnumValues(values);
  EXPECT_EQ(combobox->count(), 2);

  EXPECT_EQ(combobox->itemText(0), "foo");
  EXPECT_EQ(combobox->itemText(1), "hello");

  struct Receiver {
    MOCK_METHOD(void, setValue, (int v));
  } receiver;

  object->connection()._setter(0);
  EXPECT_EQ(combobox->currentIndex(), 0);
  EXPECT_EQ(combobox->currentText(), "foo");
  object->connection()._setter(3);
  EXPECT_EQ(combobox->currentIndex(), 1);
  EXPECT_EQ(combobox->currentText(), "hello");
  object->connection()._setter(1);
  EXPECT_EQ(combobox->currentIndex(), -1);
  EXPECT_EQ(combobox->currentText(), "");

  auto connect =
    object->connection()._connector([&receiver](auto v) { receiver.setValue(v.toInt()); });

  EXPECT_CALL(receiver, setValue(0));
  combobox->setCurrentIndex(0);
  QApplication::processEvents();

  EXPECT_CALL(receiver, setValue(3));
  combobox->setCurrentIndex(1);
  QApplication::processEvents();
}
