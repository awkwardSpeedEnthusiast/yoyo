#include "yoyo/gui_node.h"

#include "guiFixture.hpp"
#include "gui_node_mock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QGroupBox>
#include <QSignalSpy>
#include <QWidget>

#include <boost/uuid/random_generator.hpp>

#include <memory>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;
using testing::WithArg;
using yoyo::test::gui_node_mock;

class guiNodeTest : public yoyo::test::guiFixture
{
public:
  auto expectAdd(std::shared_ptr<gui_node_mock> const& parent,
                 std::shared_ptr<gui_node_mock> const& child) -> void
  {
    EXPECT_CALL(*parent, acceptsChild(std::dynamic_pointer_cast<yoyo::node_base>(child)))
      .WillOnce(Return(true));
    EXPECT_CALL(*child, acceptsParent(std::dynamic_pointer_cast<yoyo::node_base>(parent)))
      .WillOnce(Return(true));
    EXPECT_CALL(*parent, beforeChildAdd(std::dynamic_pointer_cast<yoyo::node_base>(child)))
      .WillOnce(Return(true));
    EXPECT_CALL(*parent, afterChildAdd(std::dynamic_pointer_cast<yoyo::node_base>(child)))
      .WillOnce(Return(true));
  }
};

TEST_F(guiNodeTest, properties)
{
  auto object = std::make_shared<testing::StrictMock<gui_node_mock>>(QString("mock_node"), nullptr,
                                                                     boost::uuids::uuid {});
  object->init();
  EXPECT_CALL(*object, destructorCalled());

  EXPECT_EQ(object->type(), "mock_node");
  EXPECT_FALSE(object->identifier().is_nil());
  EXPECT_EQ(object->name(), "");
  EXPECT_EQ(object->layoutDirection()._s, yoyo::types::layout_direction_t::HORIZONTAL);
  EXPECT_EQ(object->layoutDirection()._visible, false);
  EXPECT_EQ(object->styleSheet(), yoyo::properties::text_t { "" });
  EXPECT_EQ(object->xPosition(), 0);
  EXPECT_EQ(object->yPosition(), 0);
  EXPECT_EQ(object->width(), 0);
  EXPECT_EQ(object->height(), 0);
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
  ASSERT_NE(object->widget(), nullptr);

  struct Receiver {
    MOCK_METHOD(void, onNameChanged, (QString name));
    MOCK_METHOD(void, onLayoutDirectionChanged,
                (yoyo::properties::invisible_layout_direction_t ld));
    MOCK_METHOD(void, onStyleSheetChanged, (yoyo::properties::text_t name));
    MOCK_METHOD(void, onXChanged, (int x));
    MOCK_METHOD(void, onYChanged, (int x));
    MOCK_METHOD(void, onWidthChanged, (int x));
    MOCK_METHOD(void, onHeightChanged, (int x));
    MOCK_METHOD(void, onEnabledChanged, (yoyo::properties::connected_boolean_t e));
    MOCK_METHOD(void, onVisibleChanged, (bool v));
  } receiver;

  QObject::connect(object.get(), &yoyo::node_base::nameChanged,
                   [&receiver](auto v) { receiver.onNameChanged(v); });
  QObject::connect(object.get(), &yoyo::gui_node::layoutDirectionChanged,
                   [&receiver](auto v) { receiver.onLayoutDirectionChanged(v); });
  QObject::connect(object.get(), &yoyo::gui_node::styleSheetChanged,
                   [&receiver](auto v) { receiver.onStyleSheetChanged(v); });
  QObject::connect(object.get(), &yoyo::gui_node::xPositionChanged,
                   [&receiver](auto v) { receiver.onXChanged(v); });
  QObject::connect(object.get(), &yoyo::gui_node::yPositionChanged,
                   [&receiver](auto v) { receiver.onYChanged(v); });
  QObject::connect(object.get(), &yoyo::gui_node::widthChanged,
                   [&receiver](auto v) { receiver.onWidthChanged(v); });
  QObject::connect(object.get(), &yoyo::gui_node::heightChanged,
                   [&receiver](auto v) { receiver.onHeightChanged(v); });
  QObject::connect(object.get(), &yoyo::gui_node::enabledChanged,
                   [&receiver](auto v) { receiver.onEnabledChanged(v); });
  QObject::connect(object.get(), &yoyo::gui_node::visibleChanged,
                   [&receiver](auto v) { receiver.onVisibleChanged(v); });

  {
    EXPECT_CALL(receiver, onNameChanged(QString("foo")));
    object->setName("foo");
    EXPECT_EQ(object->name(), "foo");
  }
  {
    using ld = yoyo::types::layout_direction_t;
    EXPECT_NE(object->widget()->layout(), nullptr);
    EXPECT_CALL(receiver, onLayoutDirectionChanged(yoyo::properties::invisible_layout_direction_t {
                            ld::HORIZONTAL, true }));
    object->setLayoutDirection({ ld::HORIZONTAL, true });
    EXPECT_EQ(object->layoutDirection()._s, ld::HORIZONTAL);
    EXPECT_EQ(object->layoutDirection()._visible, true);
    ASSERT_NE(object->widget()->layout(), nullptr);
    ASSERT_EQ(std::string(object->widget()->layout()->metaObject()->className()), "QBoxLayout");
    EXPECT_EQ(dynamic_cast<QBoxLayout*>(object->widget()->layout())->direction(),
              QBoxLayout::LeftToRight);

    EXPECT_CALL(receiver, onLayoutDirectionChanged(yoyo::properties::invisible_layout_direction_t {
                            ld::HORIZONTAL, false }));
    object->setLayoutDirection({ ld::HORIZONTAL, false });
    EXPECT_EQ(object->layoutDirection()._s, ld::HORIZONTAL);
    EXPECT_EQ(object->layoutDirection()._visible, false);
  }
  {
    yoyo::properties::text_t expected { "QWidget {background-color: green}" };
    EXPECT_CALL(receiver, onStyleSheetChanged(expected));
    object->setStyleSheet(expected);
    EXPECT_EQ(object->styleSheet(), expected);
    EXPECT_EQ(object->widget()->styleSheet(), expected._s);
  }
  {
    EXPECT_CALL(receiver, onXChanged(42));
    object->setXposition(42);
    EXPECT_EQ(object->xPosition(), 42);
    EXPECT_EQ(object->widget()->x(), 42);
  }
  {
    EXPECT_CALL(receiver, onYChanged(5));
    object->setYposition(5);
    EXPECT_EQ(object->yPosition(), 5);
    EXPECT_EQ(object->widget()->y(), 5);
  }
  {
    EXPECT_CALL(receiver, onWidthChanged(400));
    object->setWidth(400);
    EXPECT_EQ(object->width(), 400);
    EXPECT_EQ(object->widget()->width(), 400);
  }
  {
    EXPECT_CALL(receiver, onHeightChanged(200));
    object->setHeight(200);
    EXPECT_EQ(object->height(), 200);
    EXPECT_EQ(object->widget()->height(), 200);
  }
  {
    using cb = yoyo::properties::connected_boolean_t;
    EXPECT_CALL(receiver, onEnabledChanged(cb { cb::boolean_t::FALSE, true }));
    object->setEnabled(cb { cb::boolean_t::FALSE, true });
    EXPECT_EQ(object->enabled()._connected_value, true);
    EXPECT_EQ(object->widget()->isEnabled(), false);

    EXPECT_CALL(receiver, onEnabledChanged(cb { cb::boolean_t::TRUE, false }));
    object->setEnabled(cb { cb::boolean_t::TRUE, false });
    EXPECT_EQ(object->enabled()._connected_value, false);
    EXPECT_EQ(object->widget()->isEnabled(), true);

    EXPECT_CALL(receiver, onEnabledChanged(cb { cb::boolean_t::DEFINED_BY_CONNECTION, false }));
    object->setEnabled(cb { cb::boolean_t::DEFINED_BY_CONNECTION, false });
    EXPECT_EQ(object->enabled()._connected_value, false);
    EXPECT_EQ(object->widget()->isEnabled(), false);

    EXPECT_CALL(receiver, onEnabledChanged(cb { cb::boolean_t::DEFINED_BY_CONNECTION, true }));
    object->setEnabled(cb { cb::boolean_t::DEFINED_BY_CONNECTION, true });
    EXPECT_EQ(object->enabled()._connected_value, true);
    EXPECT_EQ(object->widget()->isEnabled(), true);
  }
  {
    EXPECT_CALL(receiver, onVisibleChanged(false));
    object->setVisible(false);
    EXPECT_EQ(object->visible(), false);
    EXPECT_EQ(object->widget()->isVisible(), false);

    EXPECT_CALL(receiver, onVisibleChanged(true));
    object->setVisible(true);
    EXPECT_EQ(object->visible(), true);
    EXPECT_EQ(object->widget()->isVisible(), true);
  }
}

TEST_F(guiNodeTest, hierarchy)
{
  auto root = std::make_shared<testing::StrictMock<gui_node_mock>>(QString("mock_node_root"),
                                                                   nullptr, boost::uuids::uuid {});
  root->init();
  auto child = std::make_shared<testing::StrictMock<gui_node_mock>>(QString("mock_node_child"),
                                                                    nullptr, boost::uuids::uuid {});
  child->init();
  auto object = std::make_shared<testing::StrictMock<gui_node_mock>>(
    QString("mock_node_object"), nullptr, boost::uuids::uuid {});
  object->init();

  expectAdd(root, child);
  expectAdd(child, object);

  root->addChild(child);
  child->addChild(object);

  auto root_widget = root->widget();
  auto child_widget = child->widget();
  auto object_widget = object->widget();
  ASSERT_NE(root_widget, nullptr);
  ASSERT_NE(child_widget, nullptr);
  ASSERT_NE(object_widget, nullptr);

  ASSERT_EQ(root_widget->children().count(), 2);
  EXPECT_EQ(root_widget->children()[1], child_widget);

  ASSERT_EQ(child_widget->children().count(), 2);
  EXPECT_EQ(child_widget->children()[1], object_widget);

  // Remove
  EXPECT_CALL(*root, beforeChildRemove(_)).WillOnce(Return(true));
  EXPECT_CALL(*root, afterChildRemove(_));
  root->removeChild(child);
  expectAdd(root, child);
  root->addChild(child);

  // Making sure both nodes and widgets have been cleaned up.
  testing::Mock::VerifyAndClear(root.get());
  testing::Mock::VerifyAndClear(child.get());
  testing::Mock::VerifyAndClear(object.get());
  QSignalSpy root_spy(root_widget, &QObject::destroyed);
  QSignalSpy child_spy(child_widget, &QObject::destroyed);
  QSignalSpy object_spy(object_widget, &QObject::destroyed);
  object.reset();
  child.reset();
  EXPECT_CALL(*std::dynamic_pointer_cast<gui_node_mock>(root->childAt(0)->childAt(0)),
              destructorCalled());
  EXPECT_CALL(*std::dynamic_pointer_cast<gui_node_mock>(root->childAt(0)), destructorCalled());
  EXPECT_CALL(*root, destructorCalled());
  root.reset();
  QApplication::processEvents();
  EXPECT_EQ(root_spy.count(), 1);
  EXPECT_EQ(child_spy.count(), 1);
  EXPECT_EQ(object_spy.count(), 1);
}

TEST_F(guiNodeTest, containered)
{
  auto root_container = new QGroupBox;
  auto child_container = new QGroupBox;
  auto root = std::make_shared<testing::StrictMock<gui_node_mock>>(
    QString("mock_node_root"), root_container, boost::uuids::uuid {});
  root->init();
  auto child = std::make_shared<testing::StrictMock<gui_node_mock>>(
    QString("mock_node_child"), child_container, boost::uuids::uuid {});
  child->init();
  auto object = std::make_shared<testing::StrictMock<gui_node_mock>>(
    QString("mock_node_object"), nullptr, boost::uuids::uuid {});
  object->init();
  root->setLayoutDirection({ yoyo::types::layout_direction_t::HORIZONTAL, true });

  expectAdd(root, child);
  expectAdd(child, object);

  root->addChild(child);
  child->addChild(object);

  auto root_widget = root->widget();
  auto child_widget = child->widget();
  auto object_widget = object->widget();
  ASSERT_NE(root_widget, nullptr);
  ASSERT_NE(child_widget, nullptr);
  ASSERT_NE(object_widget, nullptr);

  ASSERT_EQ(root_widget->children().count(), 2);
  EXPECT_EQ(root_widget->children()[0], root_container);
  ASSERT_EQ(root_widget->children()[0]->children().count(), 2);
  EXPECT_EQ(root_widget->children()[0]->children()[1], child_widget);
  EXPECT_EQ(root_container->layout()->count(), 1);

  ASSERT_EQ(child_widget->children().count(), 2);
  EXPECT_EQ(child_widget->children()[0], child_container);
  ASSERT_EQ(child_widget->children()[0]->children().count(), 2);
  EXPECT_EQ(child_widget->children()[0]->children()[1], object_widget);

  // Making sure both nodes and widgets have been cleaned up.
  testing::Mock::VerifyAndClear(root.get());
  testing::Mock::VerifyAndClear(child.get());
  testing::Mock::VerifyAndClear(object.get());
  QSignalSpy root_spy(root_widget, &QObject::destroyed);
  QSignalSpy child_spy(child_widget, &QObject::destroyed);
  QSignalSpy object_spy(object_widget, &QObject::destroyed);
  QSignalSpy root_container_spy(root_container, &QObject::destroyed);
  QSignalSpy child_container_spy(child_container, &QObject::destroyed);
  object.reset();
  child.reset();
  EXPECT_CALL(*std::dynamic_pointer_cast<gui_node_mock>(root->childAt(0)->childAt(0)),
              destructorCalled());
  EXPECT_CALL(*std::dynamic_pointer_cast<gui_node_mock>(root->childAt(0)), destructorCalled());
  EXPECT_CALL(*root, destructorCalled());
  root.reset();
  QApplication::processEvents();
  EXPECT_EQ(root_spy.count(), 1);
  EXPECT_EQ(child_spy.count(), 1);
  EXPECT_EQ(object_spy.count(), 1);
  EXPECT_EQ(root_container_spy.count(), 1);
  EXPECT_EQ(child_container_spy.count(), 1);
}

TEST_F(guiNodeTest, layout)
{
  // Testing here the layout:
  // - Whether it is correctly created,
  // - the direction matches expectations,
  // - whether the widgets are in the layout in the right order
  // - whether the order is correct under reordering
  //   - with and without layout
  auto root = std::make_shared<testing::StrictMock<gui_node_mock>>(QString("mock_node_root"),
                                                                   nullptr, boost::uuids::uuid {});
  root->init();
  auto child1 = std::make_shared<testing::StrictMock<gui_node_mock>>(
    QString("mock_node_child"), nullptr, boost::uuids::uuid {});
  child1->init();
  auto child2 = std::make_shared<testing::StrictMock<gui_node_mock>>(
    QString("mock_node_object"), nullptr, boost::uuids::uuid {});
  child2->init();

  using ld = yoyo::types::layout_direction_t;
  EXPECT_EQ(root->layoutDirection()._s, ld::HORIZONTAL);

  expectAdd(root, child1);
  expectAdd(root, child2);

  root->addChild(child1);
  root->addChild(child2);

  {
    EXPECT_EQ(root->layoutDirection()._s, ld::HORIZONTAL);
    ASSERT_NE(root->widget()->layout(), nullptr);
    ASSERT_EQ(std::string(root->widget()->layout()->metaObject()->className()), "QBoxLayout");
    EXPECT_EQ(dynamic_cast<QBoxLayout*>(root->widget()->layout())->direction(),
              QBoxLayout::LeftToRight);
    auto* layout = dynamic_cast<QBoxLayout*>(root->widget()->layout());
    EXPECT_EQ(layout->count(), 2);
    EXPECT_EQ(layout->itemAt(0)->widget(), child1->widget());
    EXPECT_EQ(layout->itemAt(1)->widget(), child2->widget());
  }

  {
    root->setLayoutDirection({ ld::FREE, true });
    EXPECT_EQ(root->layoutDirection()._s, ld::FREE);
    EXPECT_EQ(root->layoutDirection()._visible, true);
    ASSERT_EQ(root->widget()->layout(), nullptr);
  }

  EXPECT_CALL(*root, afterChildMoved(_, _, _));
  root->moveChild(child2, 0);
  {
    root->setLayoutDirection({ ld::HORIZONTAL, true });
    EXPECT_EQ(root->layoutDirection()._s, ld::HORIZONTAL);
    EXPECT_EQ(root->layoutDirection()._visible, true);
    ASSERT_NE(root->widget()->layout(), nullptr);
    ASSERT_EQ(std::string(root->widget()->layout()->metaObject()->className()), "QBoxLayout");

    auto* layout = dynamic_cast<QBoxLayout*>(root->widget()->layout());

    EXPECT_EQ(layout->direction(), QBoxLayout::LeftToRight);
    EXPECT_EQ(layout->count(), 2);
    EXPECT_EQ(layout->itemAt(0)->widget(),
              std::dynamic_pointer_cast<yoyo::gui_node>(root->childAt(0))->widget());
    EXPECT_EQ(layout->itemAt(1)->widget(),
              std::dynamic_pointer_cast<yoyo::gui_node>(root->childAt(1))->widget());
  }
  {
    root->setLayoutDirection({ ld::VERTICAL, true });
    EXPECT_EQ(root->layoutDirection()._s, ld::VERTICAL);
    EXPECT_EQ(root->layoutDirection()._visible, true);
    ASSERT_NE(root->widget()->layout(), nullptr);
    ASSERT_EQ(std::string(root->widget()->layout()->metaObject()->className()), "QBoxLayout");

    auto* layout = dynamic_cast<QBoxLayout*>(root->widget()->layout());

    EXPECT_EQ(layout->direction(), QBoxLayout::TopToBottom);
    EXPECT_EQ(layout->count(), 2);
    EXPECT_EQ(layout->itemAt(0)->widget(),
              std::dynamic_pointer_cast<yoyo::gui_node>(root->childAt(0))->widget());
    EXPECT_EQ(layout->itemAt(1)->widget(),
              std::dynamic_pointer_cast<yoyo::gui_node>(root->childAt(1))->widget());
  }
  {
    EXPECT_CALL(*root, afterChildMoved(_, _, _));
    root->moveChild(child1, 0);
    auto* layout = dynamic_cast<QBoxLayout*>(root->widget()->layout());
    EXPECT_EQ(layout->count(), 2);
    EXPECT_EQ(layout->itemAt(0)->widget(),
              std::dynamic_pointer_cast<yoyo::gui_node>(root->childAt(0))->widget());
    EXPECT_EQ(layout->itemAt(1)->widget(),
              std::dynamic_pointer_cast<yoyo::gui_node>(root->childAt(1))->widget());
  }
  {
    root->setLayoutDirection({ ld::HORIZONTAL, true });
    EXPECT_EQ(root->layoutDirection()._s, ld::HORIZONTAL);
    ASSERT_NE(root->widget()->layout(), nullptr);
    ASSERT_EQ(std::string(root->widget()->layout()->metaObject()->className()), "QBoxLayout");
    auto* layout = dynamic_cast<QBoxLayout*>(root->widget()->layout());
    EXPECT_EQ(layout->direction(), QBoxLayout::LeftToRight);
    EXPECT_EQ(layout->count(), 2);
    EXPECT_EQ(layout->itemAt(0)->widget(),
              std::dynamic_pointer_cast<yoyo::gui_node>(root->childAt(0))->widget());
    EXPECT_EQ(layout->itemAt(1)->widget(),
              std::dynamic_pointer_cast<yoyo::gui_node>(root->childAt(1))->widget());
  }

  testing::Mock::VerifyAndClear(root.get());
  testing::Mock::VerifyAndClear(child1.get());
  testing::Mock::VerifyAndClear(child2.get());
  EXPECT_CALL(*child1, destructorCalled());
  EXPECT_CALL(*child2, destructorCalled());
  EXPECT_CALL(*root, destructorCalled());
  child1.reset();
  child2.reset();
  root.reset();
  QApplication::processEvents();
}
