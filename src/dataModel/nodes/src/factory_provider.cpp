#include "factory_provider.hpp"

#include "configuration.hpp"
#include "configuration_data.hpp"
#include "message.hpp"
#include "message_field.hpp"

#include "dataNodes/bit_node.hpp"
#include "dataNodes/data_group.hpp"
#include "dataNodes/int8node.hpp"
#include "dataNodes/string_node.hpp"
#include "dataNodes/uint8node.hpp"
#include "data_root.hpp"

#include "basicGui/button.hpp"
#include "basicGui/check_box.hpp"
#include "basicGui/combo_box.hpp"
#include "basicGui/group.hpp"
#include "basicGui/line_edit.hpp"
#include "gui_root.hpp"

#include "yoyo/documentation_utilities.h"

namespace
{
template <typename T>
auto createDoc(QString const& name) -> yoyo::documentation::builder
{
  return yoyo::documentation::builder { name };
}

template <typename T>
auto install(yoyo::node_factory& factory, QString const& name,
             yoyo::node_factory::node_creator create,
             yoyo::node_factory::potential_child_list children) -> void
{
  yoyo::documentation::builder builder { name };
  yoyo::updateDocumentation<T>(builder);

  if (!create) {
    factory.install_node(
      T::typeId(), name, &T::staticMetaObject, builder.build(),
      [](boost::uuids::uuid id) { return std::make_shared<T>(id); }, children);
  } else {
    factory.install_node(T::typeId(), name, &T::staticMetaObject, builder.build(), create,
                         children);
  }
}
} // namespace

namespace yoyo
{
template <>
auto updateDocumentation<yoyo::group>(documentation::builder& builder) -> void
{
  updateDocumentation<gui_node>(builder);
  builder
    .description(
      "This item can host other Gui items. In display it can be used for layout purposes only"
      " or for grouping items with a frame.")
    .property(
      "layoutDirection", "Layout direction",
      "The layout direction of a layout item defines how the child items are arranged inside the"
      " item. There is three possible values: Vertical (from top to bottom, default), Horizontal"
      " (from left to right) and Free (in this case the size and position of the child items can"
      " be set with the Width/Height/X Position/Y Position properties of the children.",
      "Defines the widget layout direction. \n"
      " - Horizontal : The layout is horizontal. The child widget position, witdh and height are "
      "set automatically\n"
      " - Vertical : The layout is vertical. The child widget position, witdh and height are set "
      "automatically\n"
      " - Free : The layout floating. The position of the items is manually defined using Width, "
      "Height, X Position and Y Position",
      QVariant::fromValue(yoyo::properties::invisible_layout_direction_t {
        yoyo::types::layout_direction_t::VERTICAL, true }))
    .property(
      "title", "Title",
      "Define the title to be shown in the Gui on the frame. If it is a pure layout item, the"
      " title will not be shown.",
      "The title shown for the group.", "");
}

template <>
auto updateDocumentation<yoyo::button>(documentation::builder& builder) -> void
{
  updateDocumentation<gui_node>(builder);
  builder.description("This item represents a button, which can be clicked to perform an action.")
    .property("title", "Title", "Define the text to be dislayed on the button.",
              "The caption text of the button.", "")
    .property(
      "onClick", "On-click",
      "Use this property to define the action to be performed, when the button is clicked. As"
      " with all script objects, it can be specified to execute a native script, a Java-"
      "script script or a file containing a script. The option Image is not supported for"
      "Buttons.",
      "The script to be executed on clicking the button. Provide either a Java-script, a "
      "native script or a file for execution (Image is not supported).\nUse Enter key to "
      "finish editing and Shift-Enter for a new line.",
      "");
}

template <>
auto updateDocumentation<yoyo::check_box>(documentation::builder& builder) -> void
{
  updateDocumentation<gui_node>(builder);
  builder
    .description("This item shows as a checkable box with a title. This is the default item for"
                 " Boolean types.")
    .property(
      "title", "Title",
      "Define the text to be dislayed next to the actual CheckBox. If not set explicitly, the"
      " displayed text will be the title of the connected signal",
      "The caption text of the CheckBox. If connected, the CheckBox will use the signal's "
      "title, if this property is not set explicitly.",
      "")
    .property(
      "connection", "Connection",
      "This property holds the connection to one or more signals.\n"
      "- Data-in: defines the path to the signal providing input values to be displayed\n"
      "- Data-out: defines the path to the signal to which the values entered here will be sent\n"
      "- Auto-connect: the signal specified here will be used both for input as well as output.",
      "This property holds the connection to one or more signals.\n"
      "- Data-in: defines the path to the signal providing input values to be displayed\n"
      "- Data-out: defines the path to the signal to which the values entered here will be sent\n"
      "- Auto-connect: the signal specified here will be used both for input as well as output.",
      "");
}

template <>
auto updateDocumentation<yoyo::combo_box>(documentation::builder& builder) -> void
{
  updateDocumentation<gui_node>(builder);
  builder
    .description("This item provides a drop down menu to enable the user to select a pre-specified "
                 "option. This is the default item for unsigned types, which have the enum property"
                 "enabled.")
    .property("title", "Title",
              "Define the title test to be dislayed left of the actual ComboBox. If not set "
              "explicitly, the displayed text will be the title of the connected signal",
              "The title for the ComboBox. If connected, the ComboBox will use the signal's "
              "title, if this property is not set explicitly.",
              QVariant::fromValue(yoyo::properties::connected_string_t { "", "" }))
    .property("unit", "Unit",
              "Define the text to be dislayed right of the actual ComboBox. If not set explicitly"
              ", the displayed text will be the unit of the connected signal",
              "The unit for the ComboBox. If connected, the ComboBox will use the signal's "
              "unit, if this property is not set explicitly.",
              QVariant::fromValue(yoyo::properties::connected_string_t { "", "" }))
    .property(
      "connection", "Connection",
      "This property holds the connection to one or more signals.\n"
      "- Data-in: defines the path to the signal providing input values to be displayed\n"
      "- Data-out: defines the path to the signal to which the values entered here will be sent\n"
      "- Auto-connect: the signal specified here will be used both for input as well as output.",
      "This property holds the connection to one or more signals.\n"
      "- Data-in: defines the path to the signal providing input values to be displayed\n"
      "- Data-out: defines the path to the signal to which the values entered here will be sent\n"
      "- Auto-connect: the signal specified here will be used both for input as well as output.",
      QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "", "", "" }))
    .property(
      "enumValues", "Enum",
      "Enum defines named options for unsigned values.\nIn case of ComboBox, this allows to"
      "show a drop down menu with the names of the defined enum options. Each option comes "
      "with a value (the actually communicated integer value), a name, a title (shown in the "
      "list of the drop-down menu) and a script, which will be executed, if the displayed "
      "value changed to this value (Image option for the script is ignored for ComboBox).",
      "Define the enum display properties for values of the connected signal.\n"
      "- Enabled: enable or disable the enum functionality for this ComboBox\n"
      "- Value: the transmitted value from the signal\n"
      "- Title: displayed in the drop-down menu of the ComboBox\n"
      "- Match event: the script to execute, if this value has been is been set to the ComboBox",
      QVariant::fromValue(yoyo::properties::enum_t { false, {} }));
}

template <>
auto updateDocumentation<yoyo::line_edit>(documentation::builder& builder) -> void
{
  updateDocumentation<gui_node>(builder);
  builder
    .description(
      "This item provides a single line to edit its value. Depending on the type connected, a"
      " validator can be in place, which restricts characters which can be entered. This is "
      "the default object for all non-Boolean types.")
    .property("title", "Title",
              "Define the title test to be dislayed left of the actual LineEdit. If not set "
              "explicitly, the displayed text will be the title of the connected signal",
              "The title for the LineEdit. If connected, the LineEdit will use the signal's "
              "title, if this property is not set explicitly.",
              QVariant::fromValue(yoyo::properties::connected_string_t { "", "" }))
    .property("unit", "Unit",
              "Define the text to be dislayed right of the actual LineEdit. If not set explicitly"
              ", the displayed text will be the unit of the connected signal",
              "The unit for the LineEdit. If connected, the LineEdit will use the signal's "
              "unit, if this property is not set explicitly.",
              QVariant::fromValue(yoyo::properties::connected_string_t { "", "" }))
    .property(
      "connection", "Connection",
      "This property holds the connection to one or more signals.\n"
      "- Data-in: defines the path to the signal providing input values to be displayed\n"
      "- Data-out: defines the path to the signal to which the values entered here will be sent\n"
      "- Auto-connect: the signal specified here will be used both for input as well as output.",
      "This property holds the connection to one or more signals.\n"
      "- Data-in: defines the path to the signal providing input values to be displayed\n"
      "- Data-out: defines the path to the signal to which the values entered here will be sent\n"
      "- Auto-connect: the signal specified here will be used both for input as well as output.",
      QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "", "", "" }))
    .property(
      "captureKey", "Capture-key",
      "The capture-key property holds whether the LineEdit shall transmit each key-press "
      "(checked) or whether it shall wait for an Enter-key press to signal that the editing is"
      "finished (unchecked).",
      "- checked: send out each character individually,\n"
      "- unchecked: send out data after Enter/Return press",
      false);
}

template <>
auto updateDocumentation<yoyo::data_group>(documentation::builder& builder) -> void
{
  updateDocumentation<data_node>(builder);
  builder.description("A signal group can have all kinds of child signals.");
}

template <>
auto updateDocumentation<yoyo::bit_node>(documentation::builder& builder) -> void
{
  updateDocumentation<data_value_node>(builder);
  builder.description("A signal for a Boolean value.")
    .property(
      "toolTip", "ToolTip",
      "Use this property to provide a string, which will be displayed as tool tip for a "
      "connected widget in the Gui. The connected widget may override this propery by its own"
      " tool tip.",
      "Use this property to provide a string, which will be displayed as tool tip for a "
      "connected widget in the Gui. The connected widget may override this propery by its own"
      " tool tip.",
      "");
}

template <>
auto updateDocumentation<yoyo::string_node>(documentation::builder& builder) -> void
{
  updateDocumentation<data_value_node>(builder);
  builder.description("A signal holding an array of characters: a string value.")
    .property(
      "toolTip", "ToolTip",
      "Use this property to provide a string, which will be displayed as tool tip for a "
      "connected widget in the Gui. The connected widget may override this propery by its own"
      " tool tip.",
      "Use this property to provide a string, which will be displayed as tool tip for a "
      "connected widget in the Gui. The connected widget may override this propery by its own"
      " tool tip.",
      "");
}

template <>
auto updateDocumentation<yoyo::int8node>(documentation::builder& builder) -> void
{
  updateDocumentation<data_value_node>(builder);
  builder.description("A signal for an 8-bit integer value.")
    .property(
      "toolTip", "ToolTip",
      "Use this property to provide a string, which will be displayed as tool tip for a "
      "connected widget in the Gui. The connected widget may override this propery by its own"
      " tool tip.",
      "Use this property to provide a string, which will be displayed as tool tip for a "
      "connected widget in the Gui. The connected widget may override this propery by its own"
      " tool tip.",
      "")
    .property(
      "min", "Minimum",
      "To restrict the lower bounds, the values of this signal can vary in, provide a value "
      "for this property.",
      "The minimal value, this signal can have.",
      QVariant::fromValue(std::numeric_limits<int8_t>::min()))
    .property(
      "max", "Maximum",
      "To restrict the lower bounds, the values of this signal can vary in, provide a value "
      "for this property.",
      "The minimal value, this signal can have.",
      QVariant::fromValue(std::numeric_limits<int8_t>::max()));
}

template <>
auto updateDocumentation<yoyo::uint8node>(documentation::builder& builder) -> void
{
  updateDocumentation<data_value_node>(builder);
  builder.description("A signal for an 8-bit unsigned integer value.")
    .property(
      "toolTip", "ToolTip",
      "Use this property to provide a string, which will be displayed as tool tip for a "
      "connected widget in the Gui. The connected widget may override this propery by its own"
      " tool tip.",
      "Use this property to provide a string, which will be displayed as tool tip for a "
      "connected widget in the Gui. The connected widget may override this propery by its own"
      " tool tip.",
      "")
    .property(
      "min", "Minimum",
      "To restrict the lower bounds, the values of this signal can vary in, provide a value "
      "for this property.",
      "The minimal value, this signal can have.",
      QVariant::fromValue(std::numeric_limits<uint8_t>::min()))
    .property(
      "max", "Maximum",
      "To restrict the lower bounds, the values of this signal can vary in, provide a value "
      "for this property.",
      "The minimal value, this signal can have.",
      QVariant::fromValue(std::numeric_limits<uint8_t>::max()))
    .property("format", "Integer format",
              "Integer values - unsigned ones in particular - can be displayed in various formats."
              "Yoyo supports the following formats:\n"
              "- Binary\n- Octal\n- Decimal\n- Hexal",
              "Specify the display format of the integer value:\n"
              "- Binary\n- Octal\n- Decimal\n- Hexal",
              {})
    .property(
      "enumValues", "Enum",
      "If an unsigned integer value has Enum enabled, it is possible to specify names and "
      "particular actions to specific values, the signal can have. The title of each option "
      "(value) will be displayed in Gui, if connected to a ComboBox for example; the script "
      "will be executed, when this particular value is set to the signal.",
      "If enabled, names, titles and scripts can be assigned to particular values. Titles "
      "will be displayed in a connected widget, if the signal has this value. The respective "
      "script will be executed, when this value is reached.",
      {});
}

template <>
auto updateDocumentation<yoyo::configuration>(documentation::builder& builder) -> void
{
  updateDocumentation<node_base>(builder);
}

template <>
auto updateDocumentation<yoyo::configuration_data>(documentation::builder& builder) -> void
{
  updateDocumentation<node_base>(builder);
  builder.description("This item contains user configurable data for the configuration.")
    .property(
      "afterLoad", "After load script",
      "The after load script will be executed directly after the configuration has been loaded"
      "completely.",
      "Script to be executed directly after load.", {})
    .property(
      "beforeClose", "Before close script",
      "The before close script will be executed directly before the configuration is to be closed.",
      "Script to be executed directly before close.", {})
    .property(
      "timerPeriod", "Periodic timer period",
      "The periodic timer period is the time interval in seconds, which passes between triggering "
      "the periodic script. If the provided interval is 0 or negative, the script is disabled.",
      "Time between trigger events for the periodic script. \nDisabled if 0 or negative.", -1)
    .property("timerScript", "Periodic script",
              "The periodic script is to be executed in periodic intervals, if set and enabled.",
              "Script to execute in periodic intervals.", {});
}

template <>
auto updateDocumentation<yoyo::data_root>(documentation::builder& builder) -> void
{
  updateDocumentation<node_base>(builder);
}

template <>
auto updateDocumentation<yoyo::gui_root>(documentation::builder& builder) -> void
{
  updateDocumentation<gui_node>(builder);
}

template <>
auto updateDocumentation<yoyo::message_field>(documentation::builder& builder) -> void
{
  updateDocumentation<node_base>(builder);
  builder
    .description("The message-field defines how to fill the message with data. It defines the "
                 "position of the data and the size of it. The field is linked to a signal, that "
                 "provides or receives the data.")
    .property("connection", "Name",
              "This property holds the connection to the signal/data object to propagate values to "
              "or to receive values from.",
              "The signal to connect to.", {})
    .property("bitPos", "Bit-pos", "The position inside the bitfield to start the message-field.",
              "Bitfield position within the message in bit", 0)
    .property(
      "bufferLength", "Buffer",
      "The length of the buffer for transmission. Set to zero (0) to disable. String type signals "
      "essentially need buffered message fields, if the messages are not sent as stream.",
      "Buffer lenghts for transmit messages. \"0\" disables the buffer. \n"
      "Note that signals of string type must be buffered, if message is not sent as stream",
      0)
    .property("bufferAsPackage", "Buffer as package",
              "Data can be transmitted as raw byte data or as packages. Use this property to "
              "switch on the package option.",
              "If enabled, the data are buffered as packet instead of raw data byte", false);
}

template <>
auto updateDocumentation<yoyo::message>(documentation::builder& builder) -> void
{
  updateDocumentation<yoyo::node_base>(builder);
  builder
    .description(
      "The message defines the data chunk which is send over the communication interface.")
    .property("interval", "Interval",
              "The behavior of this property depends on the transmission direction:\n"
              " - Tx: the interval between send operations. In case Send on new data is enabled, "
              "this corresponds to the minimal interval between messages.\n"
              " - Rx: the expected interval between received messages.\n\n"
              "Use an interval value of smaller than 0 to disable.\nInterval in seconds.",
              "The interval (in seconds) between messages:\n"
              " - Tx: the interval between send messages. If send on new data is enabled, this "
              "corresponds to the minimal interval between messages.\n"
              " - Rx: expected received message interval.\n\n"
              "Use value < 0 to disable.",
              -1)
    .property("sendOnNewData", "Send on new data",
              "Set this property to true to prevent repeatedly sending unchanged values.",
              "Message is only sent, if new data is available for one of the fields.", false)
    .property(
      "direction", "Direction",
      "This is the message direction: Tx is outgoing, Rx is incoming with reference to Yoyo.",
      "Message direction:\n"
      " - Tx: Message is sent by Yoyo.\n"
      " - Rx: Message is received by Yoyo.",
      QVariant::fromValue(yoyo::properties::transmission_direction_t::RX))
    .property("isStandard", "Message type",
              "For CAN interfaces two message types are supported: standard and extended. This "
              "property is true, if the standard message type is used, false for extended.",
              "Applicable for CAN message only:\n"
              " - checked: use standard message type\n"
              " - unchecked: use extended message type",
              true)
    .property(
      "messageLength", "Message Length",
      "When messages are sent and not in stream mode, this defines the length of the message.",
      "Message length in byte used when message is send. Only valid if stream is not set", 0)
    .property(
      "isStream", "Stream",
      "If enabled, the message is filled with the data of the first connected field to build a "
      "stream, else the message is filled by the data as defined by the field offset",
      "If enabled, the message is filled with the data of the first connected field to build a "
      "stream, else the message is filled by the data as defined by the field offset",
      false);
}

auto install_gui_nodes(node_factory& gui_factory) -> void
{
  auto noChildren = [](node_factory::node_id_list const&) { return node_factory::node_id_list {}; };
  ::install<yoyo::group>(gui_factory, "Group", {},
                         [](node_factory::node_id_list const& l) { return l; });

  ::install<yoyo::button>(gui_factory, "Button", {}, noChildren);
  ::install<yoyo::check_box>(gui_factory, "CheckBox", {}, noChildren);
  ::install<yoyo::combo_box>(gui_factory, "ComboBox", {}, noChildren);
  ::install<yoyo::line_edit>(gui_factory, "LineEdit", {}, noChildren);
}

auto install_data_nodes(node_factory& data_factory) -> void
{
  auto noChildren = [](node_factory::node_id_list const&) { return node_factory::node_id_list {}; };
  ::install<yoyo::data_group>(data_factory, "Group", {},
                              [](node_factory::node_id_list const& l) { return l; });
  ::install<yoyo::bit_node>(data_factory, "Bit", {}, noChildren);
  ::install<yoyo::string_node>(data_factory, "String", {}, noChildren);
  ::install<yoyo::int8node>(data_factory, "Int8", {}, noChildren);
  ::install<yoyo::uint8node>(data_factory, "Uint8", {}, noChildren);
}

auto install_fundamental_nodes(node_factory& factory) -> void
{
  ::install<yoyo::configuration>(
    factory, "Configuration", {}, [](node_factory::node_id_list const&) {
      return node_factory::node_id_list {
        { data_root::typeId(), &data_root::staticMetaObject, {} },
        { gui_root::typeId(), &gui_root::staticMetaObject, {} },
        { configuration_data::typeId(), &configuration_data::staticMetaObject, {} },
      };
    });
  ::install<yoyo::data_root>(factory, "Signals", {}, [](node_factory::node_id_list const& l) {
    node_factory::node_id_list result;
    std::copy_if(l.begin(), l.end(), std::back_inserter(result), [](auto const& n) {
      return std::get<1>(n)->inherits(&data_node::staticMetaObject);
    });
    return result;
  });
  ::install<yoyo::gui_root>(factory, "Layout", {}, [](node_factory::node_id_list const& l) {
    node_factory::node_id_list result;
    std::copy_if(l.begin(), l.end(), std::back_inserter(result), [](auto const& n) {
      return std::get<1>(n)->inherits(&gui_node::staticMetaObject);
    });
    return result;
  });
  ::install<yoyo::configuration_data>(
    factory, "Configuration data", {},
    [](node_factory::node_id_list const&) { return node_factory::node_id_list {}; });
  ::install<yoyo::message_field>(
    factory, "Message Field", {},
    [](node_factory::node_id_list const&) { return node_factory::node_id_list {}; });
  ::install<yoyo::message>(factory, "Message", {}, [](node_factory::node_id_list const& l) {
    node_factory::node_id_list result;
    std::copy_if(l.begin(), l.end(), std::back_inserter(result),
                 [](auto const& n) { return std::get<0>(n) == fundamental::message_field_id; });
    return result;
  });
}

auto install_communication_nodes(node_factory&) -> void {}

auto install_protocol_nodes(node_factory&) -> void {}
} // namespace yoyo
