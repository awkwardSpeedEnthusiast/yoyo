#include "PropertyWidgetFixture.hpp"
#include "yoyo/documentation.h"
#include "yoyo/node_base.h"

#include <QLabel>
#include <QLineEdit>
#include <QLocale>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
using std::string_literals::operator""s;
using testing::_;
using testing::Invoke;
using testing::Return;

class limited_node_mock : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::limited_int8_t prop1 READ prop1 WRITE set_prop1 NOTIFY prop1Changed)
  Q_PROPERTY(yoyo::properties::limited_int16_t prop2 READ prop2 WRITE set_prop2 NOTIFY prop2Changed)
  Q_PROPERTY(yoyo::properties::limited_int32_t prop3 READ prop3 WRITE set_prop3 NOTIFY prop3Changed)
  Q_PROPERTY(yoyo::properties::limited_int64_t prop4 READ prop4 WRITE set_prop4 NOTIFY prop4Changed)
  Q_PROPERTY(yoyo::properties::limited_uint8_t prop5 READ prop5 WRITE set_prop5 NOTIFY prop5Changed)
  Q_PROPERTY(
    yoyo::properties::limited_uint16_t prop6 READ prop6 WRITE set_prop6 NOTIFY prop6Changed)
  Q_PROPERTY(
    yoyo::properties::limited_uint32_t prop7 READ prop7 WRITE set_prop7 NOTIFY prop7Changed)
  Q_PROPERTY(
    yoyo::properties::limited_uint64_t prop8 READ prop8 WRITE set_prop8 NOTIFY prop8Changed)
  Q_PROPERTY(yoyo::properties::limited_float_t prop9 READ prop9 WRITE set_prop9 NOTIFY prop9Changed)
public:
  using mock_node::mock_node;

  DECLARE_PROPERTY(yoyo::properties::limited_int8_t, prop1);
  DECLARE_PROPERTY(yoyo::properties::limited_int16_t, prop2);
  DECLARE_PROPERTY(yoyo::properties::limited_int32_t, prop3);
  DECLARE_PROPERTY(yoyo::properties::limited_int64_t, prop4);
  DECLARE_PROPERTY(yoyo::properties::limited_uint8_t, prop5);
  DECLARE_PROPERTY(yoyo::properties::limited_uint16_t, prop6);
  DECLARE_PROPERTY(yoyo::properties::limited_uint32_t, prop7);
  DECLARE_PROPERTY(yoyo::properties::limited_uint64_t, prop8);
  DECLARE_PROPERTY(yoyo::properties::limited_float_t, prop9);
};

TEST_F(PropertyWidgetTest, limitedNumberProperty)
{
  createWidget();
  auto object = std::make_shared<limited_node_mock>("number_mock", boost::uuids::uuid {});
  auto docu = yoyo::documentation::builder { "Mock numbers" }
                .property("name", "Name", "", "Name-tooltip", QVariant {})
                .property("prop1", "Property 1", "", "p1-tooltip", QVariant {})
                .property("prop2", "Property 2", "", "p2-tooltip", QVariant {})
                .property("prop3", "Property 3", "", "p3-tooltip", QVariant {})
                .property("prop4", "Property 4", "", "p4-tooltip", QVariant {})
                .property("prop5", "Property 5", "", "p5-tooltip", QVariant {})
                .property("prop6", "Property 6", "", "p6-tooltip", QVariant {})
                .property("prop7", "Property 7", "", "p7-tooltip", QVariant {})
                .property("prop8", "Property 8", "", "p8-tooltip", QVariant {})
                .property("prop9", "Property 9", "", "p9-tooltip", QVariant {})
                .build();
  EXPECT_EQ(child_count(), 3);

  EXPECT_CALL(*object, prop1())
    .WillOnce(Return(yoyo::properties::limited_int8_t { 42, -100, 100 }));
  EXPECT_CALL(*object, prop2())
    .WillOnce(Return(yoyo::properties::limited_int16_t { -183, -2000, 0 }));
  EXPECT_CALL(*object, prop3())
    .WillOnce(Return(yoyo::properties::limited_int32_t { 50000, -10, 100000 }));
  EXPECT_CALL(*object, prop4())
    .WillOnce(Return(yoyo::properties::limited_int64_t { 20, -180, 5000000000 }));
  EXPECT_CALL(*object, prop5()).WillOnce(Return(yoyo::properties::limited_uint8_t { 5, 1, 10 }));
  EXPECT_CALL(*object, prop6())
    .WillOnce(Return(yoyo::properties::limited_uint16_t { 200, 100, 500 }));
  EXPECT_CALL(*object, prop7())
    .WillOnce(Return(yoyo::properties::limited_uint32_t { 70, 10, 100 }));
  EXPECT_CALL(*object, prop8())
    .WillOnce(Return(yoyo::properties::limited_uint64_t { 2000, 1000, 5000000000 }));
  EXPECT_CALL(*object, prop9()).WillOnce(Return(yoyo::properties::limited_float_t { 0.7, -1, 1 }));
  object->setName({ "mockObject1", true });

  _widget->itemSelected(object, docu);
  QApplication::processEvents();

  EXPECT_EQ(child_count(), 5 + 2 * 9);
  auto item_count = 1;
  // first row: type
  {
    auto label = dynamic_cast<QLabel*>(get_child(item_count++));
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "Type");
    label = dynamic_cast<QLabel*>(get_child(item_count++));
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "number_mock");
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
    EXPECT_EQ(input->text().toStdString(), "mockObject1");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("name")).toStdString());

    object->setName({ "anOtherName", true });
    EXPECT_EQ(input->text().toStdString(), "anOtherName");

    input->setText("changedName");
    QTest::keyClick(input, Qt::Key_Return);

    EXPECT_EQ(object->name()._s, "changedName");
    EXPECT_EQ(input->text().toStdString(), "changedName");
  }
  // prop1: limited_int8_t
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop1")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop1")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toInt(), 42);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop1")).toStdString());

    yoyo::properties::limited_int8_t prop { -1, -100, 100 };
    object->prop1Changed(prop);
    EXPECT_EQ(input->text().toInt(), -1);

    EXPECT_CALL(*object, prop1()).WillOnce(Return(prop));
    input->setText("-11");
    prop._value = -11;
    EXPECT_CALL(*object, set_prop1(prop));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toInt(), -11);
  }
  // prop2: limited_int16_t
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop2")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop2")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toInt(), -183);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop2")).toStdString());

    yoyo::properties::limited_int16_t prop { 1, -2000, 0 };
    object->prop2Changed(prop);
    EXPECT_EQ(input->text().toInt(), 1);

    EXPECT_CALL(*object, prop2()).WillOnce(Return(prop));
    input->setText("-125");
    prop._value = -125;
    EXPECT_CALL(*object, set_prop2(prop));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toInt(), -125);

    input->setText("1250");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop3: limited_int32_t
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop3")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop3")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "50.000");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop3")).toStdString());

    yoyo::properties::limited_int32_t prop { -200, -10, 100000 };
    object->prop3Changed(prop);
    EXPECT_EQ(input->text().toInt(), -200);

    EXPECT_CALL(*object, prop3()).WillOnce(Return(prop));
    input->setText("12567");
    prop._value = 12567;
    EXPECT_CALL(*object, set_prop3(prop));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toInt(), 12567);

    input->setText("125000");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop4: limited_int64_t
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop4")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop4")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toInt(), 20);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop4")).toStdString());

    yoyo::properties::limited_int64_t prop { 20000000, -180, 5000000000 };
    object->prop4Changed(prop);
    EXPECT_EQ(input->text().toStdString(), "20.000.000");

    EXPECT_CALL(*object, prop4()).WillOnce(Return(prop));
    input->setText("-125");
    prop._value = -125;
    EXPECT_CALL(*object, set_prop4(prop));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toInt(), -125);

    input->setText("-1250");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop5: limited_uint8_t
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop5")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop5")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toInt(), 5);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop5")).toStdString());

    yoyo::properties::limited_uint8_t prop { 1, 1, 10 };
    object->prop5Changed(prop);
    EXPECT_EQ(input->text().toInt(), 1);

    EXPECT_CALL(*object, prop5()).WillOnce(Return(prop));
    input->setText("7");
    prop._value = 7;
    EXPECT_CALL(*object, set_prop5(prop));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toInt(), 7);

    input->setText("20");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop6: limited_uint16_t
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop6")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop6")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "200");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop6")).toStdString());

    yoyo::properties::limited_uint16_t prop { 365, 100, 500 };
    object->prop6Changed(prop);
    EXPECT_EQ(input->text().toStdString(), "365");

    EXPECT_CALL(*object, prop6()).WillOnce(Return(prop));
    input->setText("250");
    prop._value = 250;
    EXPECT_CALL(*object, set_prop6(prop));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "250");

    input->setText("77");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop7: limited_uint32_t
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop7")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop7")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "70");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop7")).toStdString());

    yoyo::properties::limited_uint32_t prop { 35, 10, 100 };
    object->prop7Changed(prop);
    EXPECT_EQ(input->text().toStdString(), "35");

    EXPECT_CALL(*object, prop7()).WillOnce(Return(prop));
    input->setText("11");
    prop._value = 11;
    EXPECT_CALL(*object, set_prop7(prop));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "11");

    input->setText("5");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop8: limited_uint64_t
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop8")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop8")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "2.000");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop8")).toStdString());

    yoyo::properties::limited_uint64_t prop { 123456, 1000, 5000000000 };
    object->prop8Changed(prop);
    EXPECT_EQ(input->text().toStdString(), "123.456");

    EXPECT_CALL(*object, prop8()).WillOnce(Return(prop));
    input->setText("987654");
    prop._value = 987654;
    EXPECT_CALL(*object, set_prop8(prop));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "987654");

    input->setText("10");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop9: limited_float_t
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop9")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop9")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), QLocale::system().toString(0.7).toStdString());
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop9")).toStdString());

    yoyo::properties::limited_float_t prop { -0.5, -1, 1 };
    object->prop9Changed(prop);
    EXPECT_EQ(input->text().toStdString(), QLocale::system().toString(-0.5).toStdString());

    EXPECT_CALL(*object, prop9()).WillOnce(Return(prop));
    input->setText(QLocale::system().toString(0.001));
    prop._value = 0.001;
    EXPECT_CALL(*object, set_prop9(_)).WillOnce(Invoke([](auto v) {
      EXPECT_FLOAT_EQ(v._value, 0.001);
    }));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), QLocale::system().toString(0.001).toStdString());

    input->setText("10");
    QTest::keyClick(input, Qt::Key_Return);
  }
  testing::Mock::VerifyAndClear(object.get());
  testing::Mock::AllowLeak(object.get());
}

#include "PropertyWidgetLimitedNumberTests.moc"
