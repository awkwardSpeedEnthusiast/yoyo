#pragma once
#include "yoyo/node_base.h"
#include "yoyo/property_types.h"
#include "yoyo/yoyo_sdk_global.h"

#include <boost/signals2.hpp>

namespace yoyo
{
///
/// \brief base class for signalling nodes
///
/// This class is the base class for all data signalling elements.
/// It contains a value type and provides access-types for setting and retrieving the signalled
/// value.
///
class YOYO_SDK_SHARED_EXPORT data_node : public node_base
{
  Q_OBJECT
  ///
  /// \brief type of the signalled value
  ///
  /// This property holds the value type of the property to be able to display in gui.
  ///
  Q_PROPERTY(yoyo::types::value_t valueType READ valueType)
public:
  ///
  /// \brief Constructor
  ///
  /// Constructor. Creates a data_node.
  /// \param type_name the name of the type to be shown in the UI,
  /// \param id the individual node's identifier,
  /// \param valuetype the value type for this node,
  /// \param connectable true, if the node can be connected to an io of an other node type.
  ///
  data_node(QString type_name, boost::uuids::uuid id, types::value_t valuetype, bool connectable);
  ~data_node() override;
  data_node(data_node const& other) = delete;
  data_node& operator=(data_node const& other) = delete;
  data_node(data_node&& other) = delete;
  data_node& operator=(data_node&& other) = delete;

  ///
  /// \brief can this data node be connected?
  ///
  /// The can connect property signals whether this data-node will provide an actual value object.
  /// If it does, a gui element or other node category can connect to this node and receive its
  /// value as input and back.
  /// \return true if the node has a value to be connected to.
  ///
  auto canConnect() const -> bool;

  ///
  /// \brief connect to value
  ///
  /// Use this method to subscribe to value changes of the data node.
  /// \param subscriber the functor to be called, when the data node receives a new value.
  /// \return a connection object representing the newly made connection.
  ///
  auto connectToValue(std::function<void(QVariant)> subscriber) -> boost::signals2::connection;

  ///
  /// \brief set value into data node
  ///
  /// This is the data setter for all data nodes. This is to be called by a connector to transfer
  /// data to a data node.
  /// This will only work, if canConnect returns true.
  ///
  /// Being purely virtual, this method has to be implemented by derived classes.
  /// This implementation has to take care, that the type-erased value is casted sensibly to the
  /// value type of the actual data node.
  /// In case the stored type doesn't match the value type of the data node (signed int vs.
  /// unsigned), the value will be rejected. Strings are tried to be converted to numerical values
  /// if appropriate.
  /// \param value the value to be set to the data node.
  ///
  virtual auto setValue(QVariant const& value) -> void = 0;

  ///
  /// \brief return value
  ///
  /// This method returns the buffered value of the data node in case buffering is enabled. It
  /// will be default constructed if not.
  /// \return the buffered value of the data node.
  ///
  virtual auto value() const -> types::variant_t = 0;

  ///
  /// \{
  /// \name properties
  ///

  ///
  /// \brief valueType
  /// \return the value type of the data node.
  ///
  auto valueType() const -> types::value_t;

  /// \}

protected:
  ///
  /// \brief emit value changed signal
  ///
  /// This method will emit the value changed event for connected nodes to receive.
  /// \param value the new value of the data node.
  ///
  auto emitValueChanged(QVariant const& value) -> void;

private:
  struct impl;
  std::unique_ptr<impl> _p;
};
} // namespace yoyo
