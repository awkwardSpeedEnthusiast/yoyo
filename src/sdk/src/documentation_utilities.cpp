#include "yoyo/documentation_utilities.h"
#include "yoyo/data_container_node.h"
#include "yoyo/data_node.h"
#include "yoyo/data_value_node.h"
#include "yoyo/gui_node.h"

namespace yoyo
{
template <>
auto YOYO_SDK_SHARED_EXPORT updateDocumentation<node_base>(documentation::builder& builder) -> void
{
  builder.property("type", "Type", "The type name of the item.", "The type name of the item.", "")
    .property(
      "name", "Name",
      "The name of the layout item. It is used in particular, when referencing an item in a Style"
      " sheet.",
      "Set the name of the layout item to make it referencable in a Style sheet.", "");
}

template <>
auto YOYO_SDK_SHARED_EXPORT updateDocumentation<data_node>(documentation::builder& builder) -> void
{
  updateDocumentation<node_base>(builder);
  builder.property("type", "Type", "The type name of the item.", "The type name of the item.", "")
    .property(
      "name", "Identifier",
      "Use the identifier to access signals in the signal tree, creating a signal path. A "
      "signal path consists of the identifiers of the item to access, prefixed with all the "
      "identifiers of its parents separated by a dot (example: general.data.mySignal). This"
      "identifier must be unique within the list of children.",
      "Unique identifier/name for this signal.", "")
    .property("valueType", "Value type", "The value type this signal represents.",
              "The value type of this signal.", QVariant::fromValue(yoyo::types::value_t::CUSTOM));
}

template <>
auto YOYO_SDK_SHARED_EXPORT updateDocumentation<data_value_node>(documentation::builder& builder)
  -> void
{
  updateDocumentation<data_node>(builder);
  builder
    .property("title", "Title",
              "The title of this signal to be used in Gui to be displayed before the actual value "
              "display. This might be overridden by the title property of the connected widget.",
              "Title to be shown in the Gui before the signal's value; might be overridden by the "
              "title property of the connected widget.",
              "")
    .property("unit", "Unit",
              "The unit of this signal to be used in Gui to be displayed after the actual value "
              "display. This might be overridden by the unit property of the connected widget.",
              "Unit to be shown in the Gui after the signal's value; might be overridden by the "
              "unit property of the connected widget.",
              QVariant::fromValue(yoyo::properties::invisible_string_t { "", true }))
    .property(
      "access", "Access",
      "Describes the access for the signal from the Gui perspective:\n"
      "- Read: a connected widget can read the signal's data, but not write\n"
      "- Write: a connected widget can write to the signal, but not read its data\n"
      "- Read/write: a connected widget can both read from and write to the signal\n\n"
      "This explicitly excludes other means of data transfer, like protocols, communication "
      "interfaces or scripts.",
      "Describes the access for the signal from the Gui perspective:\n"
      "- Read: a connected widget can read the signal's data, but not write\n"
      "- Write: a connected widget can write to the signal, but not read its data\n"
      "- Read/write: a connected widget can both read from and write to the signal\n\n"
      "This explicitly excludes other means of data transfer, like protocols, communication "
      "interfaces or scripts.",
      QVariant::fromValue(yoyo::types::access_t::READ_WRITE))
    .property(
      "onChange", "On-change",
      "The on-change property holds a script to be executed, if a change is detected for its "
      "value. This script can be a native script, a Java-script or a file to be executed. The"
      " Image option is ignored.",
      "The on-change property holds a script to be executed, if a change is detected for its "
      "value. This script can be a native script, a Java-script or a file to be executed. The"
      " Image option is ignored.",
      QVariant::fromValue(yoyo::properties::script_t {}));
}

template <>
auto YOYO_SDK_SHARED_EXPORT
updateDocumentation<data_container_node>(documentation::builder& builder) -> void
{
  updateDocumentation<data_node>(builder);
}

template <>
auto YOYO_SDK_SHARED_EXPORT updateDocumentation<gui_node>(documentation::builder& builder) -> void
{
  updateDocumentation<node_base>(builder);
  builder
    .property(
      "name", "Name",
      "The name of the layout item. It is used in particular, when referencing an item in a Style"
      " sheet.",
      "Set the name of the layout item to make it referencable in a Style sheet.", "")
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
        yoyo::types::layout_direction_t::HORIZONTAL, false }))
    .property(
      "xPosition", "X Position",
      "When the parent layout item defines its layout as Free, this property sets the x position "
      "of the item inside its parent, else it will be defined by the layout.",
      "Defines the x position of the item in case parent has Free layout", { 0 })
    .property(
      "yPosition", "Y Position",
      "When the parent layout item defines its layout as Free, this property sets the y position "
      "of the item inside its parent, else it will be defined by the layout.",
      "Defines the y position of the item in case parent has Free layout", { 0 })
    .property(
      "width", "Width",
      "When the parent layout item defines its layout as Free, this property sets the width of "
      "the item, else it will be defined by the layout.",
      "Defines the width of the item in case parent has Free layout", { 0 })
    .property(
      "height", "Height",
      "When the parent layout item defines its layout as Free, this property sets the height of "
      "the item, else it will be defined by the layout.",
      "Defines the height of the item in case parent has Free layout", { 0 })
    .property(
      "enabled", "Enabled",
      "An enabled item can get focus, a disabled item will be grayed out. If this is defined by "
      "connection, the item will be enabled if the item is connected.",
      "Defines whether the item is enabled or not. If it is defined by connection, it will be "
      "enabled, when connected.",
      QVariant::fromValue(yoyo::properties::connected_boolean_t {
        yoyo::properties::connected_boolean_t::boolean_t::TRUE, true }))
    .property(
      "visible", "Visible",
      "An item which is set to visible will be shown in the Gui, an invisible item is hidden.",
      "Defines the x position of the item in case parent has Free layout", { true })
    .property(
      "styleSheet", "Style sheet",
      "The style sheet defines the look and feel of a widget. Colors, size constraints, fonts and"
      " other features can be defined here. See the Qt documentation on style sheets for more "
      "detail (https://doc.qt.io/qt-5/stylesheet-syntax.html).",
      "Define the widget's style sheet (see https://doc.qt.io/qt-5/stylesheet-syntax.html for "
      "more information.\nUse Enter key for submitting changes and Shift-Enter for a new line.",
      "");
}
} // namespace yoyo
