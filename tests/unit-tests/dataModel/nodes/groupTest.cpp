#include "group.hpp"
#include "button.hpp"
#include "line_edit.hpp"

#include "guiFixture.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QApplication>
#include <QGroupBox>
#include <QSignalSpy>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <memory>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;
using testing::WithArg;

class groupTest : public yoyo::test::guiFixture
{
};

TEST_F(groupTest, icon)
{
  auto id = boost::uuids::to_string(yoyo::group::typeId());
  EXPECT_TRUE(QFile::exists(":/yoyo/nodes/" + QString::fromStdString(id) + ".png"));
}

TEST_F(groupTest, properties)
{
  auto object = std::make_shared<yoyo::group>();
  object->init();

  EXPECT_TRUE(object->acceptsChildren());
  auto child1 = std::make_shared<yoyo::line_edit>();
  child1->init();

  EXPECT_EQ(object->type(), "group");
  EXPECT_FALSE(object->identifier().is_nil());
  EXPECT_EQ(object->name()._s, "");

  EXPECT_TRUE(object->acceptsChild(child1));
  EXPECT_TRUE(object->acceptsParent(child1));
  EXPECT_EQ(object->layoutDirection()._s, yoyo::types::layout_direction_t::VERTICAL);
  EXPECT_EQ(object->layoutDirection()._visible, true);
  EXPECT_EQ(object->styleSheet(), yoyo::properties::text_t { "" });
  EXPECT_EQ(object->xPosition(), 0);
  EXPECT_EQ(object->yPosition(), 0);
  EXPECT_EQ(object->width(), 0);
  EXPECT_EQ(object->height(), 0);
  EXPECT_EQ(object->title(), "");
  EXPECT_EQ(object->title(), "");
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
            yoyo::group::staticMetaObject.propertyOffset());
  ASSERT_NE(object->widget(), nullptr);
  struct Receiver {
    MOCK_METHOD(void, onTitleChanged, (QString title));
  } receiver;

  QObject::connect(object.get(), &yoyo::group::titleChanged,
                   [&receiver](auto v) { receiver.onTitleChanged(v); });

  {
    EXPECT_CALL(receiver, onTitleChanged(QString { "foo" }));
    object->setTitle("foo");
    EXPECT_EQ(object->title(), "foo");

    EXPECT_CALL(receiver, onTitleChanged(_)).Times(0);
    object->setTitle("foo");
  }
  EXPECT_FALSE(object->supportsValueType(yoyo::types::value_t::BIT));
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
}

TEST_F(groupTest, group)
{
  auto object = std::make_shared<yoyo::group>();
  object->init();
  EXPECT_TRUE(object->acceptsChildren());

  auto child1 = std::make_shared<yoyo::button>();
  child1->init();

  auto child2 = std::make_shared<yoyo::line_edit>();
  child2->init();

  auto child3 = std::make_shared<yoyo::group>();
  child3->init();

  auto group_widget = object->widget();
  ASSERT_NE(group_widget, nullptr);
  group_widget->resize(45, 45);
  group_widget->show();
  ASSERT_EQ(group_widget->children().count(), 2);
  ASSERT_NE(dynamic_cast<QGroupBox*>(group_widget->children()[0]), nullptr);
  auto groupbox = dynamic_cast<QGroupBox*>(group_widget->children()[0]);

  EXPECT_EQ(groupbox->title(), "");
  object->setTitle("foo");
  EXPECT_EQ(groupbox->title(), "foo");

  // first child is layout
  EXPECT_EQ(groupbox->children().count(), 1);

  object->addChild(child1);
  QApplication::processEvents();
  ASSERT_EQ(groupbox->children().count(), 2);
  EXPECT_EQ(groupbox->children()[1], child1->widget());

  object->addChild(child2);
  QApplication::processEvents();
  ASSERT_EQ(groupbox->children().count(), 3);
  EXPECT_EQ(groupbox->children()[2], child2->widget());

  EXPECT_EQ(group_widget->childAt(3, 20)->parent(), child1->widget());
  EXPECT_EQ(group_widget->childAt(3, 45)->parent(), child2->widget());
}
