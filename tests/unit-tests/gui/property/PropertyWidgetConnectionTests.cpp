#include "PropertyWidgetFixture.hpp"
#include "yoyo/documentation.h"
#include "yoyo/node_base.h"

#include <QLabel>
#include <QLineEdit>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
using std::string_literals::operator""s;
using testing::_;
using testing::Return;

class connection_node_mock : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::connection_t prop1 READ prop1 WRITE setProp1 NOTIFY prop1Changed)
  Q_PROPERTY(yoyo::properties::in_connection_t prop2 READ prop2 WRITE setProp2 NOTIFY prop2Changed)
  Q_PROPERTY(yoyo::properties::out_connection_t prop3 READ prop3 WRITE setProp3 NOTIFY prop3Changed)
public:
  using mock_node::mock_node;

  MOCK_METHOD(yoyo::properties::connection_t, prop1, (), (const));
  MOCK_METHOD(yoyo::properties::in_connection_t, prop2, (), (const));
  MOCK_METHOD(yoyo::properties::out_connection_t, prop3, (), (const));

  MOCK_METHOD(void, setProp1, (yoyo::properties::connection_t v));
  MOCK_METHOD(void, setProp2, (yoyo::properties::in_connection_t v));
  MOCK_METHOD(void, setProp3, (yoyo::properties::out_connection_t v));

Q_SIGNALS:
  void prop1Changed(yoyo::properties::connection_t v);
  void prop2Changed(yoyo::properties::in_connection_t v);
  void prop3Changed(yoyo::properties::out_connection_t v);
};

TEST_F(PropertyWidgetTest, connectionProperty)
{
  createWidget();
  auto object = std::make_shared<connection_node_mock>("connection_mock", boost::uuids::uuid {});
  auto docu = yoyo::documentation::builder { "Mock connection" }
                .property("name", "Name", "", "Name-tooltip", QVariant {})
                .property("prop1", "Property 1", "", "p1-tooltip", QVariant {})
                .property("prop2", "Property 2", "", "p2-tooltip", QVariant {})
                .property("prop3", "Property 3", "", "p3-tooltip", QVariant {})
                .build();
  ASSERT_EQ(_widget->children().size(), 5);
  auto content = _widget->children()[4];
  EXPECT_EQ(content->children().size(), 3);

  yoyo::properties::connection_t prop1 { {}, {}, {}, "", "", "aDataObject" };
  EXPECT_CALL(*object, prop1()).WillOnce(Return(prop1));
  yoyo::properties::in_connection_t prop2 { {}, {}, "invisible" };
  EXPECT_CALL(*object, prop2()).WillOnce(Return(prop2));
  yoyo::properties::out_connection_t prop3 { {}, {}, "ownValue" };
  EXPECT_CALL(*object, prop3()).WillOnce(Return(prop3));
  object->setName("mockObject1");

  _widget->itemSelected(object, docu);

  EXPECT_EQ(content->children().size(), 5 + 3 * 2);
  int item_index = 1;

  // first row: type
  {
    auto label = dynamic_cast<QLabel*>(content->children()[item_index++]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "Type");
    label = dynamic_cast<QLabel*>(content->children()[item_index++]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "connection_mock");
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
  // prop1: connecton_t
  {
    auto label = dynamic_cast<QLabel*>(content->children()[item_index++]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop1")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop1")).toStdString());
    ASSERT_EQ(content->children()[item_index]->metaObject()->className(),
              "yoyo::gui::connection_input"s);
    ASSERT_EQ(content->children()[item_index]->children().size(), 7);
    auto input_in = dynamic_cast<QLineEdit*>(content->children()[item_index]->children()[2]);
    auto input_out = dynamic_cast<QLineEdit*>(content->children()[item_index]->children()[4]);
    auto input_auto = dynamic_cast<QLineEdit*>(content->children()[item_index++]->children()[6]);
    ASSERT_NE(input_in, nullptr);
    ASSERT_NE(input_out, nullptr);
    ASSERT_NE(input_auto, nullptr);
    EXPECT_EQ(input_in->text().toStdString(), "");
    EXPECT_EQ(input_out->text().toStdString(), "");
    EXPECT_EQ(input_auto->text().toStdString(), "aDataObject");
    EXPECT_EQ(input_in->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop1")).toStdString());

    prop1._auto = "anOtherDataObject";
    object->prop1Changed(prop1);
    EXPECT_EQ(input_auto->text().toStdString(), "anOtherDataObject");

    EXPECT_CALL(*object, prop1()).WillOnce(Return(prop1));
    input_auto->setText("changed again prop1");
    prop1._auto = "changed again prop1";
    EXPECT_CALL(*object, setProp1(prop1));
    QTest::keyClick(input_auto, Qt::Key_Return);
    EXPECT_EQ(input_auto->text().toStdString(), "changed again prop1");

    prop1._in = "inputDataObject";
    object->prop1Changed(prop1);
    EXPECT_EQ(input_in->text().toStdString(), "inputDataObject");

    EXPECT_CALL(*object, prop1()).WillOnce(Return(prop1));
    input_in->setText("otherInputDataObject");
    prop1._in = "otherInputDataObject";
    EXPECT_CALL(*object, setProp1(prop1));
    QTest::keyClick(input_in, Qt::Key_Return);
    EXPECT_EQ(input_in->text().toStdString(), "otherInputDataObject");

    prop1._out = "outputDataObject";
    object->prop1Changed(prop1);
    EXPECT_EQ(input_out->text().toStdString(), "outputDataObject");

    EXPECT_CALL(*object, prop1()).WillOnce(Return(prop1));
    input_out->setText("otherOutputDataObject");
    prop1._out = "otherOutputDataObject";
    EXPECT_CALL(*object, setProp1(prop1));
    QTest::keyClick(input_out, Qt::Key_Return);
    EXPECT_EQ(input_out->text().toStdString(), "otherOutputDataObject");
  }
  // prop2: in_connection
  {
    auto label = dynamic_cast<QLabel*>(content->children()[item_index++]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop2")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop2")).toStdString());
    ASSERT_EQ(content->children()[item_index]->metaObject()->className(),
              "yoyo::gui::connection_in_input"s);
    ASSERT_EQ(content->children()[item_index]->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(content->children()[item_index++]->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "invisible");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop2")).toStdString());

    prop2._in = "changedProp2";
    object->prop2Changed(prop2);
    EXPECT_EQ(input->text().toStdString(), "changedProp2");

    EXPECT_CALL(*object, prop2()).WillOnce(Return(prop2));
    input->setText("changed again prop2");
    prop2._in = "changed again prop2";
    EXPECT_CALL(*object, setProp2(prop2));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "changed again prop2");
  }
  // prop3: out_connection
  {
    auto label = dynamic_cast<QLabel*>(content->children()[item_index++]);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop3")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop3")).toStdString());
    ASSERT_EQ(content->children()[item_index]->metaObject()->className(),
              "yoyo::gui::connection_out_input"s);
    ASSERT_EQ(content->children()[item_index]->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(content->children()[item_index++]->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "ownValue");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop3")).toStdString());

    prop3._out = "changedOwnValue";
    object->prop3Changed(prop3);
    EXPECT_EQ(input->text().toStdString(), "changedOwnValue");

    prop3._out = "changedConnectedValue";
    object->prop3Changed(prop3);
    EXPECT_EQ(input->text().toStdString(), "changedConnectedValue");

    EXPECT_CALL(*object, prop3()).WillOnce(Return(prop3));
    input->setText("changed again prop3");
    prop3._out = "changed again prop3";
    EXPECT_CALL(*object, setProp3(prop3));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "changed again prop3");
  }
  testing::Mock::VerifyAndClear(object.get());
}

#include "PropertyWidgetConnectionTests.moc"
