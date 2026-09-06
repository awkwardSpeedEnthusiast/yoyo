#include "json/json_property.hpp"

#include "yoyo/property_types.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <gtest/gtest.h>

using prop = yoyo::io::json::property;

TEST(JsonTest, property)
{
  EXPECT_NE(prop::io.find("QString"), prop::io.end());
  EXPECT_NE(prop::io.find("bool"), prop::io.end());
  EXPECT_NE(prop::io.find("char"), prop::io.end());
  EXPECT_NE(prop::io.find("int"), prop::io.end());
  EXPECT_NE(prop::io.find("long"), prop::io.end());
  EXPECT_NE(prop::io.find("float"), prop::io.end());
  EXPECT_NE(prop::io.find("double"), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<uint8_t>().name()), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<uint16_t>().name()), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<uint32_t>().name()), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<uint64_t>().name()), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<int8_t>().name()), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<int16_t>().name()), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<int32_t>().name()), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<int64_t>().name()), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::types::value_t>().name()), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::types::access_t>().name()), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::types::layout_direction_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::string_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::invisible_string_t>().name()),
            prop::io.end());
  EXPECT_NE(
    prop::io.find(QMetaType::fromType<yoyo::properties::invisible_layout_direction_t>().name()),
    prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::connected_string_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::connected_boolean_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::connection_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::in_connection_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::out_connection_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::enum_t>().name()), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::patterned_string_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::limited_uint8_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::limited_uint16_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::limited_uint32_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::limited_uint64_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::limited_int8_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::limited_int16_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::limited_int32_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::limited_int64_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::limited_float_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::script_t>().name()),
            prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::text_t>().name()), prop::io.end());
  EXPECT_NE(prop::io.find(QMetaType::fromType<yoyo::properties::transmission_direction_t>().name()),
            prop::io.end());
}

struct test_data {
  QString typeName;
  QVariant input;
};

class JsonTestValues : public testing::TestWithParam<test_data>
{
public:
  void expectValuesEqual(QVariant const& v1, QVariant const& v2)
  {
    ASSERT_EQ(std::string { v1.typeName() }, v2.typeName());
    static std::map<std::string,
                    std::function<void(QVariant const&, QVariant const&)>> const expecter = {
      { { "yoyo::properties::patterned_string_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::patterned_string_t>(),
                    v2.value<yoyo::properties::patterned_string_t>());
        } },
      { { "yoyo::properties::invisible_t<QString>" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::invisible_string_t>(),
                    v2.value<yoyo::properties::invisible_string_t>());
        } },
      { { "yoyo::properties::text_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::text_t>(), v2.value<yoyo::properties::text_t>())
            << v1.value<yoyo::properties::text_t>()._s.toStdString() << " "
            << v2.value<yoyo::properties::text_t>()._s.toStdString();
        } },
      { { "yoyo::properties::connected_t<QString>" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::connected_string_t>(),
                    v2.value<yoyo::properties::connected_string_t>());
        } },
      { { "yoyo::properties::connection_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::connection_t>(),
                    v2.value<yoyo::properties::connection_t>());
        } },
      { { "yoyo::properties::in_connection_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::in_connection_t>(),
                    v2.value<yoyo::properties::in_connection_t>());
        } },
      { { "yoyo::properties::out_connection_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::out_connection_t>(),
                    v2.value<yoyo::properties::out_connection_t>());
        } },
      { { "yoyo::properties::invisible_t<yoyo::types::layout_direction_t>" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::invisible_layout_direction_t>(),
                    v2.value<yoyo::properties::invisible_layout_direction_t>());
        } },
      { { "yoyo::properties::limited_value_t<uchar>" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_uint8_t>(),
                    v2.value<yoyo::properties::limited_uint8_t>());
        } },
      { { "yoyo::properties::limited_value_t<ushort>" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_uint16_t>(),
                    v2.value<yoyo::properties::limited_uint16_t>());
        } },
      { { "yoyo::properties::limited_value_t<uint>" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_uint32_t>(),
                    v2.value<yoyo::properties::limited_uint32_t>());
        } },
      { { "yoyo::properties::limited_value_t<ulong>" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_uint64_t>(),
                    v2.value<yoyo::properties::limited_uint64_t>());
        } },
      { { "yoyo::properties::limited_value_t<signed char>" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_int8_t>(),
                    v2.value<yoyo::properties::limited_int8_t>());
        } },
      { { "yoyo::properties::limited_value_t<short>" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_int16_t>(),
                    v2.value<yoyo::properties::limited_int16_t>());
        } },
      { { "yoyo::properties::limited_value_t<int>" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_int32_t>(),
                    v2.value<yoyo::properties::limited_int32_t>());
        } },
      { { "yoyo::properties::limited_value_t<long>" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_int64_t>(),
                    v2.value<yoyo::properties::limited_int64_t>());
        } },
      { { "yoyo::properties::script_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::script_t>(), v2.value<yoyo::properties::script_t>());
        } },
      { { "yoyo::properties::enum_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::enum_t>(), v2.value<yoyo::properties::enum_t>());
        } },
    };

    auto it = expecter.find(v1.typeName());

    if (it == expecter.end()) {
      EXPECT_EQ(v1, v2);
    } else {
      it->second(v1, v2);
    }
  }
};

TEST_P(JsonTestValues, properties)
{
  auto values = GetParam();
  auto v = values.input;
  ASSERT_EQ(values.typeName.toStdString(), values.input.typeName());
  auto it = prop::io.find(values.input.typeName());
  ASSERT_NE(it, prop::io.end()) << values.input.typeName() << " not found";
  auto jsonValue = it->second.first(v);
  EXPECT_FALSE(jsonValue.isNull());
  auto vs = it->second.second(jsonValue);
  EXPECT_EQ(values.typeName.toStdString(), vs.typeName());
  expectValuesEqual(v, vs);
}

INSTANTIATE_TEST_SUITE_P(
  PropertyIO, JsonTestValues,
  testing::Values(
    /*  0*/ test_data { QString { "bool" }, QVariant { true } },
    /*  1*/ test_data { QString { "bool" }, QVariant { false } },
    /*  2*/ test_data { QString { "char" }, QVariant::fromValue(std::numeric_limits<char>::min()) },
    /*  3*/ test_data { QString { "char" }, QVariant::fromValue(std::numeric_limits<char>::max()) },
    /*  4*/ test_data { QString { "char" }, QVariant::fromValue(static_cast<char>(0)) },
    /*  5*/ test_data { QString { "int" }, QVariant { 42 } },
    /*  6*/ test_data { QString { "int" }, QVariant { std::numeric_limits<int>::min() } },
    /*  7*/ test_data { QString { "int" }, QVariant { std::numeric_limits<int>::max() } },
    /*  8*/ test_data { QString { "long" }, QVariant::fromValue(static_cast<long>(-1)) },
    ///*  9*/ test_data { QString { "long" }, QVariant::fromValue(std::numeric_limits<long>::min())
    //},
    //    /* 10*/ test_data { QString { "long" },
    //    QVariant::fromValue(std::numeric_limits<long>::max()) },
    /* 11*/ test_data { QString { "signed char" }, QVariant::fromValue(static_cast<int8_t>(-42)) },
    /* 12*/
    test_data { QString { "signed char" },
                QVariant::fromValue(std::numeric_limits<int8_t>::min()) },
    /* 13*/
    test_data { QString { "signed char" },
                QVariant::fromValue(std::numeric_limits<int8_t>::max()) },
    /* 14*/ test_data { QString { "short" }, QVariant::fromValue(static_cast<int16_t>(267)) },
    /* 15*/
    test_data { QString { "short" }, QVariant::fromValue(std::numeric_limits<int16_t>::min()) },
    /* 16*/
    test_data { QString { "short" }, QVariant::fromValue(std::numeric_limits<int16_t>::max()) },
    /* 17*/ test_data { QString { "int" }, QVariant::fromValue(static_cast<int32_t>(5000000)) },
    /* 18*/
    test_data { QString { "int" }, QVariant::fromValue(std::numeric_limits<int32_t>::min()) },
    /* 19*/
    test_data { QString { "int" }, QVariant::fromValue(std::numeric_limits<int32_t>::max()) },
    /* 20*/ test_data { QString { "long" }, QVariant::fromValue(static_cast<int64_t>(-74)) },
    /* 21*/
    //    test_data { QString { "long" }, QVariant::fromValue(std::numeric_limits<int64_t>::min())
    //    },
    /* 22*/
    //    test_data { QString { "long" }, QVariant::fromValue(std::numeric_limits<int64_t>::max())
    //    },
    /* 23*/ test_data { QString { "uchar" }, QVariant::fromValue(static_cast<uint8_t>(42)) },
    /* 24*/
    test_data { QString { "uchar" }, QVariant::fromValue(std::numeric_limits<uint8_t>::min()) },
    /* 25*/
    test_data { QString { "uchar" }, QVariant::fromValue(std::numeric_limits<uint8_t>::max()) },
    /* 26*/ test_data { QString { "ushort" }, QVariant::fromValue(static_cast<uint16_t>(267)) },
    /* 27*/
    test_data { QString { "ushort" }, QVariant::fromValue(std::numeric_limits<uint16_t>::min()) },
    /* 28*/
    test_data { QString { "ushort" }, QVariant::fromValue(std::numeric_limits<uint16_t>::max()) },
    /* 29*/ test_data { QString { "uint" }, QVariant::fromValue(static_cast<uint32_t>(5000000)) },
    /* 30*/
    test_data { QString { "uint" }, QVariant::fromValue(std::numeric_limits<uint32_t>::min()) },
    /* 31*/
    test_data { QString { "uint" }, QVariant::fromValue(std::numeric_limits<uint32_t>::max()) },
    /* 32*/ test_data { QString { "ulong" }, QVariant::fromValue(static_cast<uint64_t>(74)) },
    /* 33*/
    test_data { QString { "ulong" }, QVariant::fromValue(std::numeric_limits<uint64_t>::min()) },
    /* 34*/
    test_data { QString { "ulong" }, QVariant::fromValue(std::numeric_limits<uint64_t>::max()) },
    /* 35*/ test_data { QString { "float" }, QVariant { -1.f } },
    /* 36*/ test_data { QString { "float" }, QVariant { 12442.5f } },
    /* 37*/ test_data { QString { "double" }, QVariant { -1. } },
    /* 38*/ test_data { QString { "double" }, QVariant { 12345.5 } },
    /* 39*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::BIT) },
    /* 40*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::BITFIELD) },
    /* 41*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::STRING) },
    /* 42*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::FLOAT) },
    /* 43*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::UINT8) },
    /* 44*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::UINT16) },
    /* 45*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::UINT32) },
    /* 46*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::UINT64) },
    /* 47*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::INT8) },
    /* 48*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::INT16) },
    /* 49*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::INT32) },
    /* 50*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::INT64) },
    /* 51*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::REGISTER) },
    /* 52*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::DEVICE) },
    /* 53*/
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::CUSTOM) },
    /* 54*/
    test_data { QString { "yoyo::types::access_t" },
                QVariant::fromValue(yoyo::types::access_t::NONE) },
    /* 55*/
    test_data { QString { "yoyo::types::access_t" },
                QVariant::fromValue(yoyo::types::access_t::READ) },
    /* 56*/
    test_data { QString { "yoyo::types::access_t" },
                QVariant::fromValue(yoyo::types::access_t::WRITE) },
    /* 57*/
    test_data { QString { "yoyo::types::access_t" },
                QVariant::fromValue(yoyo::types::access_t::READ_WRITE) },
    /* 58*/
    test_data { QString { "yoyo::types::layout_direction_t" },
                QVariant::fromValue(yoyo::types::layout_direction_t::FREE) },
    /* 59*/
    test_data { QString { "yoyo::types::layout_direction_t" },
                QVariant::fromValue(yoyo::types::layout_direction_t::HORIZONTAL) },
    /* 60*/
    test_data { QString { "yoyo::types::layout_direction_t" },
                QVariant::fromValue(yoyo::types::layout_direction_t::VERTICAL) },
    /* 61*/ test_data { QString { "QString" }, QVariant { QString("") } },
    /* 62*/ test_data { QString { "QString" }, QVariant { QString("foo") } },
    /* 63*/ test_data { QString { "QString" }, QVariant { QString("bar") } },
    /* 64*/
    test_data { QString { "yoyo::properties::text_t" },
                QVariant::fromValue(yoyo::properties::text_t { "" }) },
    /* 65*/
    test_data { QString { "yoyo::properties::text_t" },
                QVariant::fromValue(yoyo::properties::text_t { "foo" }) },
    /* 66*/
    test_data { QString { "yoyo::properties::text_t" },
                QVariant::fromValue(yoyo::properties::text_t { "bar" }) },
    /* 67*/
    test_data { QString { "yoyo::properties::invisible_t<QString>" },
                QVariant::fromValue(yoyo::properties::invisible_string_t { "", true }) },
    /* 68*/
    test_data { QString { "yoyo::properties::invisible_t<QString>" },
                QVariant::fromValue(yoyo::properties::invisible_string_t { "foo", true }) },
    /* 69*/
    test_data { QString { "yoyo::properties::invisible_t<QString>" },
                QVariant::fromValue(yoyo::properties::invisible_string_t { "bar", false }) },
    /* 70*/
    test_data { QString { "yoyo::properties::connected_string_t" },
                QVariant::fromValue(yoyo::properties::connected_string_t { "", "" }) },
    /* 71*/
    test_data { QString { "yoyo::properties::connected_string_t" },
                QVariant::fromValue(yoyo::properties::connected_string_t { "", "foo" }) },
    /* 72*/
    test_data { QString { "yoyo::properties::connected_string_t" },
                QVariant::fromValue(yoyo::properties::connected_string_t { "", "bar" }) },
    /* 73*/
    test_data { QString { "yoyo::properties::connected_boolean_t" },
                QVariant::fromValue(yoyo::properties::connected_boolean_t {
                  yoyo::properties::connected_boolean_t::boolean_t::TRUE, false }) },
    /* 74*/
    test_data { QString { "yoyo::properties::connected_boolean_t" },
                QVariant::fromValue(yoyo::properties::connected_boolean_t {
                  yoyo::properties::connected_boolean_t::boolean_t::FALSE, false }) },
    /* 75*/
    test_data {
      QString { "yoyo::properties::connected_boolean_t" },
      QVariant::fromValue(yoyo::properties::connected_boolean_t {
        yoyo::properties::connected_boolean_t::boolean_t::DEFINED_BY_CONNECTION, false }) },
    /* 76*/
    test_data { QString { "yoyo::properties::connection_t" },
                QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "", "", "" }) },
    /* 77*/
    test_data { QString { "yoyo::properties::connection_t" },
                QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "in", "", "" }) },
    /* 78*/
    test_data { QString { "yoyo::properties::connection_t" },
                QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "", "out", "" }) },
    /* 79*/
    test_data {
      QString { "yoyo::properties::connection_t" },
      QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "", "", "auto" }) },
    /* 80*/
    test_data {
      QString { "yoyo::properties::connection_t" },
      QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "", "bar", "foo" }) },
    /* 81*/
    test_data {
      QString { "yoyo::properties::connection_t" },
      QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "bar", "", "fuh" }) },
    /* 82*/
    test_data {
      QString { "yoyo::properties::connection_t" },
      QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "foo", "bar", "" }) },
    /* 83*/
    test_data {
      QString { "yoyo::properties::connection_t" },
      QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "foo", "fu", "bar" }) },
    /* 84*/
    test_data { QString { "yoyo::properties::in_connection_t" },
                QVariant::fromValue(yoyo::properties::in_connection_t { {}, {}, "" }) },
    /* 85*/
    test_data { QString { "yoyo::properties::in_connection_t" },
                QVariant::fromValue(yoyo::properties::in_connection_t { {}, {}, "in" }) },
    /* 86*/
    test_data { QString { "yoyo::properties::in_connection_t" },
                QVariant::fromValue(yoyo::properties::in_connection_t { {}, {}, "foo" }) },
    /* 87*/
    test_data { QString { "yoyo::properties::out_connection_t" },
                QVariant::fromValue(yoyo::properties::out_connection_t { {}, {}, "" }) },
    /* 88*/
    test_data { QString { "yoyo::properties::out_connection_t" },
                QVariant::fromValue(yoyo::properties::out_connection_t { {}, {}, "out" }) },
    /* 89*/
    test_data { QString { "yoyo::properties::out_connection_t" },
                QVariant::fromValue(yoyo::properties::out_connection_t { {}, {}, "bar" }) },
    /* 90*/
    test_data { QString { "yoyo::properties::invisible_t<yoyo::types::layout_direction_t>" },
                QVariant::fromValue(yoyo::properties::invisible_layout_direction_t {
                  yoyo::types::layout_direction_t::FREE, true }) },
    /* 91*/
    test_data { QString { "yoyo::properties::invisible_t<yoyo::types::layout_direction_t>" },
                QVariant::fromValue(yoyo::properties::invisible_layout_direction_t {
                  yoyo::types::layout_direction_t::HORIZONTAL, false }) },
    /* 92*/
    test_data { QString { "yoyo::properties::invisible_t<yoyo::types::layout_direction_t>" },
                QVariant::fromValue(yoyo::properties::invisible_layout_direction_t {
                  yoyo::types::layout_direction_t::VERTICAL, true }) },
    /* 93*/
    test_data { QString { "yoyo::properties::patterned_string_t" },
                QVariant::fromValue(yoyo::properties::patterned_string_t { "", {} }) },
    /* 94*/
    test_data { QString { "yoyo::properties::patterned_string_t" },
                QVariant::fromValue(yoyo::properties::patterned_string_t { "Foo", {} }) },
    /* 95*/
    test_data { QString { "yoyo::properties::patterned_string_t" },
                QVariant::fromValue(yoyo::properties::patterned_string_t { "bar", {} }) },
    /* 96*/
    test_data { QString { "yoyo::properties::patterned_string_t" },
                QVariant::fromValue(yoyo::properties::patterned_string_t { "auto", {} }) },
    /* 97*/
    test_data { QString { "yoyo::properties::limited_value_t<uchar>" },
                QVariant::fromValue(yoyo::properties::limited_uint8_t { 5, 0, 10 }) },
    /* 98*/
    test_data { QString { "yoyo::properties::limited_value_t<uchar>" },
                QVariant::fromValue(yoyo::properties::limited_uint8_t { 42, 10, 100 }) },
    /* 99*/
    test_data { QString { "yoyo::properties::limited_value_t<uchar>" },
                QVariant::fromValue(yoyo::properties::limited_uint8_t { 128, 0, 128 }) },
    /*100*/
    test_data { QString { "yoyo::properties::limited_value_t<uchar>" },
                QVariant::fromValue(yoyo::properties::limited_uint8_t { 255, 128, 255 }) },
    /*101*/
    test_data { QString { "yoyo::properties::limited_value_t<ushort>" },
                QVariant::fromValue(yoyo::properties::limited_uint16_t { 5, 0, 10 }) },
    /*102*/
    test_data { QString { "yoyo::properties::limited_value_t<ushort>" },
                QVariant::fromValue(yoyo::properties::limited_uint16_t { 42, 10, 100 }) },
    /*103*/
    test_data { QString { "yoyo::properties::limited_value_t<ushort>" },
                QVariant::fromValue(yoyo::properties::limited_uint16_t { 128, 0, 128 }) },
    /*104*/
    test_data { QString { "yoyo::properties::limited_value_t<ushort>" },
                QVariant::fromValue(yoyo::properties::limited_uint16_t { 65535, 128, 65535 }) },
    /*105*/
    test_data { QString { "yoyo::properties::limited_value_t<uint>" },
                QVariant::fromValue(yoyo::properties::limited_uint32_t { 5, 0, 10 }) },
    /*106*/
    test_data { QString { "yoyo::properties::limited_value_t<uint>" },
                QVariant::fromValue(yoyo::properties::limited_uint32_t { 42, 10, 100 }) },
    /*107*/
    test_data { QString { "yoyo::properties::limited_value_t<uint>" },
                QVariant::fromValue(yoyo::properties::limited_uint32_t { 128, 0, 128 }) },
    /*108*/
    test_data {
      QString { "yoyo::properties::limited_value_t<uint>" },
      QVariant::fromValue(yoyo::properties::limited_uint32_t {
        std::numeric_limits<uint32_t>::max(), 128, std::numeric_limits<uint32_t>::max() }) },
    /*109*/
    test_data { QString { "yoyo::properties::limited_value_t<ulong>" },
                QVariant::fromValue(yoyo::properties::limited_uint64_t { 5, 0, 10 }) },
    /*110*/
    test_data { QString { "yoyo::properties::limited_value_t<ulong>" },
                QVariant::fromValue(yoyo::properties::limited_uint64_t { 42, 10, 100 }) },
    /*111*/
    test_data { QString { "yoyo::properties::limited_value_t<ulong>" },
                QVariant::fromValue(yoyo::properties::limited_uint64_t { 128, 0, 128 }) },
    /*112*/
    test_data { QString { "yoyo::properties::limited_value_t<ulong>" },
                QVariant::fromValue(yoyo::properties::limited_uint64_t { 255, 128, 255 }) },
    /*113*/
    test_data { QString { "yoyo::properties::limited_value_t<signed char>" },
                QVariant::fromValue(yoyo::properties::limited_int8_t { 5, 0, 10 }) },
    /*114*/
    test_data { QString { "yoyo::properties::limited_value_t<signed char>" },
                QVariant::fromValue(yoyo::properties::limited_int8_t { -42, -100, 100 }) },
    /*115*/
    test_data { QString { "yoyo::properties::limited_value_t<signed char>" },
                QVariant::fromValue(yoyo::properties::limited_int8_t { 127, 0, 127 }) },
    /*116*/
    test_data { QString { "yoyo::properties::limited_value_t<signed char>" },
                QVariant::fromValue(yoyo::properties::limited_int8_t { -127, -127, 0 }) },
    /*117*/
    test_data { QString { "yoyo::properties::limited_value_t<short>" },
                QVariant::fromValue(yoyo::properties::limited_int16_t { 5, 0, 10 }) },
    /*118*/
    test_data { QString { "yoyo::properties::limited_value_t<short>" },
                QVariant::fromValue(yoyo::properties::limited_int16_t { -42, -110, 100 }) },
    /*119*/
    test_data { QString { "yoyo::properties::limited_value_t<short>" },
                QVariant::fromValue(yoyo::properties::limited_int16_t { 128, 0, 128 }) },
    /*120*/
    test_data { QString { "yoyo::properties::limited_value_t<short>" },
                QVariant::fromValue(yoyo::properties::limited_int16_t { 32767, -32767, 32767 }) },
    /*121*/
    test_data { QString { "yoyo::properties::limited_value_t<int>" },
                QVariant::fromValue(yoyo::properties::limited_int32_t { 5, 0, 10 }) },
    /*122*/
    test_data { QString { "yoyo::properties::limited_value_t<int>" },
                QVariant::fromValue(yoyo::properties::limited_int32_t { -42, -100, 100 }) },
    /*123*/
    test_data { QString { "yoyo::properties::limited_value_t<int>" },
                QVariant::fromValue(yoyo::properties::limited_int32_t { 128, -10, 128 }) },
    /*124*/
    test_data { QString { "yoyo::properties::limited_value_t<int>" },
                QVariant::fromValue(yoyo::properties::limited_int32_t {
                  std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min(),
                  std::numeric_limits<int32_t>::max() }) },
    /*125*/
    test_data { QString { "yoyo::properties::limited_value_t<long>" },
                QVariant::fromValue(yoyo::properties::limited_int64_t { 5, 0, 10 }) },
    /*126*/
    test_data { QString { "yoyo::properties::limited_value_t<long>" },
                QVariant::fromValue(yoyo::properties::limited_int64_t { -142, -200, -100 }) },
    /*127*/
    test_data { QString { "yoyo::properties::limited_value_t<long>" },
                QVariant::fromValue(yoyo::properties::limited_int64_t { 128, -100, 128 }) },
    /*128*/
    test_data { QString { "yoyo::properties::limited_value_t<long>" },
                QVariant::fromValue(yoyo::properties::limited_int64_t {
                  std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min(),
                  std::numeric_limits<int64_t>::max() }) },
    /*129*/
    test_data { QString { "yoyo::properties::script_t" },
                QVariant::fromValue(yoyo::properties::script_t {
                  "foo", yoyo::properties::script_t::type::FILE }) },
    /*130*/
    test_data { QString { "yoyo::properties::script_t" },
                QVariant::fromValue(yoyo::properties::script_t {
                  "bar", yoyo::properties::script_t::type::JS }) },
    /*131*/
    test_data { QString { "yoyo::properties::script_t" },
                QVariant::fromValue(yoyo::properties::script_t {
                  "Fuuu", yoyo::properties::script_t::type::NATIVE }) },
    /*132*/
    test_data { QString { "yoyo::properties::script_t" },
                QVariant::fromValue(yoyo::properties::script_t {
                  "bla", yoyo::properties::script_t::type::IMAGE }) },
    /*133*/
    test_data { QString { "yoyo::properties::transmission_direction_t" },
                QVariant::fromValue(yoyo::properties::transmission_direction_t::RX) },
    /*134*/
    test_data { QString { "yoyo::properties::transmission_direction_t" },
                QVariant::fromValue(yoyo::properties::transmission_direction_t::TX) },
    /*135*/
    test_data { QString { "yoyo::properties::enum_t" },
                QVariant::fromValue(yoyo::properties::enum_t { false, {} }) },
    /*136*/
    test_data { QString { "yoyo::properties::enum_t" },
                QVariant::fromValue(yoyo::properties::enum_t { true, {} }) },
    /*137*/
    test_data {
      QString { "yoyo::properties::enum_t" },
      QVariant::fromValue(yoyo::properties::enum_t {
        false,
        { { 0x0, { "v1", "v1", { "s1", yoyo::properties::script_t::type::FILE } } },
          { 0x3, { "v2", "v2", { "s2", yoyo::properties::script_t::type::IMAGE } } } } }) },
    /*138*/
    test_data {
      QString { "yoyo::properties::enum_t" },
      QVariant::fromValue(yoyo::properties::enum_t {
        true,
        { { 0x0, { "v1", "v1", { "s1", yoyo::properties::script_t::type::FILE } } },
          { 0x3, { "v2", "v2", { "s2", yoyo::properties::script_t::type::IMAGE } } } } }) }));
