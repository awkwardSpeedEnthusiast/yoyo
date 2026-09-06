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

  Q_PROPERTY(yoyo::properties::text_t prop5 READ prop5 WRITE setProp5 NOTIFY prop5Changed)
public:
  using mock_node::mock_node;

  MOCK_METHOD(yoyo::properties::text_t, prop5, (), (const));
  MOCK_METHOD(void, setProp5, (yoyo::properties::text_t v));

Q_SIGNALS:
  void prop5Changed(yoyo::properties::text_t v);
};

TEST_F(PropertyWidgetTest, stringProperty)
{
  createWidget();
  auto object = std::make_shared<string_node_mock>("string_mock", boost::uuids::uuid {});
  auto docu = yoyo::documentation::builder { "Mock string" }
                .property("name", "Name", "", "Name-tooltip", QVariant {})
                .property("prop5", "Property 5", "", "p5-tooltip", QVariant {})
                .build();
  EXPECT_EQ(child_count(), 3);

  yoyo::properties::text_t prop5 { "a\nlonger\ntext" };
  EXPECT_CALL(*object, prop5()).WillOnce(Return(prop5));
  object->setName({ "mockObject1", true });

  _widget->itemSelected(object, docu);
  QApplication::processEvents();

  int count = 1;
  EXPECT_EQ(child_count(), 3 + 2 * 2);

  // first row: type
  {
    auto label = dynamic_cast<QLabel*>(get_child(count++));
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "Type");
    label = dynamic_cast<QLabel*>(get_child(count++));
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), "string_mock");
  }
  // second row: name
  {
    auto c1 = get_child(count++);
    auto c2 = get_child(count++);
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
  // seventh row: prop5
  {
    auto c1 = get_child(count++);
    auto c2 = get_child(count++);
    auto label = dynamic_cast<QLabel*>(c2);
    ASSERT_NE(label, nullptr);
    EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property("prop5")).toStdString());
    EXPECT_EQ(label->toolTip().toStdString(), std::get<2>(docu->property("prop5")).toStdString());
    ASSERT_EQ(c1->metaObject()->className(), "yoyo::gui::text_input"s);
    ASSERT_EQ(c1->children().size(), 3);
    auto input = dynamic_cast<QTextEdit*>(c1->children()[1]);
    auto save = dynamic_cast<QPushButton*>(c1->children()[2]);
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
}

#include "PropertyWidgetStringsTests.moc"
