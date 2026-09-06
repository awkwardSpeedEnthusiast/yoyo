#include "PropertyWidgetFixture.hpp"
#include "TestingNodes.hpp"

#include "bool_input.hpp"
#include "connected_boolean_input.hpp"
#include "connected_string_input.hpp"
#include "connection_in_input.hpp"
#include "connection_input.hpp"
#include "connection_out_input.hpp"
#include "enum_input.hpp"
#include "enumeration_input.hpp"
#include "invisible_string_input.hpp"
#include "layout_input.hpp"
#include "number_input.hpp"
#include "pattern_string_input.hpp"
#include "property_input.hpp"
#include "qstring_input.hpp"
#include "script_input.hpp"
#include "text_input.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTest>

using namespace yoyo::test;

using testing::Return;
using testing::StrictMock;

template <typename T>
class PropertyWidgetSupportedTypesTest : public PropertyWidgetTest
{
public:
  using type = T;
  using value_t = type::type;
  using gui_t = type::propGui;
  using input_t = type::input_t;
  std::tuple<int, value_t, value_t, value_t, QString> getData()
  {
    using U = T::type;

    if constexpr (std::same_as<T, node_mock_QString>) {
      return { 2, "foo", "bar", "baz", QString {} };
    } else if constexpr (std::same_as<T, node_mock_bool>) {
      return { 2, false, true, false, "foo" };
    } else if constexpr (std::same_as<T, node_mock_char>) {
      return { 2, -1, 11, 123, "too" };
    } else if constexpr (std::same_as<T, node_mock_int>) {
      return { 2, -80000, 1, -12567, "too" };
    } else if constexpr (std::same_as<T, node_mock_short>) {
      return { 2, -510, 1111, -1235, "too" };
    } else if constexpr (std::same_as<T, node_mock_long>) {
      return { 2, -80900, 300000, -1234567, "too" };
    } else if constexpr (std::same_as<T, node_mock_qlonglong>) {
      return { 2, -3000000, 11, 123, "too" };
    } else if constexpr (std::same_as<T, node_mock_uchar>) {
      return { 2, 0x61, 0x2, 0xfa, "too" };
    } else if constexpr (std::same_as<T, node_mock_ushort>) {
      return { 2, 0x1234, 0x1111, 0x0235, "too" };
    } else if constexpr (std::same_as<T, node_mock_uint>) {
      return { 2, 0x12345678, 0x1111abc, 0x0235, "too" };
    } else if constexpr (std::same_as<T, node_mock_ulong>) {
      return { 2, 0x12345678, 0x12345678, 0x0235, "too" };
    } else if constexpr (std::same_as<T, node_mock_qulonglong>) {
      return { 2, 0x12345678abcdef90, 0x1111abc, 0x0235, "too" };
    } else if constexpr (std::same_as<T, node_mock_float>) {
      return { 2, 13.75, -1.1, 176.75, "too" };
    } else if constexpr (std::same_as<T, node_mock_double>) {
      return { 2, -1243.6, -1.5, 1.75, "too" };
    } else if constexpr (std::same_as<T, node_mock_uint8_t>) {
      return { 2, 0x1, 0x11, 0xfe, "!xg" };
    } else if constexpr (std::same_as<T, node_mock_uint16_t>) {
      return { 2, 0x500, 0x123, 0xfeab, "too" };
    } else if constexpr (std::same_as<T, node_mock_uint32_t>) {
      return { 2, 0x123456, 0xb, 0xfedcba, "too" };
    } else if constexpr (std::same_as<T, node_mock_uint64_t>) {
      return { 2, 0x12345678ab, 0xb, 0xfedcba0123, "too" };
    } else if constexpr (std::same_as<T, node_mock_int8_t>) {
      return { 2, -5, 1, 125, "!xg" };
    } else if constexpr (std::same_as<T, node_mock_int16_t>) {
      return { 2, -270, 1, 12567, "too" };
    } else if constexpr (std::same_as<T, node_mock_int32_t>) {
      return { 2, -80000, 1, -12567, "too" };
    } else if constexpr (std::same_as<T, node_mock_int64_t>) {
      return { 2, -80000, 1, 12567, "too" };
    } else if constexpr (std::same_as<T, node_mock_access_t>) {
      return { 2, value_t::READ, value_t::NONE, value_t::READ_WRITE, "foo" };
    } else if constexpr (std::same_as<T, node_mock_layout_direction_t>) {
      return { 2, value_t::FREE, value_t::HORIZONTAL, value_t::VERTICAL, "foo" };
    } else if constexpr (std::same_as<T, node_mock_string_t>) {
      return { 2, "foo", "bar", "baz", QString {} };
    } else if constexpr (std::same_as<T, node_mock_text_t>) {
    } else if constexpr (std::same_as<T, node_mock_invisible_string_t>) {
      return { 2, { "foo", true }, { "bar", true }, { "baz", true }, QString {} };
    } else if constexpr (std::same_as<T, node_mock_invisible_layout_direction_t>) {
      return { 2,
               { yoyo::types::layout_direction_t::FREE, true },
               { yoyo::types::layout_direction_t::HORIZONTAL, true },
               { yoyo::types::layout_direction_t::VERTICAL, true },
               "foo" };
    } else if constexpr (std::same_as<T, node_mock_connected_string_t>) {
      return { 2, U { "tool", "foo" }, U { "tool", "bar" }, U { "tool", "baz" }, "" };
    } else if constexpr (std::same_as<T, node_mock_connected_boolean_t>) {
      return { 3,
               { yoyo::properties::connected_boolean_t::boolean_t::TRUE, true },
               { yoyo::properties::connected_boolean_t::boolean_t::FALSE, true },
               { yoyo::properties::connected_boolean_t::boolean_t::DEFINED_BY_CONNECTION, true },
               "" };
    } else if constexpr (std::same_as<T, node_mock_connection_t>) {
    } else if constexpr (std::same_as<T, node_mock_in_connection_t>) {
      return { 2, U { [](QVariant) {}, [](yoyo::types::value_t) { return true; }, "foo" },
               U { [](QVariant) {}, [](yoyo::types::value_t) { return true; }, "bar" },
               U { [](QVariant) {}, [](yoyo::types::value_t) { return true; }, "baz" }, "" };
    } else if constexpr (std::same_as<T, node_mock_out_connection_t>) {
      std::function<boost::signals2::connection(std::function<void(QVariant)>)> connector;
      return { 2, U { connector, [](yoyo::types::value_t) { return true; }, "foo" },
               U { connector, [](yoyo::types::value_t) { return true; }, "bar" },
               U { connector, [](yoyo::types::value_t) { return true; }, "baz" }, "" };
    } else if constexpr (std::same_as<T, node_mock_enum_t>) {
    } else if constexpr (std::same_as<T, node_mock_patterned_string_t>) {
      return { 2, U { "v(i)", QRegularExpression { "[v|i]\\(.*\\)" } },
               U { "v(fooo)", QRegularExpression { "[v|i]\\(.*\\)" } },
               U { "i(3sf)", QRegularExpression { "[v|i]\\(.*\\)" } }, "" };
    } else if constexpr (std::same_as<T, node_mock_limited_uint8_t>) {
      return { 2, U { 5, 1, 10 }, U { 7, 1, 10 }, U { 2, 1, 10 }, "123" };
    } else if constexpr (std::same_as<T, node_mock_limited_uint16_t>) {
      return { 2, U { 200, 100, 500 }, U { 300, 100, 500 }, U { 499, 100, 500 }, "123" };
    } else if constexpr (std::same_as<T, node_mock_limited_uint32_t>) {
      return { 2, U { 70, 10, 100 }, U { 60, 10, 100 }, U { 80, 10, 100 }, "123" };
    } else if constexpr (std::same_as<T, node_mock_limited_uint64_t>) {
      return { 2, U { 2000, 1000, 5000000000 }, U { 4000, 1000, 5000000000 },
               U { 3000, 1000, 5000000000 }, "123" };
    } else if constexpr (std::same_as<T, node_mock_limited_int8_t>) {
      return { 2, U { 42, -100, 100 }, { 4, -100, 100 }, { -42, -100, 100 }, "123" };
    } else if constexpr (std::same_as<T, node_mock_limited_int16_t>) {
      return { 2, U { -183, -2000, 0 }, { -1803, -2000, 0 }, { -18, -2000, 0 }, "123" };
    } else if constexpr (std::same_as<T, node_mock_limited_int32_t>) {
      return { 2, U { 5000, -10, 100000 }, { 50000, -10, 100000 }, { 500, -10, 100000 }, "123" };
    } else if constexpr (std::same_as<T, node_mock_limited_int64_t>) {
      return { 2, U { 20, -180, 5000000000 }, U { -10, -180, 5000000000 },
               U { 20000, -180, 5000000000 }, "123" };
    } else if constexpr (std::same_as<T, node_mock_limited_float_t>) {
      return { 2, U { 0.7, -1, 1 }, U { 0.25, -1, 1 }, U { 0.75, -1, 1 }, "123" };
    } else if constexpr (std::same_as<T, node_mock_script_t>) {
    } else if constexpr (std::same_as<T, node_mock_transmission_direction_t>) {
      return { 2, value_t::TX, value_t::RX, value_t::TX, "foo" };
    }
    throw std::runtime_error("type not supported");
  }

  auto toString(value_t const& v) -> QString
  {
    if constexpr (std::same_as<T, node_mock_QString> || std::same_as<T, node_mock_string_t>) {
      return v;
    } else if constexpr (std::same_as<T, node_mock_bool>) {
      return v ? "true" : "false";
    } else if constexpr (std::same_as<T, node_mock_char> || std::same_as<T, node_mock_int>
                         || std::same_as<T, node_mock_short> || std::same_as<T, node_mock_long>
                         || std::same_as<T, node_mock_qlonglong> || std::same_as<T, node_mock_float>
                         || std::same_as<T, node_mock_double> || std::same_as<T, node_mock_int8_t>
                         || std::same_as<T, node_mock_int16_t> || std::same_as<T, node_mock_int32_t>
                         || std::same_as<T, node_mock_int64_t>) {
      return QLocale::system().toString(v);
    } else if constexpr (std::same_as<T, node_mock_uchar> || std::same_as<T, node_mock_ushort>
                         || std::same_as<T, node_mock_uint> || std::same_as<T, node_mock_ulong>
                         || std::same_as<T, node_mock_qulonglong>
                         || std::same_as<T, node_mock_uint8_t>
                         || std::same_as<T, node_mock_uint16_t>
                         || std::same_as<T, node_mock_uint32_t>
                         || std::same_as<T, node_mock_uint64_t>) {
      return "0x" + QString::number(v, 16);
    } else if constexpr (std::same_as<T, node_mock_limited_int8_t>
                         || std::same_as<T, node_mock_limited_int16_t>
                         || std::same_as<T, node_mock_limited_int32_t>
                         || std::same_as<T, node_mock_limited_int64_t>
                         || std::same_as<T, node_mock_limited_float_t>) {
      return QLocale::system().toString(v._value);
    } else if constexpr (std::same_as<T, node_mock_limited_uint8_t>
                         || std::same_as<T, node_mock_limited_uint16_t>
                         || std::same_as<T, node_mock_limited_uint32_t>
                         || std::same_as<T, node_mock_limited_uint64_t>) {
      return "0x" + QString::number(v._value, 16);
    } else if constexpr (std::same_as<T, node_mock_text_t>
                         || std::same_as<T, node_mock_invisible_string_t>) {
      return v._s;
    } else if constexpr (std::same_as<T, node_mock_access_t>) {
      static std::map<value_t, QString> values {
        { value_t::NONE, "None" },
        { value_t::READ, "Read" },
        { value_t::WRITE, "Write" },
        { value_t::READ_WRITE, "Read-Write" },
      };
      if (auto it = values.find(v); it != values.end()) {
        return it->second;
      }
    } else if constexpr (std::same_as<T, node_mock_layout_direction_t>) {
      static std::map<yoyo::types::layout_direction_t, QString> values {
        { yoyo::types::layout_direction_t::FREE, "Floating" },
        { yoyo::types::layout_direction_t::HORIZONTAL, "Horizontal" },
        { yoyo::types::layout_direction_t::VERTICAL, "Vertical" },
      };
      if (auto it = values.find(v); it != values.end()) {
        return it->second;
      }

    } else if constexpr (std::same_as<T, node_mock_invisible_layout_direction_t>) {
      static std::map<yoyo::types::layout_direction_t, QString> values {
        { yoyo::types::layout_direction_t::FREE, "Floating" },
        { yoyo::types::layout_direction_t::HORIZONTAL, "Horizontal" },
        { yoyo::types::layout_direction_t::VERTICAL, "Vertical" },
      };
      if (auto it = values.find(v._s); it != values.end()) {
        return it->second;
      }
    } else if constexpr (std::same_as<T, node_mock_connected_string_t>) {
      return v._own_value;
    } else if constexpr (std::same_as<T, node_mock_connected_boolean_t>) {
      static std::map<yoyo::properties::connected_boolean_t::boolean_t, QString> values {
        { yoyo::properties::connected_boolean_t::boolean_t::TRUE, "True" },
        { yoyo::properties::connected_boolean_t::boolean_t::FALSE, "False" },
        { yoyo::properties::connected_boolean_t::boolean_t::DEFINED_BY_CONNECTION,
          "Defined by connection" },
      };
      if (auto it = values.find(v._type); it != values.end()) {
        return it->second;
      }

    } else if constexpr (std::same_as<T, node_mock_connection_t>) {
    } else if constexpr (std::same_as<T, node_mock_in_connection_t>) {
      return v._in;
    } else if constexpr (std::same_as<T, node_mock_out_connection_t>) {
      return v._out;
    } else if constexpr (std::same_as<T, node_mock_enum_t>) {
    } else if constexpr (std::same_as<T, node_mock_patterned_string_t>) {
      return v._s;
    } else if constexpr (std::same_as<T, node_mock_script_t>) {
    } else if constexpr (std::same_as<T, node_mock_transmission_direction_t>) {
      static std::map<value_t, QString> values {
        { value_t::TX, "Tx" },
        { value_t::RX, "Rx" },
      };
      if (auto it = values.find(v); it != values.end()) {
        return it->second;
      }
    }
    return "";
  }
};

TYPED_TEST_SUITE_P(PropertyWidgetSupportedTypesTest);

TYPED_TEST_P(PropertyWidgetSupportedTypesTest, supportedTypeProperty)
{
  this->createWidget();
  auto [childCount, initialValue, validValue, validValue2, invalidText] = this->getData();
  QString initialText = this->toString(initialValue);
  QString validText = this->toString(validValue);
  QString validInput = this->toString(validValue2);
  QString propertyName = "prop";

  auto object = std::make_shared<StrictMock<TypeParam>>("myMockNode", boost::uuids::uuid {});
  auto docu = yoyo::documentation::builder { "Mock numbers" }
                .property("name", "Name", "", "Name-tooltip", QVariant {})
                .property("prop", "Test Property", "", "Property-tooltip", QVariant {})
                .build();

  {
    EXPECT_EQ(this->child_count(), 3);

    object->setName({ "mockObject1", true });
    EXPECT_CALL(*object, prop()).WillOnce(Return(initialValue));

    this->_widget->itemSelected(object, docu);
    QApplication::processEvents();

    EXPECT_EQ(this->child_count(), 5 + 2);
  }
  testing::InSequence seq;

  // check widgets

  auto c1 = this->get_child(5);
  auto c2 = this->get_child(6);
  auto label = dynamic_cast<QLabel*>(c2);
  ASSERT_NE(label, nullptr);
  EXPECT_EQ(label->text().toStdString(), std::get<0>(docu->property(propertyName)).toStdString());
  EXPECT_EQ(label->toolTip().toStdString(),
            std::get<2>(docu->property(propertyName)).toStdString());
  ASSERT_EQ(c1->metaObject()->className(), TypeParam::propGui::staticMetaObject.className());
  ASSERT_EQ(c1->children().size(), 2);
  auto input = dynamic_cast<typename TypeParam::input_t*>(c1->children()[1]);
  ASSERT_NE(input, nullptr);

  // initial value
  {
    if constexpr (std::same_as<typename TypeParam::input_t, QLineEdit>) {
      EXPECT_EQ(input->text().toStdString(), initialText.toStdString());
    } else if constexpr (std::same_as<typename TypeParam::input_t, QComboBox>) {
      EXPECT_EQ(input->currentText().toStdString(), initialText.toStdString());
    } else if constexpr (std::same_as<typename TypeParam::input_t, QCheckBox>) {
      EXPECT_EQ(input->isChecked(), initialValue);
    }
    EXPECT_EQ(input->parentWidget()->toolTip().toStdString(),
              std::get<2>(docu->property(propertyName)).toStdString());

    testing::Mock::VerifyAndClearExpectations(object.get());
    EXPECT_CALL(*object, prop()).Times(testing::AtMost(1)).WillOnce(Return(validValue));
    EXPECT_CALL(*object, set_prop(validValue)).Times(testing::AtMost(1));
  }
  // change dataModel
  {
    object->propChanged(validValue);
    if constexpr (std::same_as<typename TypeParam::input_t, QLineEdit>) {
      QTest::keyClick(input, Qt::Key_Return);
      EXPECT_EQ(input->text().toStdString(), validText.toStdString());
    } else if constexpr (std::same_as<typename TypeParam::input_t, QComboBox>) {
      EXPECT_EQ(input->currentText().toStdString(), validText.toStdString());
    } else if constexpr (std::same_as<typename TypeParam::input_t, QCheckBox>) {
      EXPECT_EQ(input->isChecked(), validValue);
    }
    testing::Mock::VerifyAndClearExpectations(object.get());
  }
  // change in widget
  {
    if constexpr (std::same_as<typename TypeParam::input_t, QLineEdit>) {
      input->setText(validInput);
    } else if constexpr (std::same_as<typename TypeParam::input_t, QComboBox>) {
    }
    EXPECT_CALL(*object, prop()).WillOnce(Return(validValue2));
    EXPECT_CALL(*object, set_prop(validValue2));
    if constexpr (std::same_as<typename TypeParam::input_t, QLineEdit>) {
      QTest::keyClick(input, Qt::Key_Return);
      EXPECT_EQ(input->text().toStdString(), validInput.toStdString());
    } else if constexpr (std::same_as<typename TypeParam::input_t, QComboBox>) {
      input->setCurrentText(validInput);
      EXPECT_EQ(input->currentText().toStdString(), validInput.toStdString());
    } else if constexpr (std::same_as<typename TypeParam::input_t, QCheckBox>) {
      QTest::mouseClick(input, Qt::LeftButton, {}, QPoint { 5, 5 });
      EXPECT_EQ(input->isChecked(), validValue2);
    }
  }
}

REGISTER_TYPED_TEST_SUITE_P(PropertyWidgetSupportedTypesTest, supportedTypeProperty);

using supportedPropertyTypes = testing::Types<
  node_mock_QString, node_mock_char, node_mock_int, node_mock_short, node_mock_long,
  node_mock_uchar, node_mock_ushort, node_mock_uint, node_mock_ulong, node_mock_qulonglong,
  node_mock_float, node_mock_double, node_mock_uint8_t, node_mock_uint16_t, node_mock_uint32_t,
  node_mock_uint64_t, node_mock_int16_t, node_mock_int32_t, node_mock_int64_t,
  node_mock_limited_uint8_t, node_mock_limited_uint16_t, node_mock_limited_uint32_t,
  node_mock_limited_uint64_t, node_mock_limited_int8_t, node_mock_limited_int16_t,
  node_mock_limited_int32_t, node_mock_limited_int64_t, node_mock_limited_float_t,
  node_mock_access_t, node_mock_transmission_direction_t, node_mock_layout_direction_t,
  // node_mock_invisible_layout_direction_t, node_mock_invisible_string_t,
  node_mock_connected_boolean_t, node_mock_patterned_string_t, node_mock_in_connection_t,
  node_mock_out_connection_t, node_mock_bool, node_mock_connected_string_t>;

// these have own sub widgets and need to be tested differently
// node_mock_text_t,  node_mock_connection_t, node_mock_enum_t, node_mock_script_t

// failing node_mock_int8_t, node_mock_qlonglong, node_mock_string_t,
INSTANTIATE_TYPED_TEST_SUITE_P(my, PropertyWidgetSupportedTypesTest, supportedPropertyTypes);
