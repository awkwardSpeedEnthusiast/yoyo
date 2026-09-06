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

  Q_PROPERTY(yoyo::properties::script_t prop6 READ prop6 WRITE set_prop6 NOTIFY prop6Changed)
public:
  using mock_node::mock_node;

  DECLARE_PROPERTY(yoyo::properties::script_t, prop6);
};

TEST_F(PropertyWidgetTest, mixedProperty)
{
  createWidget();
  auto object = std::make_shared<mixed_node_mock>("mixed_mock", boost::uuids::uuid {});
  auto docu = yoyo::documentation::builder { "Mock mix" }
                .property("name", "Name", "", "Name-tooltip", QVariant {})
                .property("prop6", "Property 6", "", "p6-tooltip", QVariant {})
                .build();
  EXPECT_EQ(child_count(), 3);

  yoyo::properties::script_t prop6 { "bla foo;", yoyo::properties::script_t::type::JS };
  EXPECT_CALL(*object, prop6()).WillOnce(Return(prop6));
  object->setName({ "mockObject1", true });

  _widget->itemSelected(object, docu);
  _widget->show();
  QApplication::processEvents();

  EXPECT_EQ(child_count(), 5 + 2);
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
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::invisible_string_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    // EXPECT_EQ(input->text().toStdString(), "mockObject1");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("name")).toStdString());

    object->setName({ "anOtherName", true });
    EXPECT_EQ(input->text().toStdString(), "anOtherName");

    input->setText("changedName");
    QTest::keyClick(input, Qt::Key_Return);

    EXPECT_EQ(object->name()._s, "changedName");
    EXPECT_EQ(input->text().toStdString(), "changedName");
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
}
#include "PropertyWidgetMixedTests.moc"
