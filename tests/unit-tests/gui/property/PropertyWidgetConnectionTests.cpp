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
public:
  using mock_node::mock_node;

  MOCK_METHOD(yoyo::properties::connection_t, prop1, (), (const));

  MOCK_METHOD(void, setProp1, (yoyo::properties::connection_t v));

Q_SIGNALS:
  void prop1Changed(yoyo::properties::connection_t v);
};

TEST_F(PropertyWidgetTest, connectionProperty)
{
  createWidget();
  auto object = std::make_shared<connection_node_mock>("connection_mock", boost::uuids::uuid {});
  auto docu = yoyo::documentation::builder { "Mock connection" }
                .property("name", "Name", "", "Name-tooltip", QVariant {})
                .property("prop1", "Property 1", "", "p1-tooltip", QVariant {})
                .build();
  EXPECT_EQ(child_count(), 3);

  yoyo::properties::connection_t prop1 { {}, {}, {}, "", "", "aDataObject" };
  EXPECT_CALL(*object, prop1()).WillOnce(Return(prop1));
  object->setName({ "mockObject1", true });

  _widget->itemSelected(object, docu);
  QApplication::processEvents();

  EXPECT_EQ(child_count(), 5 + 2);
  int item_count = 1;

  // first row: type
  {
    auto label = dynamic_cast<QLabel*>(get_child(item_count++));
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "Type");
    label = dynamic_cast<QLabel*>(get_child(item_count++));
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "connection_mock");
  }
  // second row: name
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
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
  // prop1: connecton_t
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop1")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop1")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::connection_input"s);
    ASSERT_EQ(c1->children().size(), 7);
    auto input_in = dynamic_cast<QLineEdit*>(c1->children()[2]);
    auto input_out = dynamic_cast<QLineEdit*>(c1->children()[4]);
    auto input_auto = dynamic_cast<QLineEdit*>(c1->children()[6]);
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
}

#include "PropertyWidgetConnectionTests.moc"
