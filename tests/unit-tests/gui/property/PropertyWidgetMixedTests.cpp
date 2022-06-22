#include "PropertyWidgetFixture.hpp"
#include "yoyo/documentation.h"
#include "yoyo/node_base.h"

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
using std::string_literals::operator""s;
using testing::_;
using testing::Invoke;
using testing::Return;

class mixed_node_mock : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(bool prop1 READ prop1 WRITE set_prop1 NOTIFY prop1Changed)
  Q_PROPERTY(yoyo::types::access_t prop2 READ prop2 WRITE set_prop2 NOTIFY prop2Changed)
  Q_PROPERTY(yoyo::types::layout_direction_t prop3 READ prop3 WRITE set_prop3 NOTIFY prop3Changed)
  Q_PROPERTY(yoyo::properties::invisible_layout_direction_t prop4 READ prop4 WRITE set_prop4 NOTIFY
               prop4Changed)
  Q_PROPERTY(
    yoyo::properties::connected_boolean_t prop5 READ prop5 WRITE set_prop5 NOTIFY prop5Changed)
  Q_PROPERTY(yoyo::properties::script_t prop6 READ prop6 WRITE set_prop6 NOTIFY prop6Changed)
public:
  using mock_node::mock_node;

  DECLARE_PROPERTY(bool, prop1);
  DECLARE_PROPERTY(yoyo::types::access_t, prop2);
  DECLARE_PROPERTY(yoyo::types::layout_direction_t, prop3);
  DECLARE_PROPERTY(yoyo::properties::invisible_layout_direction_t, prop4);
  DECLARE_PROPERTY(yoyo::properties::connected_boolean_t, prop5);
  DECLARE_PROPERTY(yoyo::properties::script_t, prop6);
};

TEST_F(PropertyWidgetTest, mixedProperty)
{
  createWidget();
  auto object = std::make_shared<mixed_node_mock>("mixed_mock", boost::uuids::uuid {});
  auto docu = yoyo::documentation::builder { "Mock mix" }
                .property("name", "Name", "", "Name-tooltip", QVariant {})
                .property("prop1", "Property 1", "", "p1-tooltip", QVariant {})
                .property("prop2", "Property 2", "", "p2-tooltip", QVariant {})
                .property("prop3", "Property 3", "", "p3-tooltip", QVariant {})
                .property("prop4", "Property 4", "", "p4-tooltip", QVariant {})
                .property("prop5", "Property 5", "", "p5-tooltip", QVariant {})
                .property("prop6", "Property 6", "", "p6-tooltip", QVariant {})
                .build();
  EXPECT_EQ(child_count(), 3);

  EXPECT_CALL(*object, prop1()).WillOnce(Return(true));
  EXPECT_CALL(*object, prop2()).WillOnce(Return(yoyo::types::access_t::NONE));
  EXPECT_CALL(*object, prop3()).WillOnce(Return(yoyo::types::layout_direction_t::HORIZONTAL));
  yoyo::properties::invisible_layout_direction_t prop4 { yoyo::types::layout_direction_t::FREE,
                                                         false };
  yoyo::properties::connected_boolean_t prop5 {
    yoyo::properties::connected_boolean_t::boolean_t::TRUE, false
  };
  yoyo::properties::script_t prop6 { "bla foo;", yoyo::properties::script_t::type::JS };
  EXPECT_CALL(*object, prop4()).WillOnce(Return(prop4));
  EXPECT_CALL(*object, prop5()).WillOnce(Return(prop5));
  EXPECT_CALL(*object, prop6()).WillOnce(Return(prop6));
  object->setName("mockObject1");

  _widget->itemSelected(object, docu);
  _widget->show();
  QApplication::processEvents();

  EXPECT_EQ(child_count(), 5 + 6 * 2);
  int item_index = 1;

  // first row: type
  {
    auto label = dynamic_cast<QLabel*>(get_child(item_index++));
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "Type");
    label = dynamic_cast<QLabel*>(get_child(item_index++));
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "mixed_mock");
  }
  // second row: name
  {
    auto c1 = get_child(item_index++);
    auto c2 = get_child(item_index++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("name")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("name")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::qstring_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "mockObject1");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("name")).toStdString());

    object->setName("anOtherName");
    EXPECT_EQ(input->text().toStdString(), "anOtherName");

    input->setText("changedName");
    QTest::keyClick(input, Qt::Key_Return);

    EXPECT_EQ(object->name(), "changedName");
    EXPECT_EQ(input->text().toStdString(), "changedName");
  }
  // prop1: bool
  {
    auto c1 = get_child(item_index++);
    auto c2 = get_child(item_index++);
    ASSERT_GT(child_count(), item_index);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop1")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop1")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::bool_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QCheckBox*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->isChecked(), true);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop1")).toStdString());

    object->prop1Changed(false);
    EXPECT_EQ(input->isChecked(), false);

    EXPECT_CALL(*object, prop1()).WillOnce(Return(false));
    EXPECT_CALL(*object, set_prop1(true));
    input->click();
    EXPECT_EQ(input->isChecked(), true);

    EXPECT_CALL(*object, prop1()).WillOnce(Return(true));
    EXPECT_CALL(*object, set_prop1(false));
    input->click();
    EXPECT_EQ(input->isChecked(), false);
  }
  // prop2: access_t
  {
    ASSERT_GT(child_count(), item_index);
    auto c1 = get_child(item_index++);
    auto c2 = get_child(item_index++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop2")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop2")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::access_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QComboBox*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->currentIndex(), 0);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop2")).toStdString());

    object->prop2Changed(yoyo::types::access_t::READ_WRITE);
    EXPECT_EQ(input->currentIndex(), 3);

    EXPECT_CALL(*object, prop2()).WillOnce(Return(yoyo::types::access_t::READ_WRITE));
    EXPECT_CALL(*object, set_prop2(yoyo::types::access_t::READ));
    input->setCurrentIndex(1);

    EXPECT_CALL(*object, prop2()).WillOnce(Return(yoyo::types::access_t::WRITE));
    EXPECT_CALL(*object, set_prop2(yoyo::types::access_t::WRITE));
    input->setCurrentIndex(2);
  }
  // prop3: layout_direction_t
  {
    ASSERT_GT(child_count(), item_index);
    auto c1 = get_child(item_index++);
    auto c2 = get_child(item_index++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop3")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop3")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::layout_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QComboBox*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->currentIndex(), 1);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop3")).toStdString());

    object->prop3Changed(yoyo::types::layout_direction_t::FREE);
    EXPECT_EQ(input->currentIndex(), 0);

    EXPECT_CALL(*object, prop3()).WillOnce(Return(yoyo::types::layout_direction_t::FREE));
    EXPECT_CALL(*object, set_prop3(yoyo::types::layout_direction_t::VERTICAL));
    input->setCurrentIndex(2);
  }
  // prop4: invisible_layout_direction_t
  {
    ASSERT_GT(child_count(), item_index);
    auto c1 = get_child(item_index++);
    auto c2 = get_child(item_index++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop4")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop4")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::layout_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QComboBox*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->currentIndex(), 0);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop4")).toStdString());

    EXPECT_FALSE(input->isVisible());
    EXPECT_FALSE(label->isVisible());

    EXPECT_CALL(*object, prop4()).WillOnce(Return(prop4));
    prop4._s = yoyo::types::layout_direction_t::VERTICAL;
    EXPECT_CALL(*object, set_prop4(prop4));
    input->setCurrentIndex(2);

    prop4._s = yoyo::types::layout_direction_t::HORIZONTAL;
    prop4._visible = true;
    object->prop4Changed(prop4);
    EXPECT_EQ(input->currentIndex(), 1);
    EXPECT_TRUE(input->isVisible());
    EXPECT_TRUE(label->isVisible());

    prop4._visible = false;
    object->prop4Changed(prop4);
    EXPECT_EQ(input->currentIndex(), 1);
    EXPECT_FALSE(input->isVisible());
    EXPECT_FALSE(label->isVisible());
  }
  // prop5: connected_boolean_t
  {
    ASSERT_GT(child_count(), item_index);
    auto c1 = get_child(item_index++);
    auto c2 = get_child(item_index++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop5")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop5")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::connected_boolean_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QComboBox*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->currentIndex(), 0);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop5")).toStdString());

    prop5._type = yoyo::properties::connected_boolean_t::boolean_t::FALSE;
    object->prop5Changed(prop5);
    EXPECT_EQ(input->currentIndex(), 1);

    EXPECT_CALL(*object, prop5()).WillOnce(Return(prop5));
    prop5._type = yoyo::properties::connected_boolean_t::boolean_t::DEFINED_BY_CONNECTION;
    EXPECT_CALL(*object, set_prop5(prop5));
    input->setCurrentIndex(2);
  }
  // prop6: script_t
  {
    QApplication::processEvents();
    ON_CALL(*object, set_prop6(_)).WillByDefault(Invoke([object](auto v) {
      Q_EMIT object->prop6Changed(v);
    }));
    ASSERT_GT(child_count(), item_index);
    auto c1 = get_child(item_index++);
    auto c2 = get_child(item_index++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop6")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop6")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::script_input"s);
    ASSERT_EQ(c1->children().size(), 5);
    auto input_type = dynamic_cast<QComboBox*>(c1->children()[1]);
    auto input_file = dynamic_cast<QLineEdit*>(c1->children()[2]);
    auto input_text = dynamic_cast<QTextEdit*>(c1->children()[3]);
    auto input_save = dynamic_cast<QPushButton*>(c1->children()[4]);
    ASSERT_NE(input_type, nullptr);
    ASSERT_NE(input_file, nullptr);
    ASSERT_NE(input_text, nullptr);
    ASSERT_NE(input_save, nullptr);
    EXPECT_EQ(input_type->currentIndex(), 1);
    EXPECT_EQ(input_file->text().toStdString(), "bla foo;");
    EXPECT_EQ(input_text->toPlainText().toStdString(), "bla foo;");
    EXPECT_FALSE(input_file->isVisible());
    EXPECT_TRUE(input_text->isVisible());
    EXPECT_TRUE(input_save->isVisible());
    EXPECT_EQ(input_type->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop6")).toStdString());

    prop6._script_type = yoyo::properties::script_t::type::FILE;
    prop6._text = "lorem_ipsum.script";
    object->prop6Changed(prop6);
    QApplication::processEvents();
    EXPECT_EQ(input_type->currentIndex(), 0);
    EXPECT_EQ(input_file->text().toStdString(), "lorem_ipsum.script");
    EXPECT_EQ(input_text->toPlainText().toStdString(), "lorem_ipsum.script");
    EXPECT_TRUE(input_file->isVisible());
    EXPECT_FALSE(input_text->isVisible());
    EXPECT_FALSE(input_save->isVisible());

    EXPECT_CALL(*object, prop6()).WillOnce(Return(prop6));
    prop6._text = "anOtherFile.js";
    EXPECT_CALL(*object, set_prop6(prop6));
    input_file->setText("anOtherFile.js");
    QTest::keyClick(input_file, Qt::Key_Return);

    EXPECT_CALL(*object, prop6()).WillOnce(Return(prop6));
    prop6._script_type = yoyo::properties::script_t::type::NATIVE;
    EXPECT_CALL(*object, set_prop6(prop6));
    input_type->setCurrentIndex(2);
    QApplication::processEvents();
    EXPECT_FALSE(input_file->isVisible());
    EXPECT_TRUE(input_text->isVisible());
    EXPECT_TRUE(input_save->isVisible());

    EXPECT_CALL(*object, prop6()).WillOnce(Return(prop6));
    prop6._text = "aGroup.myData=5\n";
    EXPECT_CALL(*object, set_prop6(prop6));
    input_text->setText("aGroup.myData=5\n");
    QTest::mouseClick(input_save, Qt::LeftButton);
  }
  testing::Mock::VerifyAndClear(object.get());
  testing::Mock::AllowLeak(object.get());
}
#include "PropertyWidgetMixedTests.moc"
