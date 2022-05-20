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

class enum_node_mock : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::enum_t prop1 READ prop1 WRITE set_prop1 NOTIFY prop1Changed)
public:
  using mock_node::mock_node;

  DECLARE_PROPERTY(yoyo::properties::enum_t, prop1);
};

TEST_F(PropertyWidgetTest, enumProperty)
{
  createWidget();
  auto object = std::make_shared<enum_node_mock>("enum_mock", boost::uuids::uuid {});
  auto docu = yoyo::documentation::builder { "Mock enum" }
                .property("name", "Name", "", "Name-tooltip", QVariant {})
                .property("prop1", "Property 1", "", "p1-tooltip", QVariant {})
                .build();
  ASSERT_EQ(_widget->children().size(), 5);
  auto content = _widget->children()[4];
  EXPECT_EQ(content->children().size(), 3);

  yoyo::properties::enum_t prop1 { false, {} };
  EXPECT_CALL(*object, prop1()).WillOnce(Return(prop1));
  object->setName("mockObject1");

  _widget->itemSelected(object, docu);
  QApplication::processEvents();

  EXPECT_EQ(content->children().size(), 5 + 1 * 2);
  int item_index = 1;

  // first row: type
  {
    auto label = dynamic_cast<QLabel*>(content->children()[item_index++]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "Type");
    label = dynamic_cast<QLabel*>(content->children()[item_index++]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "enum_mock");
  }
  // second row: name
  {
    auto label = dynamic_cast<QLabel*>(content->children()[item_index++]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("name")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("name")).toStdString());
    ASSERT_EQ(content->children()[item_index]->metaObject()->className(),
              "yoyo::gui::qstring_input"s);
    ASSERT_EQ(content->children()[item_index]->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(content->children()[item_index++]->children()[1]);
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
  // prop1: enum_t
  {
    ON_CALL(*object, set_prop1(_)).WillByDefault(Invoke([object](auto v) {
      object->prop1Changed(v);
    }));
    using sc = yoyo::properties::script_t;
    ASSERT_GT(content->children().size(), item_index);
    auto label = dynamic_cast<QLabel*>(content->children()[item_index++]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop1")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop1")).toStdString());
    ASSERT_EQ(content->children()[item_index]->metaObject()->className(), "yoyo::gui::enum_input"s);
    ASSERT_EQ(content->children()[item_index]->children().size(), 13);
    auto input_enabled = dynamic_cast<QCheckBox*>(content->children()[item_index]->children()[2]);
    auto input_select = dynamic_cast<QComboBox*>(content->children()[item_index]->children()[4]);
    auto input_data = dynamic_cast<QLineEdit*>(content->children()[item_index]->children()[6]);
    auto input_value = dynamic_cast<QLineEdit*>(content->children()[item_index]->children()[8]);
    auto input_title = dynamic_cast<QLineEdit*>(content->children()[item_index]->children()[10]);
    auto input_event = dynamic_cast<QLineEdit*>(content->children()[item_index++]->children()[12]);
    ASSERT_NE(input_enabled, nullptr);
    ASSERT_NE(input_select, nullptr);
    ASSERT_NE(input_data, nullptr);
    ASSERT_NE(input_value, nullptr);
    ASSERT_NE(input_title, nullptr);
    ASSERT_NE(input_event, nullptr);
    EXPECT_EQ(input_enabled->isChecked(), false);
    EXPECT_FALSE(input_select->isEnabled());
    EXPECT_FALSE(input_data->isEnabled());
    EXPECT_FALSE(input_value->isEnabled());
    EXPECT_FALSE(input_title->isEnabled());
    EXPECT_FALSE(input_event->isEnabled());
    EXPECT_EQ(input_enabled->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop1")).toStdString());

    prop1._enabled = true;
    EXPECT_CALL(*object, set_prop1(prop1));
    input_enabled->click();
    EXPECT_EQ(input_enabled->isChecked(), true);
    EXPECT_TRUE(input_select->isEnabled());
    EXPECT_TRUE(input_data->isEnabled());
    EXPECT_TRUE(input_value->isEnabled());
    EXPECT_TRUE(input_title->isEnabled());
    EXPECT_TRUE(input_event->isEnabled());

    prop1._enabled = false;
    EXPECT_CALL(*object, set_prop1(prop1));
    input_enabled->click();
    EXPECT_EQ(input_enabled->isChecked(), false);
    EXPECT_FALSE(input_select->isEnabled());
    EXPECT_FALSE(input_data->isEnabled());
    EXPECT_FALSE(input_value->isEnabled());
    EXPECT_FALSE(input_title->isEnabled());
    EXPECT_FALSE(input_event->isEnabled());

    prop1._enabled = true;
    prop1._values.insert({ 0, std::make_tuple("v1", "V1", sc { "img1.jpg", sc::type::IMAGE }) });
    prop1._values.insert({ 1, std::make_tuple("v2", "V2", sc { "img2.jpg", sc::type::IMAGE }) });
    object->prop1Changed(prop1);
    EXPECT_EQ(input_enabled->isChecked(), true);
    EXPECT_TRUE(input_select->isEnabled());
    EXPECT_TRUE(input_data->isEnabled());
    EXPECT_TRUE(input_value->isEnabled());
    EXPECT_TRUE(input_title->isEnabled());
    EXPECT_TRUE(input_event->isEnabled());
    EXPECT_EQ(input_select->count(), 2);
    EXPECT_EQ(input_select->currentIndex(), 0);
    EXPECT_EQ(input_data->text(), "0");
    EXPECT_EQ(input_value->text(), "v1");
    EXPECT_EQ(input_title->text(), "V1");
    EXPECT_EQ(input_event->text(), "img1.jpg");

    input_select->setCurrentIndex(1);
    EXPECT_EQ(input_data->text(), "1");
    EXPECT_EQ(input_value->text(), "v2");
    EXPECT_EQ(input_title->text(), "V2");
    EXPECT_EQ(input_event->text(), "img2.jpg");

    std::get<0>(prop1._values[1]) = "value2";
    EXPECT_CALL(*object, set_prop1(prop1));
    input_value->setText("value2");
    QTest::keyClick(input_value, Qt::Key_Return);

    std::get<1>(prop1._values[1]) = "title2";
    EXPECT_CALL(*object, set_prop1(prop1));
    input_title->setText("title2");
    QTest::keyClick(input_title, Qt::Key_Return);

    input_select->setCurrentIndex(0);

    std::get<2>(prop1._values[0])._text = "image1.jpg";
    EXPECT_EQ(std::get<2>(prop1._values[0])._text, "image1.jpg");
    EXPECT_CALL(*object, set_prop1(prop1));
    input_event->setText("image1.jpg");
    QTest::keyClick(input_event, Qt::Key_Return);

    input_data->setText("1");
    QTest::keyClick(input_data, Qt::Key_Return);
    EXPECT_EQ(input_select->currentIndex(), 1);
    EXPECT_EQ(input_data->text(), "1");
    EXPECT_EQ(input_value->text(), "value2");
    EXPECT_EQ(input_title->text(), "title2");
    EXPECT_EQ(input_event->text(), "img2.jpg");

    prop1._values.insert({ 2, { "", "", { "", sc::type::NATIVE } } });
    EXPECT_CALL(*object, set_prop1(prop1));
    input_data->setText("2");
    QTest::keyClick(input_data, Qt::Key_Return);
    EXPECT_EQ(input_select->currentIndex(), 2);
    EXPECT_EQ(input_select->currentText(), "0x2");
    EXPECT_EQ(input_data->text(), "2");
    EXPECT_EQ(input_value->text(), "");
    EXPECT_EQ(input_title->text(), "");
    EXPECT_EQ(input_event->text(), "");

    prop1._values = {};
    object->prop1Changed(prop1);
    QApplication::processEvents();

    prop1._values.insert({ 2, { "", "", { "", sc::type::NATIVE } } });
    EXPECT_CALL(*object, set_prop1(prop1));
    input_data->setText("2");
    QTest::keyClick(input_data, Qt::Key_Return);
    EXPECT_EQ(input_select->currentIndex(), 0);
    EXPECT_EQ(input_select->currentText(), "0x2");
    EXPECT_EQ(input_data->text(), "2");
    EXPECT_EQ(input_value->text(), "");
    EXPECT_EQ(input_title->text(), "");
    EXPECT_EQ(input_event->text(), "");
  }
  testing::Mock::VerifyAndClear(object.get());
}
#include "PropertyWidgetEnumTests.moc"
