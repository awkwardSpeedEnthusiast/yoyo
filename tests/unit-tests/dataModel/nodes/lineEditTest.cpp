#include "line_edit.hpp"

#include "guiFixture.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QSignalSpy>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <memory>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;
using testing::WithArg;

class lineEditTest : public yoyo::test::guiFixture
{
};

TEST_F(lineEditTest, icon)
{
  auto id = boost::uuids::to_string(yoyo::line_edit::typeId());
  EXPECT_TRUE(QFile::exists(":/yoyo/nodes/" + QString::fromStdString(id) + ".png"));
}

TEST_F(lineEditTest, properties)
{
  auto object = std::make_shared<yoyo::line_edit>();
  object->init();
  EXPECT_EQ(object->type(), "line_edit");
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
            yoyo::line_edit::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("unit"),
            yoyo::line_edit::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("connection"),
            yoyo::line_edit::staticMetaObject.propertyOffset());
  EXPECT_GE(object->metaObject()->indexOfProperty("captureKey"),
            yoyo::line_edit::staticMetaObject.propertyOffset());
  ASSERT_NE(object->widget(), nullptr);

  struct Receiver {
    MOCK_METHOD(void, onTitleChanged, (yoyo::properties::connected_string_t title));
    MOCK_METHOD(void, onUnitChanged, (yoyo::properties::connected_string_t unit));
    MOCK_METHOD(void, onConnectionChanged, (yoyo::properties::connection_t s));
    MOCK_METHOD(void, onCaptureKeyChanged, (bool v));
  } receiver;

  QObject::connect(object.get(), &yoyo::line_edit::titleChanged,
                   [&receiver](auto v) { receiver.onTitleChanged(v); });
  QObject::connect(object.get(), &yoyo::line_edit::unitChanged,
                   [&receiver](auto v) { receiver.onUnitChanged(v); });
  QObject::connect(object.get(), &yoyo::line_edit::connectionChanged,
                   [&receiver](auto s) { receiver.onConnectionChanged(s); });
  QObject::connect(object.get(), &yoyo::line_edit::captureKeyChanged,
                   [&receiver](auto s) { receiver.onCaptureKeyChanged(s); });

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
    EXPECT_CALL(receiver, onCaptureKeyChanged(true));
    object->setCaptureKey(true);
    EXPECT_EQ(object->captureKey(), true);

    EXPECT_CALL(receiver, onCaptureKeyChanged(false));
    object->setCaptureKey(false);
    EXPECT_EQ(object->captureKey(), false);

    EXPECT_CALL(receiver, onCaptureKeyChanged(_)).Times(0);
    object->setCaptureKey(false);
  }

  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::BIT));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::BITFIELD));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::STRING));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::FLOAT));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::UINT8));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::UINT16));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::UINT32));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::UINT64));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::INT8));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::INT16));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::INT32));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::INT64));
  EXPECT_TRUE(object->supportsValueType(yoyo::types::value_t::REGISTER));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::DEVICE));
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::CUSTOM));

  auto connection = object->connection();
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::BIT));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::BITFIELD));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::STRING));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::FLOAT));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::UINT8));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::UINT16));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::UINT32));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::UINT64));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::INT8));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::INT16));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::INT32));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::INT64));
  EXPECT_TRUE(connection._supports(yoyo::types::value_t::REGISTER));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::DEVICE));
  EXPECT_FALSE(connection._supports(yoyo::types::value_t::CUSTOM));
}

TEST_F(lineEditTest, lineedit)
{
  auto object = std::make_shared<yoyo::line_edit>();
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
  EXPECT_EQ(widget2->metaObject()->className(), std::string { "QLineEdit" });
  auto lineedit = dynamic_cast<QLineEdit*>(widget2);
  ASSERT_NE(lineedit, nullptr);

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

  struct Receiver {
    MOCK_METHOD(void, setValue, (std::string s));
  } receiver;

  object->connection()._setter("foo-bar");
  EXPECT_EQ(lineedit->text(), "foo-bar");
  object->connection()._setter("");
  EXPECT_EQ(lineedit->text(), "");

  auto connect = object->connection()._connector(
    [&receiver](auto v) { receiver.setValue(v.toString().toStdString()); });

  EXPECT_FALSE(object->captureKey());

  QTest::keyClicks(lineedit, "1234.6");
  QApplication::processEvents();
  EXPECT_CALL(receiver, setValue("1234.6"));
  QTest::keyClick(lineedit, Qt::Key_Return);
  QApplication::processEvents();

  QTest::keyClicks(lineedit, "lorem ipsum");
  QApplication::processEvents();
  EXPECT_CALL(receiver, setValue("1234.6lorem ipsum"));
  QTest::keyClick(lineedit, Qt::Key_Return);
  QApplication::processEvents();

  object->setCaptureKey(true);

  testing::Sequence seq;
  EXPECT_CALL(receiver, setValue("1")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("2")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("3")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("4")).InSequence(seq);
  EXPECT_CALL(receiver, setValue(".")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("6")).InSequence(seq);
  QTest::keyClicks(lineedit, "1234.6");
  QApplication::processEvents();

  EXPECT_CALL(receiver, setValue("l")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("o")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("r")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("e")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("m")).InSequence(seq);
  EXPECT_CALL(receiver, setValue(" ")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("i")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("p")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("s")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("u")).InSequence(seq);
  EXPECT_CALL(receiver, setValue("m")).InSequence(seq);
  QTest::keyClicks(lineedit, "lorem ipsum");
  QApplication::processEvents();
}
