#include "property_input_factory.hpp"
#include "access_input.hpp"
#include "bool_input.hpp"
#include "connected_boolean_input.hpp"
#include "connected_string_input.hpp"
#include "connection_in_input.hpp"
#include "connection_input.hpp"
#include "connection_out_input.hpp"
#include "enum_input.hpp"
#include "invisible_string_input.hpp"
#include "layout_input.hpp"
#include "number_input.hpp"
#include "pattern_string_input.hpp"
#include "property_input.hpp"
#include "qstring_input.hpp"
#include "script_input.hpp"
#include "text_input.hpp"

namespace yoyo::gui
{
std::map<std::string, property::creator> const property::factory {
  { "QString",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::qstring_input>(n, p, parent);
    } },
  { "bool", [](auto n, auto p,
               auto parent) { return std::make_shared<yoyo::gui::bool_input>(n, p, parent); } },
  { "char", [](auto n, auto p,
               auto parent) { return std::make_shared<yoyo::gui::number_input>(n, p, parent); } },
  { "int", [](auto n, auto p,
              auto parent) { return std::make_shared<yoyo::gui::number_input>(n, p, parent); } },
  { "short", [](auto n, auto p,
                auto parent) { return std::make_shared<yoyo::gui::number_input>(n, p, parent); } },
  { "long", [](auto n, auto p,
               auto parent) { return std::make_shared<yoyo::gui::number_input>(n, p, parent); } },
  { "qlonglong",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "uchar", [](auto n, auto p,
                auto parent) { return std::make_shared<yoyo::gui::number_input>(n, p, parent); } },
  { "ushort", [](auto n, auto p,
                 auto parent) { return std::make_shared<yoyo::gui::number_input>(n, p, parent); } },
  { "uint", [](auto n, auto p,
               auto parent) { return std::make_shared<yoyo::gui::number_input>(n, p, parent); } },
  { "ulong", [](auto n, auto p,
                auto parent) { return std::make_shared<yoyo::gui::number_input>(n, p, parent); } },
  { "qulonglong",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "float", [](auto n, auto p,
                auto parent) { return std::make_shared<yoyo::gui::number_input>(n, p, parent); } },
  { "double", [](auto n, auto p,
                 auto parent) { return std::make_shared<yoyo::gui::number_input>(n, p, parent); } },
  { "uint8_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "uint16_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "uint32_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "uint64_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "int8_t", [](auto n, auto p,
                 auto parent) { return std::make_shared<yoyo::gui::number_input>(n, p, parent); } },
  { "int16_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "int32_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "int64_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "yoyo::types::access_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::access_input>(n, p, parent);
    } },
  { "yoyo::types::layout_direction_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::layout_input>(n, p, parent);
    } },
  { "yoyo::properties::string_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::qstring_input>(n, p, parent);
    } },
  { "yoyo::properties::text_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::text_input>(n, p, parent);
    } },
  { "yoyo::properties::invisible_string_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::invisible_string_input>(n, p, parent);
    } },
  { "yoyo::properties::invisible_layout_direction_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::layout_input>(n, p, parent);
    } },
  { "yoyo::properties::connected_string_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::connected_string_input>(n, p, parent);
    } },
  { "yoyo::properties::connected_boolean_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::connected_boolean_input>(n, p, parent);
    } },
  { "yoyo::properties::connection_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::connection_input>(n, p, parent);
    } },
  { "yoyo::properties::in_connection_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::connection_in_input>(n, p, parent);
    } },
  { "yoyo::properties::out_connection_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::connection_out_input>(n, p, parent);
    } },
  { "yoyo::properties::enum_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::enum_input>(n, p, parent);
    } },
  { "yoyo::properties::patterned_string_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::pattern_string_input>(n, p, parent);
    } },
  { "yoyo::properties::limited_uint8_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "yoyo::properties::limited_uint16_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "yoyo::properties::limited_uint32_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "yoyo::properties::limited_uint64_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "yoyo::properties::limited_int8_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "yoyo::properties::limited_int16_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "yoyo::properties::limited_int32_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "yoyo::properties::limited_int64_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "yoyo::properties::limited_float_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::number_input>(n, p, parent);
    } },
  { "yoyo::properties::script_t",
    [](auto n, auto p, auto parent) {
      return std::make_shared<yoyo::gui::script_input>(n, p, parent);
    } }
};
} // namespace yoyo::gui
