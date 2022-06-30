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
  EXPECT_NE(prop::io.find("uint8_t"), prop::io.end());
  EXPECT_NE(prop::io.find("uint16_t"), prop::io.end());
  EXPECT_NE(prop::io.find("uint32_t"), prop::io.end());
  EXPECT_NE(prop::io.find("uint64_t"), prop::io.end());
  EXPECT_NE(prop::io.find("int8_t"), prop::io.end());
  EXPECT_NE(prop::io.find("int16_t"), prop::io.end());
  EXPECT_NE(prop::io.find("int32_t"), prop::io.end());
  EXPECT_NE(prop::io.find("int64_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::types::value_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::types::access_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::types::layout_direction_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::string_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::invisible_string_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::invisible_layout_direction_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::connected_string_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::connected_boolean_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::connection_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::in_connection_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::out_connection_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::enum_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::patterned_string_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::limited_uint8_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::limited_uint16_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::limited_uint32_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::limited_uint64_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::limited_int8_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::limited_int16_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::limited_int32_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::limited_int64_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::limited_float_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::script_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::text_t"), prop::io.end());
  EXPECT_NE(prop::io.find("yoyo::properties::transmission_direction_t"), prop::io.end());
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
      { { "yoyo::properties::invisible_string_t" },
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
      { { "yoyo::properties::connected_string_t" },
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
      { { "yoyo::properties::invisible_layout_direction_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::invisible_layout_direction_t>(),
                    v2.value<yoyo::properties::invisible_layout_direction_t>());
        } },
      { { "yoyo::properties::limited_uint8_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_uint8_t>(),
                    v2.value<yoyo::properties::limited_uint8_t>());
        } },
      { { "yoyo::properties::limited_uint6_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_uint16_t>(),
                    v2.value<yoyo::properties::limited_uint16_t>());
        } },
      { { "yoyo::properties::limited_uint2_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_uint32_t>(),
                    v2.value<yoyo::properties::limited_uint32_t>());
        } },
      { { "yoyo::properties::limited_uint4_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_uint64_t>(),
                    v2.value<yoyo::properties::limited_uint64_t>());
        } },
      { { "yoyo::properties::limited_int8_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_int8_t>(),
                    v2.value<yoyo::properties::limited_int8_t>());
        } },
      { { "yoyo::properties::limited_int16_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_int16_t>(),
                    v2.value<yoyo::properties::limited_int16_t>());
        } },
      { { "yoyo::properties::limited_int32_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_int32_t>(),
                    v2.value<yoyo::properties::limited_int32_t>());
        } },
      { { "yoyo::properties::limited_int64_t" },
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
  auto it = prop::io.find(values.typeName);
  ASSERT_NE(it, prop::io.end());
  auto jsonValue = it->second.first(v);
  EXPECT_FALSE(jsonValue.isNull());
  auto vs = it->second.second(jsonValue);
  EXPECT_EQ(values.typeName.toStdString(), vs.typeName());
  expectValuesEqual(v, vs);
}

INSTANTIATE_TEST_SUITE_P(
  PropertyIO, JsonTestValues,
  testing::Values(
    test_data { QString { "bool" }, QVariant { true } },
    test_data { QString { "bool" }, QVariant { false } },
    test_data { QString { "char" }, QVariant::fromValue(std::numeric_limits<char>::min()) },
    test_data { QString { "char" }, QVariant::fromValue(std::numeric_limits<char>::max()) },
    test_data { QString { "char" }, QVariant::fromValue(static_cast<char>(0)) },
    test_data { QString { "int" }, QVariant { 42 } },
    test_data { QString { "int" }, QVariant { std::numeric_limits<int>::min() } },
    test_data { QString { "int" }, QVariant { std::numeric_limits<int>::max() } },
    test_data { QString { "long" }, QVariant::fromValue(static_cast<long>(-1)) },
    test_data { QString { "long" }, QVariant::fromValue(std::numeric_limits<long>::min()) },
    test_data { QString { "long" }, QVariant::fromValue(std::numeric_limits<long>::max()) },
    test_data { QString { "signed char" }, QVariant::fromValue(static_cast<int8_t>(-42)) },
    test_data { QString { "signed char" },
                QVariant::fromValue(std::numeric_limits<int8_t>::min()) },
    test_data { QString { "signed char" },
                QVariant::fromValue(std::numeric_limits<int8_t>::max()) },
    test_data { QString { "short" }, QVariant::fromValue(static_cast<int16_t>(267)) },
    test_data { QString { "short" }, QVariant::fromValue(std::numeric_limits<int16_t>::min()) },
    test_data { QString { "short" }, QVariant::fromValue(std::numeric_limits<int16_t>::max()) },
    test_data { QString { "int" }, QVariant::fromValue(static_cast<int32_t>(5000000)) },
    test_data { QString { "int" }, QVariant::fromValue(std::numeric_limits<int32_t>::min()) },
    test_data { QString { "int" }, QVariant::fromValue(std::numeric_limits<int32_t>::max()) },
    test_data { QString { "qlonglong" }, QVariant::fromValue(static_cast<int64_t>(-74)) },
    test_data { QString { "qlonglong" }, QVariant::fromValue(std::numeric_limits<int64_t>::min()) },
    test_data { QString { "qlonglong" }, QVariant::fromValue(std::numeric_limits<int64_t>::max()) },
    test_data { QString { "uchar" }, QVariant::fromValue(static_cast<uint8_t>(42)) },
    test_data { QString { "uchar" }, QVariant::fromValue(std::numeric_limits<uint8_t>::min()) },
    test_data { QString { "uchar" }, QVariant::fromValue(std::numeric_limits<uint8_t>::max()) },
    test_data { QString { "ushort" }, QVariant::fromValue(static_cast<uint16_t>(267)) },
    test_data { QString { "ushort" }, QVariant::fromValue(std::numeric_limits<uint16_t>::min()) },
    test_data { QString { "ushort" }, QVariant::fromValue(std::numeric_limits<uint16_t>::max()) },
    test_data { QString { "uint" }, QVariant::fromValue(static_cast<uint32_t>(5000000)) },
    test_data { QString { "uint" }, QVariant::fromValue(std::numeric_limits<uint32_t>::min()) },
    test_data { QString { "uint" }, QVariant::fromValue(std::numeric_limits<uint32_t>::max()) },
    test_data { QString { "qulonglong" }, QVariant::fromValue(static_cast<uint64_t>(74)) },
    test_data { QString { "qulonglong" },
                QVariant::fromValue(std::numeric_limits<uint64_t>::min()) },
    test_data { QString { "qulonglong" },
                QVariant::fromValue(std::numeric_limits<uint64_t>::max()) },
    test_data { QString { "float" }, QVariant { -1.f } },
    test_data { QString { "float" }, QVariant { 12442.5f } },
    test_data { QString { "double" }, QVariant { -1. } },
    test_data { QString { "double" }, QVariant { 12345.5 } },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::BIT) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::BITFIELD) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::STRING) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::FLOAT) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::UINT8) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::UINT16) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::UINT32) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::UINT64) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::INT8) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::INT16) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::INT32) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::INT64) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::REGISTER) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::DEVICE) },
    test_data { QString { "yoyo::types::value_t" },
                QVariant::fromValue(yoyo::types::value_t::CUSTOM) },
    test_data { QString { "yoyo::types::access_t" },
                QVariant::fromValue(yoyo::types::access_t::NONE) },
    test_data { QString { "yoyo::types::access_t" },
                QVariant::fromValue(yoyo::types::access_t::READ) },
    test_data { QString { "yoyo::types::access_t" },
                QVariant::fromValue(yoyo::types::access_t::WRITE) },
    test_data { QString { "yoyo::types::access_t" },
                QVariant::fromValue(yoyo::types::access_t::READ_WRITE) },
    test_data { QString { "yoyo::types::layout_direction_t" },
                QVariant::fromValue(yoyo::types::layout_direction_t::FREE) },
    test_data { QString { "yoyo::types::layout_direction_t" },
                QVariant::fromValue(yoyo::types::layout_direction_t::HORIZONTAL) },
    test_data { QString { "yoyo::types::layout_direction_t" },
                QVariant::fromValue(yoyo::types::layout_direction_t::VERTICAL) },
    test_data { QString { "QString" }, QVariant { QString("") } },
    test_data { QString { "QString" }, QVariant { QString("foo") } },
    test_data { QString { "QString" }, QVariant { QString("bar") } },
    test_data { QString { "yoyo::properties::text_t" },
                QVariant::fromValue(yoyo::properties::text_t { "" }) },
    test_data { QString { "yoyo::properties::text_t" },
                QVariant::fromValue(yoyo::properties::text_t { "foo" }) },
    test_data { QString { "yoyo::properties::text_t" },
                QVariant::fromValue(yoyo::properties::text_t { "bar" }) },
    test_data { QString { "yoyo::properties::invisible_string_t" },
                QVariant::fromValue(yoyo::properties::invisible_string_t { "", true }) },
    test_data { QString { "yoyo::properties::invisible_string_t" },
                QVariant::fromValue(yoyo::properties::invisible_string_t { "foo", true }) },
    test_data { QString { "yoyo::properties::invisible_string_t" },
                QVariant::fromValue(yoyo::properties::invisible_string_t { "bar", false }) },
    test_data { QString { "yoyo::properties::connected_string_t" },
                QVariant::fromValue(yoyo::properties::connected_string_t { "", "" }) },
    test_data { QString { "yoyo::properties::connected_string_t" },
                QVariant::fromValue(yoyo::properties::connected_string_t { "", "foo" }) },
    test_data { QString { "yoyo::properties::connected_string_t" },
                QVariant::fromValue(yoyo::properties::connected_string_t { "", "bar" }) },
    test_data { QString { "yoyo::properties::connected_boolean_t" },
                QVariant::fromValue(yoyo::properties::connected_boolean_t {
                  yoyo::properties::connected_boolean_t::boolean_t::TRUE, false }) },
    test_data { QString { "yoyo::properties::connected_boolean_t" },
                QVariant::fromValue(yoyo::properties::connected_boolean_t {
                  yoyo::properties::connected_boolean_t::boolean_t::FALSE, false }) },
    test_data {
      QString { "yoyo::properties::connected_boolean_t" },
      QVariant::fromValue(yoyo::properties::connected_boolean_t {
        yoyo::properties::connected_boolean_t::boolean_t::DEFINED_BY_CONNECTION, false }) },
    test_data { QString { "yoyo::properties::connection_t" },
                QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "", "", "" }) },
    test_data { QString { "yoyo::properties::connection_t" },
                QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "in", "", "" }) },
    test_data { QString { "yoyo::properties::connection_t" },
                QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "", "out", "" }) },
    test_data {
      QString { "yoyo::properties::connection_t" },
      QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "", "", "auto" }) },
    test_data {
      QString { "yoyo::properties::connection_t" },
      QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "", "bar", "foo" }) },
    test_data {
      QString { "yoyo::properties::connection_t" },
      QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "bar", "", "fuh" }) },
    test_data {
      QString { "yoyo::properties::connection_t" },
      QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "foo", "bar", "" }) },
    test_data {
      QString { "yoyo::properties::connection_t" },
      QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "foo", "fu", "bar" }) },
    test_data { QString { "yoyo::properties::in_connection_t" },
                QVariant::fromValue(yoyo::properties::in_connection_t { {}, {}, "" }) },
    test_data { QString { "yoyo::properties::in_connection_t" },
                QVariant::fromValue(yoyo::properties::in_connection_t { {}, {}, "in" }) },
    test_data { QString { "yoyo::properties::in_connection_t" },
                QVariant::fromValue(yoyo::properties::in_connection_t { {}, {}, "foo" }) },
    test_data { QString { "yoyo::properties::out_connection_t" },
                QVariant::fromValue(yoyo::properties::out_connection_t { {}, {}, "" }) },
    test_data { QString { "yoyo::properties::out_connection_t" },
                QVariant::fromValue(yoyo::properties::out_connection_t { {}, {}, "out" }) },
    test_data { QString { "yoyo::properties::out_connection_t" },
                QVariant::fromValue(yoyo::properties::out_connection_t { {}, {}, "bar" }) },
    test_data { QString { "yoyo::properties::invisible_layout_direction_t" },
                QVariant::fromValue(yoyo::properties::invisible_layout_direction_t {
                  yoyo::types::layout_direction_t::FREE, true }) },
    test_data { QString { "yoyo::properties::invisible_layout_direction_t" },
                QVariant::fromValue(yoyo::properties::invisible_layout_direction_t {
                  yoyo::types::layout_direction_t::HORIZONTAL, false }) },
    test_data { QString { "yoyo::properties::invisible_layout_direction_t" },
                QVariant::fromValue(yoyo::properties::invisible_layout_direction_t {
                  yoyo::types::layout_direction_t::VERTICAL, true }) },
    test_data { QString { "yoyo::properties::patterned_string_t" },
                QVariant::fromValue(yoyo::properties::patterned_string_t { "", {} }) },
    test_data { QString { "yoyo::properties::patterned_string_t" },
                QVariant::fromValue(yoyo::properties::patterned_string_t { "Foo", {} }) },
    test_data { QString { "yoyo::properties::patterned_string_t" },
                QVariant::fromValue(yoyo::properties::patterned_string_t { "bar", {} }) },
    test_data { QString { "yoyo::properties::patterned_string_t" },
                QVariant::fromValue(yoyo::properties::patterned_string_t { "auto", {} }) },
    test_data { QString { "yoyo::properties::limited_uint8_t" },
                QVariant::fromValue(yoyo::properties::limited_uint8_t { 5, 0, 10 }) },
    test_data { QString { "yoyo::properties::limited_uint8_t" },
                QVariant::fromValue(yoyo::properties::limited_uint8_t { 42, 10, 100 }) },
    test_data { QString { "yoyo::properties::limited_uint8_t" },
                QVariant::fromValue(yoyo::properties::limited_uint8_t { 128, 0, 128 }) },
    test_data { QString { "yoyo::properties::limited_uint8_t" },
                QVariant::fromValue(yoyo::properties::limited_uint8_t { 255, 128, 255 }) },
    test_data { QString { "yoyo::properties::limited_uint16_t" },
                QVariant::fromValue(yoyo::properties::limited_uint16_t { 5, 0, 10 }) },
    test_data { QString { "yoyo::properties::limited_uint16_t" },
                QVariant::fromValue(yoyo::properties::limited_uint16_t { 42, 10, 100 }) },
    test_data { QString { "yoyo::properties::limited_uint16_t" },
                QVariant::fromValue(yoyo::properties::limited_uint16_t { 128, 0, 128 }) },
    test_data { QString { "yoyo::properties::limited_uint16_t" },
                QVariant::fromValue(yoyo::properties::limited_uint16_t { 65535, 128, 65535 }) },
    test_data { QString { "yoyo::properties::limited_uint32_t" },
                QVariant::fromValue(yoyo::properties::limited_uint32_t { 5, 0, 10 }) },
    test_data { QString { "yoyo::properties::limited_uint32_t" },
                QVariant::fromValue(yoyo::properties::limited_uint32_t { 42, 10, 100 }) },
    test_data { QString { "yoyo::properties::limited_uint32_t" },
                QVariant::fromValue(yoyo::properties::limited_uint32_t { 128, 0, 128 }) },
    test_data {
      QString { "yoyo::properties::limited_uint32_t" },
      QVariant::fromValue(yoyo::properties::limited_uint32_t {
        std::numeric_limits<uint32_t>::max(), 128, std::numeric_limits<uint32_t>::max() }) },
    test_data { QString { "yoyo::properties::limited_uint64_t" },
                QVariant::fromValue(yoyo::properties::limited_uint64_t { 5, 0, 10 }) },
    test_data { QString { "yoyo::properties::limited_uint64_t" },
                QVariant::fromValue(yoyo::properties::limited_uint64_t { 42, 10, 100 }) },
    test_data { QString { "yoyo::properties::limited_uint64_t" },
                QVariant::fromValue(yoyo::properties::limited_uint64_t { 128, 0, 128 }) },
    test_data { QString { "yoyo::properties::limited_uint64_t" },
                QVariant::fromValue(yoyo::properties::limited_uint64_t { 255, 128, 255 }) },
    test_data { QString { "yoyo::properties::limited_int8_t" },
                QVariant::fromValue(yoyo::properties::limited_int8_t { 5, 0, 10 }) },
    test_data { QString { "yoyo::properties::limited_int8_t" },
                QVariant::fromValue(yoyo::properties::limited_int8_t { -42, -100, 100 }) },
    test_data { QString { "yoyo::properties::limited_int8_t" },
                QVariant::fromValue(yoyo::properties::limited_int8_t { 127, 0, 127 }) },
    test_data { QString { "yoyo::properties::limited_int8_t" },
                QVariant::fromValue(yoyo::properties::limited_int8_t { -127, -127, 0 }) },
    test_data { QString { "yoyo::properties::limited_int16_t" },
                QVariant::fromValue(yoyo::properties::limited_int16_t { 5, 0, 10 }) },
    test_data { QString { "yoyo::properties::limited_int16_t" },
                QVariant::fromValue(yoyo::properties::limited_int16_t { -42, -110, 100 }) },
    test_data { QString { "yoyo::properties::limited_int16_t" },
                QVariant::fromValue(yoyo::properties::limited_int16_t { 128, 0, 128 }) },
    test_data { QString { "yoyo::properties::limited_int16_t" },
                QVariant::fromValue(yoyo::properties::limited_int16_t { 32767, -32767, 32767 }) },
    test_data { QString { "yoyo::properties::limited_int32_t" },
                QVariant::fromValue(yoyo::properties::limited_int32_t { 5, 0, 10 }) },
    test_data { QString { "yoyo::properties::limited_int32_t" },
                QVariant::fromValue(yoyo::properties::limited_int32_t { -42, -100, 100 }) },
    test_data { QString { "yoyo::properties::limited_int32_t" },
                QVariant::fromValue(yoyo::properties::limited_int32_t { 128, -10, 128 }) },
    test_data { QString { "yoyo::properties::limited_int32_t" },
                QVariant::fromValue(yoyo::properties::limited_int32_t {
                  std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::min(),
                  std::numeric_limits<int32_t>::max() }) },
    test_data { QString { "yoyo::properties::limited_int64_t" },
                QVariant::fromValue(yoyo::properties::limited_int64_t { 5, 0, 10 }) },
    test_data { QString { "yoyo::properties::limited_int64_t" },
                QVariant::fromValue(yoyo::properties::limited_int64_t { -142, -200, -100 }) },
    test_data { QString { "yoyo::properties::limited_int64_t" },
                QVariant::fromValue(yoyo::properties::limited_int64_t { 128, -100, 128 }) },
    test_data { QString { "yoyo::properties::limited_int64_t" },
                QVariant::fromValue(yoyo::properties::limited_int64_t {
                  std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min(),
                  std::numeric_limits<int64_t>::max() }) },
    test_data { QString { "yoyo::properties::script_t" },
                QVariant::fromValue(yoyo::properties::script_t {
                  "foo", yoyo::properties::script_t::type::FILE }) },
    test_data { QString { "yoyo::properties::script_t" },
                QVariant::fromValue(yoyo::properties::script_t {
                  "bar", yoyo::properties::script_t::type::JS }) },
    test_data { QString { "yoyo::properties::script_t" },
                QVariant::fromValue(yoyo::properties::script_t {
                  "Fuuu", yoyo::properties::script_t::type::NATIVE }) },
    test_data { QString { "yoyo::properties::script_t" },
                QVariant::fromValue(yoyo::properties::script_t {
                  "bla", yoyo::properties::script_t::type::IMAGE }) },
    test_data { QString { "yoyo::properties::transmission_direction_t" },
                QVariant::fromValue(yoyo::properties::transmission_direction_t::RX) },
    test_data { QString { "yoyo::properties::transmission_direction_t" },
                QVariant::fromValue(yoyo::properties::transmission_direction_t::TX) },
    test_data { QString { "yoyo::properties::enum_t" },
                QVariant::fromValue(yoyo::properties::enum_t { false, {} }) },
    test_data { QString { "yoyo::properties::enum_t" },
                QVariant::fromValue(yoyo::properties::enum_t { true, {} }) },
    test_data {
      QString { "yoyo::properties::enum_t" },
      QVariant::fromValue(yoyo::properties::enum_t {
        false,
        { { 0x0, { "v1", "v1", { "s1", yoyo::properties::script_t::type::FILE } } },
          { 0x3, { "v2", "v2", { "s2", yoyo::properties::script_t::type::IMAGE } } } } }) },
    test_data {
      QString { "yoyo::properties::enum_t" },
      QVariant::fromValue(yoyo::properties::enum_t {
        true,
        { { 0x0, { "v1", "v1", { "s1", yoyo::properties::script_t::type::FILE } } },
          { 0x3, { "v2", "v2", { "s2", yoyo::properties::script_t::type::IMAGE } } } } }) }));
