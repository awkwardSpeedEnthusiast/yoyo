#include "button.hpp"

#include "guiFixture.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QApplication>
#include <QPushButton>
#include <QSignalSpy>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <memory>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;
using testing::WithArg;

class buttonTest : public yoyo::test::guiFixture
{
};

TEST_F(buttonTest, icon)
{
  auto id = boost::uuids::to_string(yoyo::button::typeId());
  EXPECT_TRUE(QFile::exists(":/yoyo/nodes/" + QString::fromStdString(id) + ".png"));
}

TEST_F(buttonTest, properties)
{
  auto button = std::make_shared<yoyo::button>();
  button->init();
  EXPECT_EQ(button->type(), "button");
  EXPECT_FALSE(button->identifier().is_nil());
  EXPECT_EQ(button->name()._s, "");
  EXPECT_EQ(button->layoutDirection()._s, yoyo::types::layout_direction_t::HORIZONTAL);
  EXPECT_EQ(button->layoutDirection()._visible, false);
  EXPECT_EQ(button->styleSheet(), yoyo::properties::text_t { "" });
  EXPECT_EQ(button->xPosition(), 0);
  EXPECT_EQ(button->yPosition(), 0);
  EXPECT_EQ(button->width(), 0);
  EXPECT_EQ(button->height(), 0);
  EXPECT_EQ(button->title(), "");
  EXPECT_EQ(button->onClick(), yoyo::properties::script_t {});
  EXPECT_GE(button->metaObject()->indexOfProperty("name"),
            yoyo::node_base::staticMetaObject.propertyOffset());
  EXPECT_GE(button->metaObject()->indexOfProperty("layoutDirection"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(button->metaObject()->indexOfProperty("styleSheet"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(button->metaObject()->indexOfProperty("xPosition"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(button->metaObject()->indexOfProperty("yPosition"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(button->metaObject()->indexOfProperty("width"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(button->metaObject()->indexOfProperty("height"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(button->metaObject()->indexOfProperty("enabled"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(button->metaObject()->indexOfProperty("visible"),
            yoyo::gui_node::staticMetaObject.propertyOffset());
  EXPECT_GE(button->metaObject()->indexOfProperty("title"),
            yoyo::button::staticMetaObject.propertyOffset());
  EXPECT_GE(button->metaObject()->indexOfProperty("onClick"),
            yoyo::button::staticMetaObject.propertyOffset());
  ASSERT_NE(button->widget(), nullptr);

  struct Receiver {
    MOCK_METHOD(void, onTitleChanged, (QString title));
    MOCK_METHOD(void, onOnClickChanged, (yoyo::properties::script_t s));
  } receiver;

  QObject::connect(button.get(), &yoyo::button::titleChanged,
                   [&receiver](auto v) { receiver.onTitleChanged(v); });
  QObject::connect(button.get(), &yoyo::button::onClickChanged,
                   [&receiver](auto s) { receiver.onOnClickChanged(s); });

  {
    EXPECT_CALL(receiver, onTitleChanged(QString("foo")));
    button->setTitle("foo");
    EXPECT_EQ(button->title(), "foo");
  }
  {
    using yoyo::properties::script_t;
    EXPECT_CALL(receiver, onOnClickChanged(script_t { "script.js", script_t::type::FILE }));
    button->setOnClick({ "script.js", script_t::type::FILE });
    EXPECT_EQ(button->onClick()._text, "script.js");
    EXPECT_EQ(button->onClick()._script_type, script_t::type::FILE);

    EXPECT_CALL(receiver, onOnClickChanged(script_t { "image.jpg", script_t::type::IMAGE }));
    button->setOnClick({ "image.jpg", script_t::type::IMAGE });
    EXPECT_EQ(button->onClick()._text, "image.jpg");
    EXPECT_EQ(button->onClick()._script_type, script_t::type::IMAGE);
  }
}

TEST_F(buttonTest, button)
{
  auto button = std::make_shared<yoyo::button>();
  button->init();
  ASSERT_EQ(button->widget()->children().count(), 2);
  auto buttonWidget = button->widget()->children().at(1);
  ASSERT_NE(buttonWidget, nullptr);
  EXPECT_EQ(buttonWidget->metaObject()->className(), std::string { "QPushButton" });
  auto pushButton = dynamic_cast<QPushButton*>(buttonWidget);
  ASSERT_NE(pushButton, nullptr);
  EXPECT_EQ(pushButton->text(), "");

  button->setTitle("foo");
  EXPECT_EQ(pushButton->text(), "foo");

  struct Receiver {
    MOCK_METHOD(void, scriptExecutionRequested, (yoyo::properties::script_t s));
  } receiver;

  QObject::connect(button.get(), &yoyo::button::scriptExecutionRequested,
                   [&receiver](auto s) { receiver.scriptExecutionRequested(s); });

  EXPECT_CALL(receiver, scriptExecutionRequested(_)).Times(0);
  QTest::mouseClick(pushButton, Qt::LeftButton);
  QApplication::processEvents();

  auto script =
    yoyo::properties::script_t { "myScript.js", yoyo::properties::script_t::type::FILE };
  EXPECT_CALL(receiver, scriptExecutionRequested(script));
  button->setOnClick(script);
  QTest::mouseClick(pushButton, Qt::LeftButton);
  QApplication::processEvents();
}
