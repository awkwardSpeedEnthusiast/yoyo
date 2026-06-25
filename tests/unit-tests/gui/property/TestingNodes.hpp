#pragma once
#include "PropertyWidgetFixture.hpp"

#include "yoyo/documentation.h"
#include "yoyo/node_base.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace yoyo::gui
{
class bool_input;
class connected_boolean_input;
class connected_string_input;
class connection_in_input;
class connection_input;
class connection_out_input;
class enum_input;
class enumeration_input;
class invisible_string_input;
class layout_input;
class number_input;
class pattern_string_input;
class property_input;
class qstring_input;
class script_input;
class text_input;
} // namespace yoyo::gui

class QLineEdit;
class QComboBox;
class QCheckBox;

namespace yoyo::test
{

class node_mock_QString : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(QString prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = QString;
  using propGui = yoyo::gui::qstring_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(QString, prop);
};

class node_mock_bool : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(bool prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = bool;
  using mock_node::mock_node;
  using propGui = yoyo::gui::bool_input;
  using input_t = QCheckBox;

  DECLARE_PROPERTY(bool, prop);
};

class node_mock_char : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(char prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = char;
  using mock_node::mock_node;
  using propGui = yoyo::gui::number_input;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(char, prop);
};

class node_mock_int : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(int prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = int;
  using mock_node::mock_node;
  using propGui = yoyo::gui::number_input;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(int, prop);
};

class node_mock_short : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(short prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = short;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(short, prop);
};

class node_mock_long : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(long prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = long;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(long, prop);
};

class node_mock_qlonglong : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(qlonglong prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = qlonglong;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(qlonglong, prop);
};

class node_mock_uchar : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(uchar prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = uchar;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(uchar, prop);
};

class node_mock_ushort : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(ushort prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = ushort;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(ushort, prop);
};

class node_mock_uint : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(uint prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = uint;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(uint, prop);
};

class node_mock_ulong : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(ulong prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = ulong;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(ulong, prop);
};

class node_mock_qulonglong : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(qulonglong prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = qulonglong;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(qulonglong, prop);
};

class node_mock_float : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(float prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = float;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(float, prop);
};

class node_mock_double : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(double prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = double;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(double, prop);
};

class node_mock_uint8_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(uint8_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = uint8_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(uint8_t, prop);
};

class node_mock_uint16_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(uint16_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = uint16_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(uint16_t, prop);
};

class node_mock_uint32_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(uint32_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = uint32_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(uint32_t, prop);
};

class node_mock_uint64_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(uint64_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = uint64_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(uint64_t, prop);
};

class node_mock_int8_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(int8_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = int8_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(int8_t, prop);
};

class node_mock_int16_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(int16_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = int16_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(int16_t, prop);
};

class node_mock_int32_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(int32_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = int32_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(int32_t, prop);
};

class node_mock_int64_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(int64_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = int64_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(int64_t, prop);
};

class node_mock_access_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::types::access_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::types::access_t;
  using propGui = yoyo::gui::enumeration_input;
  using mock_node::mock_node;
  using input_t = QComboBox;

  DECLARE_PROPERTY(yoyo::types::access_t, prop);
};

class node_mock_layout_direction_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::types::layout_direction_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::types::layout_direction_t;
  using propGui = yoyo::gui::layout_input;
  using mock_node::mock_node;
  using input_t = QComboBox;

  DECLARE_PROPERTY(yoyo::types::layout_direction_t, prop);
};

class node_mock_string_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::string_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::string_t;
  using propGui = yoyo::gui::qstring_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::string_t, prop);
};

class node_mock_text_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::text_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::text_t;
  using propGui = yoyo::gui::text_input;
  using mock_node::mock_node;

  DECLARE_PROPERTY(yoyo::properties::text_t, prop);
};

class node_mock_invisible_string_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::invisible_string_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::invisible_string_t;
  using propGui = yoyo::gui::invisible_string_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::invisible_string_t, prop);
};

class node_mock_invisible_layout_direction_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(
    yoyo::properties::invisible_layout_direction_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::invisible_layout_direction_t;
  using propGui = yoyo::gui::layout_input;
  using mock_node::mock_node;
  using input_t = QComboBox;

  DECLARE_PROPERTY(yoyo::properties::invisible_layout_direction_t, prop);
};

class node_mock_connected_string_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::connected_string_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::connected_string_t;
  using propGui = yoyo::gui::connected_string_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::connected_string_t, prop);
};

class node_mock_connected_boolean_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::connected_boolean_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::connected_boolean_t;
  using propGui = yoyo::gui::connected_boolean_input;
  using mock_node::mock_node;
  using input_t = QComboBox;

  DECLARE_PROPERTY(yoyo::properties::connected_boolean_t, prop);
};

class node_mock_connection_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::connection_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::connection_t;
  using propGui = yoyo::gui::connection_input;
  using mock_node::mock_node;

  DECLARE_PROPERTY(yoyo::properties::connection_t, prop);
};

class node_mock_in_connection_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::in_connection_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::in_connection_t;
  using propGui = yoyo::gui::connection_in_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::in_connection_t, prop);
};

class node_mock_out_connection_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::out_connection_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::out_connection_t;
  using propGui = yoyo::gui::connection_out_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::out_connection_t, prop);
};

class node_mock_enum_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::enum_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::enum_t;
  using propGui = yoyo::gui::enum_input;
  using mock_node::mock_node;

  DECLARE_PROPERTY(yoyo::properties::enum_t, prop);
};

class node_mock_patterned_string_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::patterned_string_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::patterned_string_t;
  using propGui = yoyo::gui::pattern_string_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::patterned_string_t, prop);
};

class node_mock_limited_uint8_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::limited_uint8_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::limited_uint8_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::limited_uint8_t, prop);
};

class node_mock_limited_uint16_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::limited_uint16_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::limited_uint16_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::limited_uint16_t, prop);
};

class node_mock_limited_uint32_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::limited_uint32_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::limited_uint32_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::limited_uint32_t, prop);
};

class node_mock_limited_uint64_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::limited_uint64_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::limited_uint64_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::limited_uint64_t, prop);
};

class node_mock_limited_int8_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::limited_int8_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::limited_int8_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::limited_int8_t, prop);
};

class node_mock_limited_int16_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::limited_int16_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::limited_int16_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::limited_int16_t, prop);
};

class node_mock_limited_int32_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::limited_int32_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::limited_int32_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::limited_int32_t, prop);
};

class node_mock_limited_int64_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::limited_int64_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::limited_int64_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::limited_int64_t, prop);
};

class node_mock_limited_float_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::limited_float_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::limited_float_t;
  using propGui = yoyo::gui::number_input;
  using mock_node::mock_node;
  using input_t = QLineEdit;

  DECLARE_PROPERTY(yoyo::properties::limited_float_t, prop);
};

class node_mock_script_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::script_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::script_t;
  using propGui = yoyo::gui::script_input;
  using mock_node::mock_node;

  DECLARE_PROPERTY(yoyo::properties::script_t, prop);
};

class node_mock_transmission_direction_t : public mock_node
{
  Q_OBJECT

  Q_PROPERTY(
    yoyo::properties::transmission_direction_t prop READ prop WRITE set_prop NOTIFY propChanged)
public:
  using type = yoyo::properties::transmission_direction_t;
  using propGui = yoyo::gui::enumeration_input;
  using mock_node::mock_node;
  using input_t = QComboBox;

  DECLARE_PROPERTY(yoyo::properties::transmission_direction_t, prop);
};

} // namespace yoyo::test