#pragma once
#include "yoyo/data_node.h"
#include "yoyo/yoyo_sdk_global.h"

namespace yoyo
{
///
/// \brief Convenience class for data/value handling data nodes.
///
/// This class is a convenience class for data/value handling data nodes. It implements all purely
/// virtual methods of its base classes, so it can be instantiated even though it is recommended to
/// further specialize it.
/// It is implemented as a general leaf node accepting all nodes as parents and accepting no
/// children.
/// It defines three new value data relevant properties: title, unit and access-type.
///
class YOYO_SDK_SHARED_EXPORT data_value_node : public data_node
{
  Q_OBJECT
  ///
  /// \brief title
  ///
  /// The title property is to be displayed in the UI. Additionally it is to be displayed in gui
  /// elements, if they are connected to this data node, left of the actual visualization element.
  ///
  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)

  ///
  /// \brief unit
  ///
  /// The unit property is to be displayed in the UI. Additionally it is to be displayled in gui
  /// elements, if they are connected to this data node, right of the actual visualization element.
  Q_PROPERTY(yoyo::properties::invisible_string_t unit READ unit WRITE setUnit NOTIFY unitChanged)

  ///
  /// \brief access
  ///
  /// This is the access type properts to be displayed in UI. It defines whether gui elements may
  /// read and/or write to/from this data node. In short: it defines in which direction the
  /// connections between this data node and gui elements are made.
  Q_PROPERTY(yoyo::types::access_t access READ access WRITE setAccess NOTIFY accessChanged)

  ///
  /// \brief on value change script
  ///
  /// This property allows to store a script, which is invoked when the value of the data node
  /// changes.
  Q_PROPERTY(
    yoyo::properties::script_t onChange READ onChange WRITE setOnChange NOTIFY onChangeChanged)

public:
  ///
  /// \brief Constructor
  /// \param type_name the name of the node type to be displayed in UI,
  /// \param id the individual node's identifier,
  /// \param valuetype the value type of the node.
  ///
  data_value_node(QString type_name, boost::uuids::uuid id, types::value_t valuetype);
  ~data_value_node() override;
  data_value_node(data_value_node const& other) = delete;
  data_value_node& operator=(data_value_node const& other) = delete;
  data_value_node(data_value_node&& other) = delete;
  data_value_node& operator=(data_value_node&& other) = delete;

  ///
  /// \brief set signalling value
  ///
  /// This method sets a value into the data node.
  /// Since this implementation is a primitive one, it directly forwards the value to the output
  /// signal. No buffering is done in this node.
  /// If access is read-only, this method will return without effect.
  /// \param value the new value for the data node.
  ///
  auto setValue(QVariant const& value) -> void override;

  ///
  /// \{
  /// \name properties
  ///

  ///
  /// \brief access
  ///
  /// Returns the access type of the data node. The access type defines the connections which can
  /// be made to or from this data node to a gui node. If read is allowed, connections to the
  /// value notifier are allowed. If write is allowed, calls to setValue are allowed.
  /// \return the access type of the node.
  ///
  auto access() const -> types::access_t;

  ///
  /// \brief set access
  ///
  /// Sets the access type of the data node. The access type defines the connections which can
  /// be made to or from this data node to a gui node. If read is allowed, connections to the
  /// value notifier are allowed. If write is allowed, calls to setValue are allowed.
  /// \param acc the new access value for the node.
  ///
  auto setAccess(types::access_t acc) -> void;

  ///
  /// \brief title
  ///
  /// Returns the title to be seen in UI. If connected to a gui element, this title shall also be
  /// visible inside this gui element.
  /// \return the title of this data node.
  ///
  auto title() const -> QString;

  ///
  /// \brief set title
  ///
  /// Sets the title to be seen in UI. If connected to a gui element, this title shall also be
  /// visible inside this gui element.
  /// \param t the new title for this data node.
  ///
  auto setTitle(QString const& t) -> void;

  ///
  /// \brief unit
  ///
  /// Returns the unit to be seen in UI. If connected to a gui element, this unit value shall also
  /// be visible inside this gui element.
  /// \return the unit of this data node.
  ///
  auto unit() const -> yoyo::properties::invisible_string_t;

  ///
  /// \brief set unit
  ///
  /// Sets the unit to be seen in UI. If connected to a gui element, this unit value shall also
  /// be visible inside this gui element.
  /// \param u the unit for this data node.
  ///
  auto setUnit(yoyo::properties::invisible_string_t const& u) -> void;

  ///
  /// \brief on value change script
  ///
  /// Returns the script to be executed, when the value of this data node changes.
  /// \return the on value change script.
  ///
  auto onChange() const -> yoyo::properties::script_t;

  ///
  /// \brief set on alue change script
  ///
  /// Sets the script to be executed, when the value of this data node changes.
  /// \param s the new on value change script.
  ///
  auto setOnChange(yoyo::properties::script_t s) -> void;

  /// \}

  auto acceptsChildren() const -> bool override;
  auto acceptsChild(std::shared_ptr<node_base> const& child) const -> bool override;
  auto acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool override;

Q_SIGNALS:
  ///
  /// \brief title changed signal
  /// This signal is emitted, if the title of the node changed.
  /// \param t the new title of the data node.
  ///
  void titleChanged(QString const& t);

  ///
  /// \brief unit changed signal
  /// This signal is emitted, if the unit descriptor of the node changed.
  /// \param u the new unit of the data node.
  ///
  void unitChanged(yoyo::properties::invisible_string_t const& u);

  ///
  /// \brief access changed signal
  /// This signal is emitted, if the access type of the node changed.
  /// \param acc the new access type of the data node.
  ///
  void accessChanged(types::access_t acc);

  ///
  /// \brief on value change script changed signal
  /// \param s the signal to invoke, when the value of this data node changes.
  ///
  void onChangeChanged(properties::script_t s);

protected:
  ///
  /// \brief do value changed procedures for value data nodes
  ///
  /// This method will emit the value changed signal from the base node.
  /// In addition it will also check the on change script and emit a script execution request, if
  /// the script is not empty.
  /// \param v the new value of the data node.
  ///
  auto doValueChanged(QVariant v) -> void;

private:
  auto childAboutToBeAdded(std::shared_ptr<node_base> const& child) -> bool override;
  auto childAdded(std::shared_ptr<node_base> const& child) -> bool override;
  auto childAboutToBeRemoved(std::shared_ptr<node_base> const& child) -> bool override;
  auto childRemoved(std::shared_ptr<node_base> const& child) -> bool override;
  auto childMoved(std::shared_ptr<node_base> const& child, size_t from_index, size_t to_index)
    -> void override;

private:
  struct impl;
  std::unique_ptr<impl> _p;
};
} // namespace yoyo
