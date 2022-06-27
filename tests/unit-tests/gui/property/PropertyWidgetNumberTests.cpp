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

class number_node_mock : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(int prop1 READ prop1 WRITE set_prop1 NOTIFY prop1Changed)
  Q_PROPERTY(int8_t prop2 READ prop2 WRITE set_prop2 NOTIFY prop2Changed)
  Q_PROPERTY(int16_t prop3 READ prop3 WRITE set_prop3 NOTIFY prop3Changed)
  Q_PROPERTY(int32_t prop4 READ prop4 WRITE set_prop4 NOTIFY prop4Changed)
  Q_PROPERTY(int64_t prop5 READ prop5 WRITE set_prop5 NOTIFY prop5Changed)
  Q_PROPERTY(uint8_t prop6 READ prop6 WRITE set_prop6 NOTIFY prop6Changed)
  Q_PROPERTY(uint16_t prop7 READ prop7 WRITE set_prop7 NOTIFY prop7Changed)
  Q_PROPERTY(uint32_t prop8 READ prop8 WRITE set_prop8 NOTIFY prop8Changed)
  Q_PROPERTY(uint64_t prop9 READ prop9 WRITE set_prop9 NOTIFY prop9Changed)
  Q_PROPERTY(char prop10 READ prop10 WRITE set_prop10 NOTIFY prop10Changed)
  Q_PROPERTY(short prop11 READ prop11 WRITE set_prop11 NOTIFY prop11Changed)
  Q_PROPERTY(long prop12 READ prop12 WRITE set_prop12 NOTIFY prop12Changed)
  Q_PROPERTY(long long prop13 READ prop13 WRITE set_prop13 NOTIFY prop13Changed)
  Q_PROPERTY(unsigned char prop14 READ prop14 WRITE set_prop14 NOTIFY prop14Changed)
  Q_PROPERTY(unsigned short prop15 READ prop15 WRITE set_prop15 NOTIFY prop15Changed)
  Q_PROPERTY(unsigned int prop16 READ prop16 WRITE set_prop16 NOTIFY prop16Changed)
  Q_PROPERTY(unsigned long prop17 READ prop17 WRITE set_prop17 NOTIFY prop17Changed)
  Q_PROPERTY(unsigned long long prop18 READ prop18 WRITE set_prop18 NOTIFY prop18Changed)
  Q_PROPERTY(float prop19 READ prop19 WRITE set_prop19 NOTIFY prop19Changed)
  Q_PROPERTY(double prop20 READ prop20 WRITE set_prop20 NOTIFY prop20Changed)
public:
  using mock_node::mock_node;

  DECLARE_PROPERTY(int, prop1);
  DECLARE_PROPERTY(int8_t, prop2);
  DECLARE_PROPERTY(int16_t, prop3);
  DECLARE_PROPERTY(int32_t, prop4);
  DECLARE_PROPERTY(int64_t, prop5);
  DECLARE_PROPERTY(uint8_t, prop6);
  DECLARE_PROPERTY(uint16_t, prop7);
  DECLARE_PROPERTY(uint32_t, prop8);
  DECLARE_PROPERTY(uint64_t, prop9);
  DECLARE_PROPERTY(char, prop10);
  DECLARE_PROPERTY(short, prop11);
  DECLARE_PROPERTY(long, prop12);
  DECLARE_PROPERTY(long long, prop13);
  DECLARE_PROPERTY(unsigned char, prop14);
  DECLARE_PROPERTY(unsigned short, prop15);
  DECLARE_PROPERTY(unsigned int, prop16);
  DECLARE_PROPERTY(unsigned long, prop17);
  DECLARE_PROPERTY(unsigned long long, prop18);
  DECLARE_PROPERTY(float, prop19);
  DECLARE_PROPERTY(double, prop20);
};

TEST_F(PropertyWidgetTest, numberProperty)
{
  createWidget();
  auto object = std::make_shared<number_node_mock>("number_mock", boost::uuids::uuid {});
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
                .property("prop10", "Property 10", "", "p10-tooltip", QVariant {})
                .property("prop11", "Property 11", "", "p11-tooltip", QVariant {})
                .property("prop12", "Property 12", "", "p12-tooltip", QVariant {})
                .property("prop13", "Property 13", "", "p13-tooltip", QVariant {})
                .property("prop14", "Property 14", "", "p14-tooltip", QVariant {})
                .property("prop15", "Property 15", "", "p15-tooltip", QVariant {})
                .property("prop16", "Property 16", "", "p16-tooltip", QVariant {})
                .property("prop17", "Property 17", "", "p17-tooltip", QVariant {})
                .property("prop18", "Property 18", "", "p18-tooltip", QVariant {})
                .build();
  EXPECT_EQ(child_count(), 3);

  EXPECT_CALL(*object, prop1()).WillOnce(Return(42));
  EXPECT_CALL(*object, prop2()).WillOnce(Return(-5));
  EXPECT_CALL(*object, prop3()).WillOnce(Return(-270));
  EXPECT_CALL(*object, prop4()).WillOnce(Return(-80000));
  EXPECT_CALL(*object, prop5()).WillOnce(Return(-80000));
  EXPECT_CALL(*object, prop6()).WillOnce(Return(1));
  EXPECT_CALL(*object, prop7()).WillOnce(Return(0x500));
  EXPECT_CALL(*object, prop8()).WillOnce(Return(0x123456));
  EXPECT_CALL(*object, prop9()).WillOnce(Return(0x12345678ab));
  EXPECT_CALL(*object, prop10()).WillOnce(Return(-1));
  EXPECT_CALL(*object, prop11()).WillOnce(Return(-510));
  EXPECT_CALL(*object, prop12()).WillOnce(Return(-80900));
  EXPECT_CALL(*object, prop13()).WillOnce(Return(-3000000));
  EXPECT_CALL(*object, prop14()).WillOnce(Return('a'));
  EXPECT_CALL(*object, prop15()).WillOnce(Return(0x1234));
  EXPECT_CALL(*object, prop16()).WillOnce(Return(0x12345678));
  EXPECT_CALL(*object, prop17()).WillOnce(Return(0x12345678));
  EXPECT_CALL(*object, prop18()).WillOnce(Return(0x12345678abcdef90));
  EXPECT_CALL(*object, prop19()).WillOnce(Return(13.75));
  EXPECT_CALL(*object, prop20()).WillOnce(Return(-1243.6));
  object->setName({ "mockObject1", true });

  _widget->itemSelected(object, docu);
  QApplication::processEvents();

  EXPECT_EQ(child_count(), 5 + 2 * 20);
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
  // prop1: int
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

    object->prop1Changed(1);
    EXPECT_EQ(input->text().toInt(), 1);

    EXPECT_CALL(*object, prop1()).WillOnce(Return(1));
    input->setText("-11");
    EXPECT_CALL(*object, set_prop1(-11));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toInt(), -11);
  }
  // prop2: int8
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
    EXPECT_EQ(input->text().toInt(), -5);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop2")).toStdString());

    object->prop2Changed(1);
    EXPECT_EQ(input->text().toInt(), 1);

    EXPECT_CALL(*object, prop2()).WillOnce(Return(1));
    input->setText("125");
    EXPECT_CALL(*object, set_prop2(125));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toInt(), 125);

    input->setText("1250");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop3: int16
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
    EXPECT_EQ(input->text().toInt(), -270);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop3")).toStdString());

    object->prop3Changed(1);
    EXPECT_EQ(input->text().toInt(), 1);

    EXPECT_CALL(*object, prop3()).WillOnce(Return(1));
    input->setText("12567");
    EXPECT_CALL(*object, set_prop3(12567));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toInt(), 12567);

    input->setText("12500000");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop4: int32
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
    EXPECT_EQ(input->text().toInt(), -80000);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop4")).toStdString());

    object->prop4Changed(1);
    EXPECT_EQ(input->text().toInt(), 1);

    EXPECT_CALL(*object, prop4()).WillOnce(Return(1));
    input->setText("-12567");
    EXPECT_CALL(*object, set_prop4(-12567));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toInt(), -12567);

    input->setText("12500000000");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop5: int64
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
    EXPECT_EQ(input->text().toInt(), -80000);
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop5")).toStdString());

    object->prop5Changed(1);
    EXPECT_EQ(input->text().toInt(), 1);

    EXPECT_CALL(*object, prop5()).WillOnce(Return(1));
    input->setText("12567");
    EXPECT_CALL(*object, set_prop5(12567));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toInt(), 12567);

    input->setText("12500000000");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop6: uint8
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
    EXPECT_EQ(input->text().toStdString(), "0x1");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop6")).toStdString());

    object->prop6Changed(11);
    EXPECT_EQ(input->text().toStdString(), "0xb");

    EXPECT_CALL(*object, prop6()).WillOnce(Return(0xb));
    input->setText("0xfe");
    EXPECT_CALL(*object, set_prop6(0xfe));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "0xfe");

    input->setText("too");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop7: uint16
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
    EXPECT_EQ(input->text().toStdString(), "0x500");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop7")).toStdString());

    object->prop7Changed(11);
    EXPECT_EQ(input->text().toStdString(), "0xb");

    EXPECT_CALL(*object, prop7()).WillOnce(Return(0xb));
    input->setText("0xfedc");
    EXPECT_CALL(*object, set_prop7(0xfedc));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "0xfedc");

    input->setText("too");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop8: uint32
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
    EXPECT_EQ(input->text().toStdString(), "0x123456");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop8")).toStdString());

    object->prop8Changed(11);
    EXPECT_EQ(input->text().toStdString(), "0xb");

    EXPECT_CALL(*object, prop8()).WillOnce(Return(0xb));
    input->setText("0xfedcba");
    EXPECT_CALL(*object, set_prop8(0xfedcba));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "0xfedcba");

    input->setText("too");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop9: uint64
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
    EXPECT_EQ(input->text().toStdString(), "0x12345678ab");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop9")).toStdString());

    object->prop9Changed(11);
    EXPECT_EQ(input->text().toStdString(), "0xb");

    EXPECT_CALL(*object, prop9()).WillOnce(Return(0xb));
    input->setText("0xfedcba0123");
    EXPECT_CALL(*object, set_prop9(0xfedcba0123));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "0xfedcba0123");

    input->setText("too");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop10: char
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop10")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop10")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "-1");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop10")).toStdString());

    object->prop10Changed(11);
    EXPECT_EQ(input->text().toStdString(), "11");

    EXPECT_CALL(*object, prop10()).WillOnce(Return(11));
    input->setText("123");
    EXPECT_CALL(*object, set_prop10(123));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "123");

    input->setText("1234");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop11: short
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop11")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop11")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "-510");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop11")).toStdString());

    object->prop11Changed(1111);
    EXPECT_EQ(input->text().toStdString(), "1.111");

    EXPECT_CALL(*object, prop11()).WillOnce(Return(1111));
    input->setText("-1235");
    EXPECT_CALL(*object, set_prop11(-1235));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "-1235");

    input->setText("12346778");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop12: long
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop12")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop12")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "-80900");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop12")).toStdString());

    object->prop12Changed(300000);
    EXPECT_EQ(input->text().toStdString(), "300.000");

    EXPECT_CALL(*object, prop12()).WillOnce(Return(300000));
    input->setText("-123456");
    EXPECT_CALL(*object, set_prop12(-123456));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "-123456");

    input->setText("123456789012345");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop13: long long
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop13")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop13")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "-3000000");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop13")).toStdString());

    object->prop13Changed(11);
    EXPECT_EQ(input->text().toStdString(), "11");

    EXPECT_CALL(*object, prop13()).WillOnce(Return(11));
    input->setText("123");
    EXPECT_CALL(*object, set_prop13(123));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "123");

    input->setText("123456789012345678901234");
    QTest::keyClick(input, Qt::Key_Return);
  }

  // prop14: unsigned char
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop14")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop14")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "0x61");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop14")).toStdString());

    object->prop14Changed(0x02);
    EXPECT_EQ(input->text().toStdString(), "0x2");

    EXPECT_CALL(*object, prop14()).WillOnce(Return(0x2));
    input->setText("0xfa");
    EXPECT_CALL(*object, set_prop14(0xfa));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "0xfa");

    input->setText("toool");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop15: unsigned short
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop15")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop15")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "0x1234");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop15")).toStdString());

    object->prop15Changed(0x1111);
    EXPECT_EQ(input->text().toStdString(), "0x1111");

    EXPECT_CALL(*object, prop15()).WillOnce(Return(0x1111));
    input->setText("0x0235");
    EXPECT_CALL(*object, set_prop15(0x0235));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "0x0235");

    input->setText("sgrt");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop16: unsigned int
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop16")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop16")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "0x12345678");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop16")).toStdString());

    object->prop16Changed(0x1111abc);
    EXPECT_EQ(input->text().toStdString(), "0x1111abc");

    EXPECT_CALL(*object, prop16()).WillOnce(Return(0x1111abc));
    input->setText("0x0235");
    EXPECT_CALL(*object, set_prop16(0x0235));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "0x0235");

    input->setText("sgrt");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop17: unsigned long
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop17")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop17")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "0x12345678");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop17")).toStdString());

    object->prop17Changed(0x1111abc);
    EXPECT_EQ(input->text().toStdString(), "0x1111abc");

    EXPECT_CALL(*object, prop17()).WillOnce(Return(0x1111abc));
    input->setText("0x0235");
    EXPECT_CALL(*object, set_prop17(0x0235));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "0x0235");

    input->setText("sgrt");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop18: unsigned long long
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop18")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop18")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), "0x12345678abcdef90");
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop18")).toStdString());

    object->prop18Changed(0x1111abc);
    EXPECT_EQ(input->text().toStdString(), "0x1111abc");

    EXPECT_CALL(*object, prop18()).WillOnce(Return(0x1111abc2));
    input->setText("0x0235");
    EXPECT_CALL(*object, set_prop18(0x0235));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), "0x0235");

    input->setText("sgrt");
    QTest::keyClick(input, Qt::Key_Return);
  }
  // prop19: float
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop19")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop19")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), QLocale::system().toString(13.75).toStdString());
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop19")).toStdString());

    object->prop19Changed(-1.1);
    EXPECT_EQ(input->text().toStdString(), QLocale::system().toString(-1.1).toStdString());

    EXPECT_CALL(*object, prop19()).WillOnce(Return(-1.1));
    input->setText(QLocale::system().toString(176.75));
    EXPECT_CALL(*object, set_prop19(176.75));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), QLocale::system().toString(176.75).toStdString());
  }
  // prop20: double
  {
    auto c1 = get_child(item_count++);
    auto c2 = get_child(item_count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop20")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop20")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::number_input"s);
    ASSERT_EQ(c1->children().size(), 2);
    auto input = dynamic_cast<QLineEdit*>(c1->children()[1]);
    ASSERT_NE(input, nullptr);
    EXPECT_EQ(input->text().toStdString(), QLocale::system().toString(-1243.6).toStdString());
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property("prop20")).toStdString());

    object->prop20Changed(-1.5);
    EXPECT_EQ(input->text().toStdString(), QLocale::system().toString(-1.5).toStdString());

    EXPECT_CALL(*object, prop20()).WillOnce(Return(-1.5));
    input->setText(QLocale::system().toString(1.75));
    EXPECT_CALL(*object, set_prop20(1.75));
    QTest::keyClick(input, Qt::Key_Return);
    EXPECT_EQ(input->text().toStdString(), QLocale::system().toString(1.75).toStdString());
  }
  testing::Mock::VerifyAndClear(object.get());
  testing::Mock::AllowLeak(object.get());
}

#include "PropertyWidgetNumberTests.moc"
