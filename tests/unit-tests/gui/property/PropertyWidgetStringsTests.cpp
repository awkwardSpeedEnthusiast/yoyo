#include "PropertyWidgetFixture.hpp"
#include "yoyo/documentation.h"
#include "yoyo/node_base.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
using std::string_literals::operator""s;
using testing::_;
using testing::Return;

class string_node_mock : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(QString prop1 READ prop1 WRITE setProp1 NOTIFY prop1Changed)
  Q_PROPERTY(
    yoyo::properties::invisible_string_t prop2 READ prop2 WRITE setProp2 NOTIFY prop2Changed)
  Q_PROPERTY(
    yoyo::properties::connected_string_t prop3 READ prop3 WRITE setProp3 NOTIFY prop3Changed)
  Q_PROPERTY(
    yoyo::properties::patterned_string_t prop4 READ prop4 WRITE setProp4 NOTIFY prop4Changed)
  Q_PROPERTY(yoyo::properties::text_t prop5 READ prop5 WRITE setProp5 NOTIFY prop5Changed)
public:
  using mock_node::mock_node;

  MOCK_METHOD(QString, prop1, (), (const));
  MOCK_METHOD(yoyo::properties::invisible_string_t, prop2, (), (const));
  MOCK_METHOD(yoyo::properties::connected_string_t, prop3, (), (const));
  MOCK_METHOD(yoyo::properties::patterned_string_t, prop4, (), (const));
  MOCK_METHOD(yoyo::properties::text_t, prop5, (), (const));

  MOCK_METHOD(void, setProp1, (QString v));
  MOCK_METHOD(void, setProp2, (yoyo::properties::invisible_string_t v));
  MOCK_METHOD(void, setProp3, (yoyo::properties::connected_string_t v));
  MOCK_METHOD(void, setProp4, (yoyo::properties::patterned_string_t v));
  MOCK_METHOD(void, setProp5, (yoyo::properties::text_t v));

Q_SIGNALS:
  void prop1Changed(QString v);
  void prop2Changed(yoyo::properties::invisible_string_t v);
  void prop3Changed(yoyo::properties::connected_string_t v);
  void prop4Changed(yoyo::properties::patterned_string_t v);
  void prop5Changed(yoyo::properties::text_t v);
};

TEST_F(PropertyWidgetTest, stringProperty)
{
  createWidget();
  auto object = std::make_shared<string_node_mock>("string_mock", boost::uuids::uuid {});
  auto docu = yoyo::documentation::builder { "Mock string" }
                .property("name", "Name", "", "Name-tooltip", QVariant {})
                .property("prop1", "Property 1", "", "p1-tooltip", QVariant {})
                .property("prop2", "Property 2", "", "p2-tooltip", QVariant {})
                .property("prop3", "Property 3", "", "p3-tooltip", QVariant {})
                .property("prop4", "Property 4", "", "p4-tooltip", QVariant {})
                .property("prop5", "Property 5", "", "p5-tooltip", QVariant {})
                .build();
  ASSERT_EQ(_widget->children().size(), 5);
  auto content = _widget->children()[4];
  EXPECT_EQ(content->children().size(), 3);

  EXPECT_CALL(*object, prop1()).WillOnce(Return("MyProp1"));
  yoyo::properties::invisible_string_t prop2 { "invisible", false };
  EXPECT_CALL(*object, prop2()).WillOnce(Return(prop2));
  yoyo::properties::connected_string_t prop3 { "connectedValue", "ownValue" };
  EXPECT_CALL(*object, prop3()).WillOnce(Return(prop3));
  yoyo::properties::patterned_string_t prop4 { "currentValue", QRegExp { "[a-zA-Z]*" } };
  EXPECT_CALL(*object, prop4()).WillOnce(Return(prop4));
  yoyo::properties::text_t prop5 { "a\nlonger\ntext" };
  EXPECT_CALL(*object, prop5()).WillOnce(Return(prop5));
  object->setName("mockObject1");

  _widget->itemSelected(object, docu);

  EXPECT_EQ(content->children().size(), 5 + 5 * 2);

  // first row: type
  {
    auto label = dynamic_cast<QLabel*>(content->children()[1]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "Type");
    label = dynamic_cast<QLabel*>(content->children()[2]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "string_mock");
  }
  // second row: name
  {
    auto label = dynamic_cast<QLabel*>(content->children()[3]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("name")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("name")).toStdString());
    ASSERT_EQ(content->children()[4]->metaObject()->className(), "yoyo::gui::qstring_input"s);
    ASSERT_EQ(content->children()[4]->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(content->children()[4]->children()[1]);
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
  // third row: prop1
  {
    auto label = dynamic_cast<QLabel*>(content->children()[5]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop1")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop1")).toStdString());
    ASSERT_EQ(content->children()[6]->metaObject()->className(), "yoyo::gui::qstring_input"s);
    ASSERT_EQ(content->children()[6]->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(content->children()[6]->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "MyProp1");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop1")).toStdString());

    object->prop1Changed("changedProp1");
    EXPECT_EQ(input->text().toStdString(), "changedProp1");

    EXPECT_CALL(*object, prop1()).WillOnce(Return("changedProp1"));
    input->setText("changed again prop1");
    EXPECT_CALL(*object, setProp1(QString("changed again prop1")));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "changed again prop1");
  }
  // fourth row: prop2
  {
    auto label = dynamic_cast<QLabel*>(content->children()[7]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop2")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop2")).toStdString());
    ASSERT_EQ(content->children()[8]->metaObject()->className(),
              "yoyo::gui::invisible_string_input"s);
    ASSERT_EQ(content->children()[8]->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(content->children()[8]->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "invisible");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop2")).toStdString());
    EXPECT_FALSE(input->isVisible());
    EXPECT_FALSE(label->isVisible());

    prop2._s = "changedProp2";
    object->prop2Changed(prop2);
    EXPECT_EQ(input->text().toStdString(), "changedProp2");

    prop2._visible = true;
    object->prop2Changed(prop2);
    EXPECT_TRUE(label->isVisible());
    EXPECT_TRUE(input->isVisible());

    EXPECT_CALL(*object, prop2()).WillOnce(Return(prop2));
    input->setText("changed again prop2");
    prop2._s = "changed again prop2";
    EXPECT_CALL(*object, setProp2(prop2));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "changed again prop2");
  }
  // fifth row: prop3
  {
    auto label = dynamic_cast<QLabel*>(content->children()[9]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop3")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop3")).toStdString());
    ASSERT_EQ(content->children()[10]->metaObject()->className(),
              "yoyo::gui::connected_string_input"s);
    ASSERT_EQ(content->children()[10]->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(content->children()[10]->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "ownValue");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop3")).toStdString());

    prop3._own_value = "changedOwnValue";
    object->prop3Changed(prop3);
    EXPECT_EQ(input->text().toStdString(), "changedOwnValue");

    prop3._connected_value = "changedConnectedValue";
    object->prop3Changed(prop3);
    EXPECT_EQ(input->text().toStdString(), "changedOwnValue");

    EXPECT_CALL(*object, prop3()).WillOnce(Return(prop3));
    input->setText("changed again prop3");
    prop3._own_value = "changed again prop3";
    EXPECT_CALL(*object, setProp3(prop3));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "changed again prop3");
  }
  // sixth row: prop4
  {
    auto label = dynamic_cast<QLabel*>(content->children()[11]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop4")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop4")).toStdString());
    ASSERT_EQ(content->children()[12]->metaObject()->className(),
              "yoyo::gui::pattern_string_input"s);
    ASSERT_EQ(content->children()[12]->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(content->children()[12]->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "currentValue");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop4")).toStdString());

    prop4._s = "changedValue";
    object->prop4Changed(prop4);
    EXPECT_EQ(input->text().toStdString(), "changedValue");

    input->setText("changed again prop4");
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "changed again prop4");

    EXPECT_CALL(*object, prop4()).WillOnce(Return(prop4));
    input->setText("changedAgainProp");
    prop4._s = "changedAgainProp";
    EXPECT_CALL(*object, setProp4(prop4));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "changedAgainProp");
  }
  // seventh row: prop5
  {
    auto label = dynamic_cast<QLabel*>(content->children()[13]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop5")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop5")).toStdString());
    ASSERT_EQ(content->children()[14]->metaObject()->className(), "yoyo::gui::text_input"s);
    ASSERT_EQ(content->children()[14]->children().size(), 3);
    auto input = dynamic_cast<QTextEdit*>(content->children()[14]->children()[1]);
    auto save = dynamic_cast<QPushButton*>(content->children()[14]->children()[2]);
    ASSERT_NE(input, nullptr);
    ASSERT_NE(save, nullptr);
    EXPECT_EQ(input->toPlainText().toStdString(), "a\nlonger\ntext");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop5")).toStdString());

    prop5._s = "an\neven\nlonger\ntext";
    object->prop5Changed(prop5);
    EXPECT_EQ(input->toPlainText().toStdString(), "an\neven\nlonger\ntext");

    EXPECT_CALL(*object, prop5()).WillOnce(Return(prop5));
    input->setText("can\nthis\ntext\nget\nlonger");
    prop5._s = "can\nthis\ntext\nget\nlonger";
    EXPECT_CALL(*object, setProp5(prop5));
    QTest::mouseClick(save, Qt::LeftButton);
    EXPECT_EQ(input->toPlainText().toStdString(), "can\nthis\ntext\nget\nlonger");
  }
  _widget->itemSelected(nullptr, nullptr);
  testing::Mock::VerifyAndClearExpectations(object.get());
  testing::Mock::VerifyAndClear(object.get());
  testing::Mock::AllowLeak(object.get());
}

#include "PropertyWidgetStringsTests.moc"
