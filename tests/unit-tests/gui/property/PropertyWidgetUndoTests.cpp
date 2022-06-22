#include "PropertyWidgetFixture.hpp"
#include "command_handler.h"
#include "yoyo/documentation.h"
#include "yoyo/node_base.h"

#include <QLabel>
#include <QLineEdit>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
using std::string_literals::operator""s;
using testing::_;
using testing::Invoke;
using testing::Return;

class undo_node_mock : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(int prop1 READ prop1 WRITE set_prop1 NOTIFY prop1Changed)
public:
  using mock_node::mock_node;

  DECLARE_PROPERTY(int, prop1);
};

TEST_F(PropertyWidgetTest, undo)
{
  createWidget();
  auto object = std::make_shared<undo_node_mock>("undo_mock", boost::uuids::uuid {});
  auto docu = yoyo::documentation::builder { "Mock undo" }
                .property("name", "Name", "", "Name-tooltip", QVariant {})
                .property("prop1", "Property 1", "", "p1-tooltip", QVariant {})
                .build();
  ON_CALL(*object, set_prop1(_)).WillByDefault(Invoke([object](auto v) {
    object->prop1Changed(v);
  }));
  EXPECT_CALL(*object, prop1()).WillOnce(Return(42));
  object->setName("mockObject1");

  _widget->itemSelected(object, docu);

  auto handler = yoyo::command::commandhandler();
  ASSERT_NE(handler, nullptr);

  EXPECT_EQ(child_count(), 5 + 2 * 1);
  auto item_count = 3;
  // undo name
  {
    auto c1 = get_child(item_count++);
    item_count++;
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "mockObject1");

    EXPECT_FALSE(handler->hasCommandToUndo());
    EXPECT_FALSE(handler->hasCommandToRedo());
    input->setText("changedName");
    QTest::keyClick(input, Qt::Key_Return);

    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo().toStdString(), "Set property name");
    EXPECT_EQ(object->name(), "changedName");
    EXPECT_EQ(input->text().toStdString(), "changedName");

    handler->undo();
    EXPECT_FALSE(handler->hasCommandToUndo());
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Set property name");
    EXPECT_EQ(object->name(), "mockObject1");
    EXPECT_EQ(input->text().toStdString(), "mockObject1");

    handler->redo();
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_FALSE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextUndo().toStdString(), "Set property name");
    EXPECT_EQ(object->name(), "changedName");
    EXPECT_EQ(input->text().toStdString(), "changedName");
  }
  handler->clearStack();
  // undo prop1
  {
    auto c1 = get_child(item_count++);
    item_count++;
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);

    EXPECT_FALSE(handler->hasCommandToUndo());
    EXPECT_FALSE(handler->hasCommandToRedo());

    EXPECT_CALL(*object, prop1()).WillOnce(Return(1));
    input->setText("-11");
    EXPECT_CALL(*object, set_prop1(-11));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toInt(), -11);
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_FALSE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextUndo().toStdString(), "Set property prop1");

    EXPECT_CALL(*object, set_prop1(1));
    handler->undo();
    EXPECT_EQ(input->text().toInt(), 1);
    EXPECT_FALSE(handler->hasCommandToUndo());
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Set property prop1");

    EXPECT_CALL(*object, set_prop1(-11));
    handler->redo();
    EXPECT_EQ(input->text().toInt(), -11);
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_FALSE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextUndo().toStdString(), "Set property prop1");
  }
  EXPECT_TRUE(testing::Mock::VerifyAndClear(object.get()));
  testing::Mock::AllowLeak(object.get());
}

#include "PropertyWidgetUndoTests.moc"
