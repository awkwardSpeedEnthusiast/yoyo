#pragma once

#include "yoyo/yoyo_sdk_global.h"

#include <QMetaType>
#include <QRegularExpression>
#include <QString>

#include <boost/signals2.hpp>

#include <map>
#include <tuple>
#include <variant>
#include <vector>

/// \file property_types.h
///
/// This file contains declarations for types to be used for properties.
///
/// Nodes can have properties (declared with the Q_PROPERTY macro), which are persisted, displayed
/// and can be modified by the user.
///
/// For a property to be displayable, it needs to have a type, which is known to yoyo. Known types
/// are:
/// - fundamental types: bool, char, short, int, long, long long, unsigned char, unsigned short,
///     unsigned int, unsigned long, unsigned long long, float, double
/// - fixed size integers: int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t
/// - QString
/// - all types defined in this file
///
/// Any other type used as property will not be persisted and cannot be displayed, even if the type
/// is published to the Qt type system.
///

namespace yoyo
{
namespace types
{
///
/// \brief The value_t enum
///
/// Possible value types of data nodes.
///
enum class value_t {
  /// Boolean
  BIT = 0,
  /// Variable length object
  BITFIELD = 1,
  /// String
  STRING = 2,
  /// Floating point value in single precision
  FLOAT = 3,
  /// 8-bit unsigned integer
  UINT8 = 4,
  /// 16-bit unsigned integer
  UINT16 = 5,
  /// 32-bit unsigned integer
  UINT32 = 6,
  /// 64-bit unsigned integer
  UINT64 = 7,
  /// 8-bit signed integer
  INT8 = 8,
  /// 16-bit signed integer
  INT16 = 9,
  /// 32-bit signed integer
  INT32 = 10,
  /// 64-bit signed integer
  INT64 = 11,
  ///
  REGISTER = 12,
  ///
  DEVICE = 13,
  /// Other values
  CUSTOM,
};

///
/// Values are moved around with two different types: a type erased variant using QVariant (like
/// std::any) and the more restricted and type-save version of std::variant.
/// This is an alias for the used std::variant.
///
using variant_t = std::variant<bool, QString, uint64_t, uint32_t, uint16_t, uint8_t, int64_t,
                               int32_t, int16_t, int8_t, float>;

///
/// \brief The access_t enum
///
/// This enum defines the possible access types for data nodes. Please note, that none is there for
/// completeness, but isn't very sensible, because there will be no data transmitted, neither back
/// nor forth.
///
enum class access_t {
  NONE,
  READ,
  WRITE,
  READ_WRITE,
};

///
/// \brief The layout_direction_t enum
///
/// This enum holds the possible layout directions of the gui nodes' widgets. In case of FREE,
/// widgets must be set manually.
///
enum class layout_direction_t {
  FREE,
  HORIZONTAL,
  VERTICAL,
};
} // namespace types
///
/// \namespace properties
///
/// Here we define the types to be used as properties by node classes to enable more sophisticated
/// behavior when displaying or editing them.
///
namespace properties
{
///
/// An alias for QStrings
///
using string_t = QString;

///
/// \brief an object for longer strings with line breaks
///
/// This class is dedicated to identify properties which need to be edited in a text-field instead
/// of a line-edit.
///
struct text_t {
  ///
  /// \brief the displayed text
  ///
  QString _s;

  explicit text_t(QString const& s)
    : _s { s }
  {
  }
  text_t() = default;
  ~text_t() = default;
  text_t(text_t const&) = default;
  text_t(text_t&&) = default;
  text_t& operator=(text_t const&) = default;
  text_t& operator=(text_t&&) = default;
  auto operator==(text_t const& other) const
  {
    return _s == other._s;
  };
  auto operator!=(text_t const& other) const
  {
    return _s != other._s;
  };
};

///
/// \brief The invisible_t struct
///
/// This struct provides the possibility to switch off the visibility of the object in the display
/// or edit window. By default it is visible.
///
template <typename T>
struct invisible_t {
  ///
  /// \brief value
  ///
  /// The value of the property
  ///
  T _s;
  ///
  /// \brief visible
  ///
  /// This property will only been shown in gui, if _visible is true
  ///
  bool _visible { true };
  ///
  /// \brief Comparison operator
  /// \param other the object to compare to
  /// \return true if both objects are the same
  ///
  auto operator==(invisible_t<T> const& other) const -> bool
  {
    return _s == other._s && _visible == other._visible;
  }
  ///
  /// \brief Negate comparison operator
  /// \param other the object to compare to
  /// \return true if the objects are different.
  ///
  auto operator!=(invisible_t<T> const& other) const -> bool
  {
    return !(*this == other);
  }
};

///
/// \brief The connected_string_t struct
///
/// This struct allows for some properties of a gui node to use the corresponding value of the
/// connected data node (input preceedes output, if different), if one is defined.
/// An example would be the title property, which can be defined in a data node and should be
/// displayed in the connected gui node, if the gui node does not override this with the definition
/// of an own value.
///
struct connected_string_t {
  ///
  /// \brief connected value
  ///
  /// The value from the connection. This value is adjusted by the connection.
  ///
  QString _connected_value;
  ///
  /// \brief own value
  ///
  /// The value stored for the owning node. This value can be adjusted by the gui.
  ///
  QString _own_value;
  ///
  /// \brief value
  ///
  /// \return the value to be displayed by the gui node.
  ///
  auto value() -> QString
  {
    if (_own_value.isEmpty()) {
      return _connected_value;
    }

    return _own_value;
  }

  ///
  /// \brief Comparison operator
  /// \param other the object to compare to
  /// \return true if the objects are equal.
  ///
  auto operator==(connected_string_t const& other) const -> bool
  {
    return _own_value == other._own_value && _connected_value == other._connected_value;
  }
  ///
  /// \brief Negate comparison operator
  /// \param other the object to compare to
  /// \return true if the objects are not equal.
  ///
  auto operator!=(connected_string_t const& other) const -> bool
  {
    return !(*this == other);
  }
};

///
/// \brief The patterned_string_t struct
///
/// This struct allows for the string to require a pattern at input.
///
struct patterned_string_t {
  ///
  /// \brief value
  ///
  /// The actual value.
  ///
  QString _s;
  ///
  /// \brief pattern
  ///
  /// The pattern, that the value must match.
  ///
  QRegularExpression _pattern;
  patterned_string_t(QString s, QRegularExpression pattern)
    : _s { s }
    , _pattern { pattern }
  {
  }

  patterned_string_t() = default;
  ///
  /// \brief Assignment constructor
  ///
  patterned_string_t(patterned_string_t const&) = default;
  ///
  /// \brief Move constructor
  ///
  patterned_string_t(patterned_string_t&&) = default;
  ///
  /// \brief Assignment operator
  /// \param other the object to copy into this
  /// \return a reference to this object
  ///
  patterned_string_t& operator=(patterned_string_t const& other) = default;
  ///
  /// \brief Move operator
  /// \param other the object to move into this
  /// \return a reference to this object
  ///
  patterned_string_t& operator=(patterned_string_t&& other) = default;
  ///
  /// \brief assign other object's properties
  ///
  /// Due to the nature of Qt property system, const values cannot be propagated with property()/
  /// setProperty() - assignment operator is used instead of copy constructor. What should never
  /// be done is overwriting the pattern in the master copy of this object hosted in the node, so
  /// use this method here for assigning the entered values through the setter.
  /// \param other the object to assign values from.
  ///
  auto assign(patterned_string_t const& other) -> patterned_string_t&
  {
    _s = other._s;
    return *this;
  }
  ///
  /// \brief Comparison operator
  /// \param other the object to compare to,
  /// \return true if the objects are considered equal.
  ///
  auto operator==(patterned_string_t const& other) const -> bool
  {
    return _s == other._s && _pattern == other._pattern;
  }
  ///
  /// \brief Negate comparison operator
  /// \param other the object to compare to,
  /// \return true if the objects are not considered equal.
  ///
  auto operator!=(patterned_string_t const& other) const -> bool
  {
    return !(*this == other);
  }
};

///
/// \brief The connected_boolean_t struct
///
/// This struct manages properties of gui nodes like enabled, which make sense only to be true, if
/// the connected data node is write enabled.
/// If DEFINED_BY_CONNECTION is choosen, but no data node is connected, the default is TRUE.
///
struct connected_boolean_t {
  ///
  /// Defines the how the boolean property is handled with respect to the connection.
  ///
  enum class boolean_t {
    TRUE,
    FALSE,
    DEFINED_BY_CONNECTION,
  };
  ///
  /// \brief type
  ///
  /// Whether the value is enabled.
  ///
  boolean_t _type;
  ///
  /// \brief connected value, the boolean value on the other side of the connection.
  ///
  /// The value from the other side of the connection
  ///
  bool _connected_value { true };
  ///
  /// \brief value
  /// \return the resulting value.
  ///
  auto value() -> bool
  {
    switch (_type) {
    case boolean_t::TRUE: return true;
    case boolean_t::FALSE: return false;
    case boolean_t::DEFINED_BY_CONNECTION: return _connected_value;
    }

    return false;
  }

  ///
  /// \brief Comparison operator
  /// \param other the object to compare to
  /// \return true, if the objects are equal
  ///
  auto operator==(connected_boolean_t const& other) const -> bool
  {
    return _type == other._type && _connected_value == other._connected_value;
  }
  ///
  /// \brief Negate comparison operator
  /// \param other the object to compare to
  /// \return true, if the objects are not equal
  ///
  auto operator!=(connected_boolean_t const& other) const -> bool
  {
    return !(*this == other);
  }
};

///
/// \brief The connection_t struct
///
/// This type is used for connection purposes of gui nodes and specifies the path to the data
/// node(s) to be connected to the gui node for data input/output.
/// The input value will connect the notifier of the specified data node to the gui node, the
/// output value will connect the notifier of the gui node to the setter of the data node. The auto
/// value will connect both the input as well as the output.
/// Depending on the access type of the data node, it might be, that values are reset again by the
/// connection engine.
/// The connector is a const functor (to be set at creation of the property value) and is used by
/// the connection engine to make a direct connection of the gui node's value notifier and the data
/// node. This is necessary because for gui nodes - in contrast to data nodes - multiple connections
/// are allowed.
///
struct connection_t {
  ///
  /// \brief connection functor
  ///
  /// A functor for creating a connection to this slot. This is used for outgoing data, so the
  /// subscriber provided for connection will be called by the non-data node owning this property
  /// to inform connected nodes, that new values are available.
  ///
  std::function<boost::signals2::connection(std::function<void(QVariant)>)> _connector;
  ///
  /// \brief setter functor
  ///
  /// This functor will allow setting the value into the non-data node, so this is for incoming
  /// data.
  ///
  std::function<void(QVariant)> _setter;
  ///
  /// \brief supports functor
  ///
  /// This functor will, when called, return true, if the connection supports the specified value
  /// type. It is used by the connection engine to see, whether a connection between a data node
  /// and a node defining a connection is allowed.
  ///
  std::function<bool(types::value_t t)> _supports;
  ///
  /// \brief in-path
  ///
  /// The path to the node to connect for incoming data
  ///
  QString _in;
  ///
  /// \brief out-path
  ///
  /// The path to the node to connect for outgoing data
  ///
  QString _out;
  ///
  /// \brief auto-connect path
  ///
  /// If this value is set, it will override _in and _out, if the node for this path exists
  ///
  QString _auto;

  connection_t(std::function<boost::signals2::connection(std::function<void(QVariant)>)> connector,
               std::function<void(QVariant)> setter, std::function<bool(types::value_t t)> supports,
               QString in, QString out, QString auto_p)
    : _connector { connector }
    , _setter { setter }
    , _supports { supports }
    , _in { in }
    , _out { out }
    , _auto { auto_p }
  {
  }

  connection_t() = default;
  ~connection_t() = default;
  ///
  /// \brief copy constructor
  ///
  connection_t(connection_t const&) = default;
  ///
  /// \brief move constructor
  ///
  connection_t(connection_t&&) = default;
  ///
  /// \brief move operator
  ///
  /// This operator is necessary to define due to a const member.
  /// Note: the const member is not moved.
  /// \param other the value to move to this object.
  ///
  auto operator=(connection_t&& other) -> connection_t& = default;
  ///
  /// \brief assignment operator
  ///
  /// This operator is necessary to define due to a const member.
  /// Note: the const member is not copied.
  /// \param other the value to assign to this object.
  ///
  auto operator=(connection_t const& other) -> connection_t& = default;

  ///
  /// \brief comparison operator
  /// \param other the item to compare to,
  /// \return true if the two items are to be compared equal.
  ///
  auto operator==(connection_t const& other) const -> bool
  {
    return _in == other._in && _out == other._out && _auto == other._auto;
  }
  ///
  /// \brief negate comparison operator
  /// \param other the item to compare to,
  /// \return true if the tow items are not equal.
  ///
  auto operator!=(connection_t const& other) const -> bool
  {
    return !(*this == other);
  }

  ///
  /// \brief assign other object's properties
  ///
  /// Due to the nature of Qt property system, const values cannot be propagated with property()/
  /// setProperty() - assignment operator is used instead of copy constructor. What should never
  /// be done is overwriting the functors in the master copy of this object hosted in the node, so
  /// use this method here for assigning the entered values through the setter.
  /// \param other the object to assign values from.
  ///
  auto assign(connection_t const& other)
  {
    _auto = other._auto;
    _in = other._in;
    _out = other._out;
  }
};

///
/// \brief a connection property for pure data sinks
///
/// Use this property if your non-data node is a pure value sink: it can receive data, but it cannot
/// send out data (at least not via this connection).
///
struct in_connection_t {
  ///
  /// \brief setter functor
  ///
  /// This functor will allow setting the value in the non-data node
  ///
  std::function<void(QVariant)> _setter;
  ///
  /// \brief supports functor
  ///
  /// This functor will, when called, return true, if the connection supports the specified value
  /// type. It is used by the connection engine to see, whether a connection between a data node
  /// and a node defining a connection is allowed.
  ///
  std::function<bool(types::value_t t)> _supports;
  ///
  /// \brief in-path
  ///
  /// The path to the node to connect for incoming data
  ///
  QString _in;

  in_connection_t(std::function<void(QVariant)> setter,
                  std::function<bool(types::value_t t)> supports, QString in)
    : _setter { setter }
    , _supports { supports }
    , _in { in }
  {
  }
  in_connection_t() = default;
  ~in_connection_t() = default;
  ///
  /// \brief copy constructor
  ///
  in_connection_t(in_connection_t const&) = default;
  ///
  /// \brief move constructor
  ///
  in_connection_t(in_connection_t&&) = default;
  ///
  /// \brief move operator
  ///
  /// This operator is necessary to define due to a const member.
  /// Note: the const member is not moved.
  /// \param other the value to move to this object.
  ///
  auto operator=(in_connection_t&& other) -> in_connection_t& = default;
  ///
  /// \brief assignment operator
  ///
  /// This operator is necessary to define due to a const member.
  /// Note: the const member is not copied.
  /// \param other the value to assign to this object.
  ///
  auto operator=(in_connection_t const& other) -> in_connection_t& = default;
  ///
  /// \brief comparison operator
  /// \param other the item to compare to,
  /// \return true if the two items are to be compared equal.
  ///
  auto operator==(in_connection_t const& other) const -> bool
  {
    return _in == other._in;
  }
  ///
  /// \brief negate comparison operator
  /// \param other the item to compare to,
  /// \return true if the tow items are not equal.
  ///
  auto operator!=(in_connection_t const& other) const -> bool
  {
    return !(*this == other);
  }
  ///
  /// \brief assign other object's properties
  ///
  /// Due to the nature of Qt property system, const values cannot be propagated with property()/
  /// setProperty() - assignment operator is used instead of copy constructor. What should never
  /// be done is overwriting the functors in the master copy of this object hosted in the node, so
  /// use this method here for assigning the entered values through the setter.
  /// \param other the object to assign values from.
  ///
  auto assign(in_connection_t const& other)
  {
    _in = other._in;
  }
};

///
/// \brief a connection property for pure data sources
///
/// Use this property if your non-data node is a pure value source: it can send out data, but it
/// cannot receive data (at least not via this connection).
///
struct out_connection_t {
  ///
  /// \brief connection functor
  ///
  /// A functor for creating a connection to this slot. This is used for outgoing data, so the
  /// subscriber provided for connection will be called by the non-data node owning this property
  /// to inform connected nodes, that new values are available.
  ///
  std::function<boost::signals2::connection(std::function<void(QVariant)>)> _connector;
  ///
  /// \brief supports functor
  ///
  /// This functor will, when called, return true, if the connection supports the specified value
  /// type. It is used by the connection engine to see, whether a connection between a data node
  /// and a node defining a connection is allowed.
  ///
  std::function<bool(types::value_t t)> _supports;
  ///
  /// \brief out-path
  ///
  /// The path to the node to connect for outgoing data
  ///
  QString _out;

  out_connection_t(
    std::function<boost::signals2::connection(std::function<void(QVariant)>)> connector,
    std::function<bool(types::value_t t)> supports, QString out)
    : _connector { connector }
    , _supports { supports }
    , _out { out }
  {
  }

  out_connection_t() = default;
  ~out_connection_t() = default;
  ///
  /// \brief copy constructor
  ///
  out_connection_t(out_connection_t const&) = default;
  ///
  /// \brief move constructor
  ///
  out_connection_t(out_connection_t&&) = default;
  ///
  /// \brief move operator
  ///
  /// This operator is necessary to define due to a const member.
  /// Note: the const member is not moved.
  /// \param other the value to move to this object.
  ///
  auto operator=(out_connection_t&& other) -> out_connection_t& = default;
  ///
  /// \brief assignment operator
  ///
  /// This operator is necessary to define due to a const member.
  /// Note: the const member is not copied.
  /// \param other the value to assign to this object.
  ///
  auto operator=(out_connection_t const& other) -> out_connection_t& = default;
  ///
  /// \brief comparison operator
  /// \param other the item to compare to,
  /// \return true if the two items are to be compared equal.
  ///
  auto operator==(out_connection_t const& other) const -> bool
  {
    return _out == other._out;
  }
  ///
  /// \brief negate comparison operator
  /// \param other the item to compare to,
  /// \return true if the tow items are not equal.
  ///
  auto operator!=(out_connection_t const& other) const -> bool
  {
    return !(*this == other);
  }
  ///
  /// \brief assign other object's properties
  ///
  /// Due to the nature of Qt property system, const values cannot be propagated with property()/
  /// setProperty() - assignment operator is used instead of copy constructor. What should never
  /// be done is overwriting the functors in the master copy of this object hosted in the node, so
  /// use this method here for assigning the entered values through the setter.
  /// \param other the object to assign values from.
  ///
  auto assign(out_connection_t const& other)
  {
    _out = other._out;
  }
};

///
/// \brief The script_t struct
///
/// This type capsules a script, which can be executed at a given situation. This application
/// currently supports two different types of scripts: JavaScript and a native format. Additionally
/// the script can be loaded from file.
///
struct script_t {
  ///
  /// \brief The type enum
  ///
  /// Defines the type of script, which can be stored with this struct
  ///
  enum class type { FILE, JS, NATIVE, IMAGE };
  ///
  /// \brief script text
  ///
  /// The script; either a path to a file containing the script or the script itself.
  ///
  QString _text;
  ///
  /// \brief script type
  ///
  /// The type of this script.
  ///
  type _script_type;
  ///
  /// \brief comparison operator
  ///
  /// Compares this object to an other script_t
  /// \param other the item to compare to
  /// \return true if the objects are considered identical
  auto operator==(script_t const& other) const -> bool
  {
    return _script_type == other._script_type && _text == other._text;
  }
  ///
  /// \brief negate comparison operator
  ///
  /// Compares this object to an other script_t
  /// \param other the item to compare to
  /// \return true if the objects are considered to be not identical
  auto operator!=(script_t const& other) const -> bool
  {
    return !(*this == other);
  }
};

///
/// \brief The enum_t struct
///
/// This struct capsules the enum feature of data nodes with unsigned integer value type. Any
/// setting of values to these data nodes should be restricted to the values available in this
/// property, if enabled.
/// The values map's value type consists of name, title (to be displayed in gui elements) and a
/// script to be executed, whenever the data node reaches the value in question.
///
struct enum_t {
  ///
  /// \brief enabled
  ///
  /// Is the enum feature enabled?
  ///
  bool _enabled { false };
  ///
  /// \brief values
  ///
  /// Stores the possible values for the enum behavior of the node
  ///
  std::map<uint64_t, std::tuple<QString, QString, script_t>> _values;
  ///
  /// \brief comparison operator
  ///
  /// Compares this object to an other enum_t
  /// \param other the item to compare to
  /// \return true if the objects are considered identical
  auto operator==(enum_t const& other) const -> bool
  {
    return _enabled == other._enabled && _values == other._values;
  }

  enum_t(bool enabled, std::map<uint64_t, std::tuple<QString, QString, script_t>> values)
    : _enabled { enabled }
    , _values { values }
  {
  }
  enum_t() = default;
  enum_t(enum_t const&) = default;
  enum_t(enum_t&&) = default;
  ~enum_t() = default;
  enum_t& operator=(enum_t const&) = default;
  enum_t& operator=(enum_t&&) = default;
};

///
/// \brief The limited_value_t struct
///
/// This templated struct allows for properties, which should only vary within certain limits,
/// defined by the declaring node. This could be the number of bytes for a bitfield type (0-64) or
/// a percentage (0-100).
/// This type is templated to declare it once for several types. See below for the available types.
///
template <typename T>
struct limited_value_t {
  ///
  /// \brief value
  ///
  /// The actual value
  ///
  T _value;
  ///
  /// \brief min
  ///
  /// The value must be bigger or equal than this
  ///
  T _min { std::numeric_limits<T>::min() };
  ///
  /// \brief max
  ///
  /// The value must be smaller or equal than this
  ///
  T _max { std::numeric_limits<T>::max() };
  ///
  /// \brief default constructor
  ///
  limited_value_t() = default;
  ///
  /// \brief Constructor
  ///
  /// Creates a limited value explicitly specifying the values. Use this method to create an
  /// object of this type, since min and max are const.
  /// \param value the value,
  /// \param min the minimum for the value,
  /// \param max the maximum for the value.
  ///
  limited_value_t(T value, T min, T max)
    : _value { value }
    , _min { min }
    , _max { max }
  {
  }
  ~limited_value_t() = default;
  ///
  /// \brief Copy constructor
  ///
  limited_value_t(limited_value_t<T> const&) = default;
  ///
  /// \brief Move constructor
  ///
  limited_value_t(limited_value_t<T>&&) = default;
  ///
  /// \brief Comparison operator
  ///
  limited_value_t<T>& operator=(limited_value_t<T> const&) = default;
  ///
  /// \brief assign other object's properties
  ///
  /// Due to the nature of Qt property system, const values cannot be propagated with property()/
  /// setProperty() - assignment operator is used instead of copy constructor. What should never
  /// be done is overwriting the limits in the master copy of this object hosted in the node, so
  /// use this method here for assigning the entered values through the setter.
  /// \param other the object to assign values from.
  ///
  auto assign(limited_value_t<T> const& other) -> void
  {
    _value = other._value;
  }
  ///
  /// \brief Move operator
  /// \return true if the objects are different
  ///
  limited_value_t<T>& operator=(limited_value_t<T>&&) = default;

  ///
  /// \brief comparison operator
  ///
  /// Compares this object to an other limited_value_t
  /// \param other the item to compare to
  /// \return true if the objects are considered identical
  auto operator==(limited_value_t<T> const& other) const -> bool
  {
    return _value == other._value;
  }
  ///
  /// \brief negate comparison operator
  ///
  /// Compares this object to an other limited_value_t
  /// \param other the item to compare to
  /// \return true if the objects are considered to be not identical
  auto operator!=(limited_value_t<T> const& other) const -> bool
  {
    return !(*this == other);
  }
};

///
/// \brief the display format for unsigned integers
///
/// Bit representation of unsigned integers (in particular) can be displayed in various ways. Yoyo
/// offers these four options for display.
///
enum class integer_format_t {
  BINARY,
  OCTAL,
  DECIMAL,
  HEXAL,
};

///
/// \brief transmission direction for messages
///
/// Transmission of messages can come in two directions: incomming (RX) and outgoing (TX). This enum
/// defines the direction in which messages are transceived.
///
enum class transmission_direction_t {
  TX,
  RX,
};

using invisible_string_t = invisible_t<QString>;
using invisible_layout_direction_t = invisible_t<types::layout_direction_t>;
using limited_uint8_t = limited_value_t<uint8_t>;
using limited_uint16_t = limited_value_t<uint16_t>;
using limited_uint32_t = limited_value_t<uint32_t>;
using limited_uint64_t = limited_value_t<uint64_t>;
using limited_int8_t = limited_value_t<int8_t>;
using limited_int16_t = limited_value_t<int16_t>;
using limited_int32_t = limited_value_t<int32_t>;
using limited_int64_t = limited_value_t<int64_t>;
using limited_float_t = limited_value_t<float>;
} // namespace properties
///
/// \brief registerAllPropertyTypes
///
/// This method is called by the application to register the types defined in this file with the
/// Qt meta type system. Do not call it yourself, the main function will take care of it.
///
auto YOYO_SDK_SHARED_EXPORT registerAllPropertyTypes() -> void;

///
/// \brief comparison function
///
/// This function compares two values from node properties. This will explicitly include
/// casting the values, if they are of composite type, which is not natively comparable with
/// QVariant.
/// \param lhs one value,
/// \param rhs the other value,
/// \returns true if the values are equal.
///
auto YOYO_SDK_SHARED_EXPORT value_equals(QVariant const& lhs, QVariant const& rhs) -> bool;
} // namespace yoyo

Q_DECLARE_METATYPE(yoyo::types::value_t)
Q_DECLARE_METATYPE(yoyo::types::access_t)
Q_DECLARE_METATYPE(yoyo::types::variant_t)
Q_DECLARE_METATYPE(yoyo::types::layout_direction_t)

Q_DECLARE_METATYPE(yoyo::properties::string_t)
Q_DECLARE_METATYPE(yoyo::properties::invisible_string_t)
Q_DECLARE_METATYPE(yoyo::properties::invisible_layout_direction_t)
Q_DECLARE_METATYPE(yoyo::properties::connected_string_t)
Q_DECLARE_METATYPE(yoyo::properties::connected_boolean_t)
Q_DECLARE_METATYPE(yoyo::properties::connection_t)
Q_DECLARE_METATYPE(yoyo::properties::in_connection_t)
Q_DECLARE_METATYPE(yoyo::properties::out_connection_t)
Q_DECLARE_METATYPE(yoyo::properties::enum_t)
Q_DECLARE_METATYPE(yoyo::properties::patterned_string_t)
Q_DECLARE_METATYPE(yoyo::properties::limited_uint8_t)
Q_DECLARE_METATYPE(yoyo::properties::limited_uint16_t)
Q_DECLARE_METATYPE(yoyo::properties::limited_uint32_t)
Q_DECLARE_METATYPE(yoyo::properties::limited_uint64_t)
Q_DECLARE_METATYPE(yoyo::properties::limited_int8_t)
Q_DECLARE_METATYPE(yoyo::properties::limited_int16_t)
Q_DECLARE_METATYPE(yoyo::properties::limited_int32_t)
Q_DECLARE_METATYPE(yoyo::properties::limited_int64_t)
Q_DECLARE_METATYPE(yoyo::properties::limited_float_t)
Q_DECLARE_METATYPE(yoyo::properties::script_t)
Q_DECLARE_METATYPE(yoyo::properties::text_t)
Q_DECLARE_METATYPE(yoyo::properties::integer_format_t)
Q_DECLARE_METATYPE(int8_t)
Q_DECLARE_METATYPE(int16_t)
Q_DECLARE_METATYPE(int32_t)
Q_DECLARE_METATYPE(int64_t)
Q_DECLARE_METATYPE(uint8_t)
Q_DECLARE_METATYPE(uint16_t)
Q_DECLARE_METATYPE(uint32_t)
Q_DECLARE_METATYPE(uint64_t)
Q_DECLARE_METATYPE(yoyo::properties::transmission_direction_t)
