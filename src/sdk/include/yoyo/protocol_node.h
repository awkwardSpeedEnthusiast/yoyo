#pragma once
#include "yoyo/node_base.h"
#include "yoyo/yoyo_sdk_global.h"

namespace yoyo
{
///
/// \brief base class for all nodes below protocols
///
/// This class is the base class for all nodes which are to be installed into the node_factory
/// responsible for protocol nodes.
///
class YOYO_SDK_SHARED_EXPORT protocol_node_base : public node_base
{
  Q_OBJECT
public:
  using node_base::node_base;
  protocol_node_base(protocol_node_base const& other) = delete;
  protocol_node_base& operator=(protocol_node_base const& other) = delete;
  protocol_node_base(protocol_node_base&& other) = delete;
  protocol_node_base& operator=(protocol_node_base&& other) = delete;
};

///
/// \brief base class for protocols
///
/// This class offers base functionality for protocols. Each protocol allows serialization (data
/// coming from inside the program), which will end up in the output connection slot of the node,
/// and deserialization (data coming from a microcontroller or other sources outside the program),
/// which will end up in the input connection slot of the node.
///
class YOYO_SDK_SHARED_EXPORT protocol_node : public protocol_node_base
{
  Q_OBJECT
  ///
  /// \brief property to define the output connection slot
  ///
  /// This property defines the connection facing the output side of the application. It will be
  /// connected to the serialize function.
  ///
  Q_PROPERTY(yoyo::properties::connection_t output READ output WRITE setOutput NOTIFY outputChanged)
  ///
  /// \brief property to define the input connnection slot
  ///
  /// This property defines the connection facing the input side of the application. It will be
  /// connected to the deserialize function.
  Q_PROPERTY(yoyo::properties::connection_t input READ input WRITE setInput NOTIFY inputChanged)
public:
  ///
  /// \brief constructor
  ///
  /// \param type the type-name as displayed in the Gui,
  /// \param identifier the internal identifier of the node.
  ///
  protocol_node(QString type, boost::uuids::uuid identifier = boost::uuids::uuid {});
  ~protocol_node() override;
  protocol_node(protocol_node const& other) = delete;
  protocol_node& operator=(protocol_node const& other) = delete;
  protocol_node(protocol_node&& other) = delete;
  protocol_node& operator=(protocol_node&& other) = delete;

  ///
  /// \{
  /// \name properties
  ///

  ///
  /// \brief getter of the output property
  ///
  /// The output property defines the connection to the side facing the external connection of
  /// the application. It will be connected to the serialize function.
  /// \return the connection specification for the output connection.
  ///
  auto output() const -> yoyo::properties::connection_t;
  ///
  /// \brief getter of the input property
  ///
  /// The input property defines the connection to the side facing the internal data-structures of
  /// the application. It will be connected to the deserialize function.
  /// \return the connection specification for the input connection.
  ///
  auto input() const -> yoyo::properties::connection_t;
  ///
  /// \brief setter of the output property
  ///
  /// The output property defines the connection to the side facing the external connection of
  /// the application. It will be connected to the serialize function.
  /// \param out the connection specification for the output connection.
  ///
  auto setOutput(yoyo::properties::connection_t out) -> void;
  ///
  /// \brief setter of the input property
  ///
  /// The input property defines the connection to the side facing the internal data-structures of
  /// the application. It will be connected to the deserialize function.
  /// \param in the connection specification for the input connection.
  ///
  auto setInput(yoyo::properties::connection_t in) -> void;

  ///
  ///  \}
  ///

  ///
  /// \{
  /// \name protocol relevant methods
  ///

  ///
  /// \brief serialize data for output to external processes/devices
  ///
  /// This method is the data serialization method for the protocol. It will be connected to
  /// data node by the connection engine. This data-node is meant to be application facing, a
  /// node, which is connected to a gui node for example, taking user input.
  ///
  /// It may also be called directly by an implementation of this class or by child nodes, which
  /// are connected to individual data nodes to provide improved functionality.
  ///
  /// This method will serialize the data and publish it to the output connection and any node,
  /// which is connected here.
  ///
  /// Note: Being purely virtual, this method needs to be implemented by deriving classes, taking
  /// care to meet the specifications, which are defined above.
  /// \param v the data to serialize.
  ///
  virtual auto serialize(QVariant const& v) -> void = 0;
  ///
  /// \brief deserialize data for input from external processes/devices
  ///
  /// This method is the data deserialization method for the protocol. It will be connected to a
  /// data node by the connection engine. This data-node is meant to be outside facing, a node,
  /// which is connected to a communication interface.
  ///
  /// This method will deserialize the data and post it to the input connection and any node,
  /// which is connected here. It can either set the input channel directly or use the child nodes
  /// to fill explicitly connected data nodes.
  ///
  /// Note: Being purely virtual, this method needs to be implemented by deriving classes, taking
  /// care to meet the specifications, which are defined above.
  /// \param v the data to deserialize.
  ///
  virtual auto deserialize(QVariant const& v) -> void = 0;
  ///
  /// \brief determine whether output supports a value type
  ///
  /// This method is called by the connection engine to determine, whether this connection
  /// supports a specific value type; or whether the type can be converted internally to the
  /// wanted format.
  ///
  /// Note: Being purely virtual, this method needs to be implemented by deriving classes, taking
  /// care to meet the specifications defined above.
  /// \param t the type to check,
  /// \return true if the type is supported.
  ///
  virtual auto outputSupports(types::value_t t) const -> bool = 0;
  ///
  /// \brief determine whether inpuput supports a value type
  ///
  /// This method is called by the connection engine to determine, whether this connection
  /// supports a specific value type; or whether the type can be converted internally to the
  /// wanted format.
  ///
  /// Note: Being purely virtual, this method needs to be implemented by deriving classes, taking
  /// care to meet the specifications defined above.
  /// \param t the type to check,
  /// \return true if the type is supported.
  ///
  virtual auto inputSupports(types::value_t t) const -> bool = 0;

  ///
  /// \}
  ///

Q_SIGNALS:
  ///
  /// \brief changed signal for the output property
  ///
  /// This signal will notify the subscriber that the output connection specification changed.
  /// \param out the new value of the output specification.
  ///
  void outputChanged(yoyo::properties::connection_t out);
  ///
  /// \brief changed signal for the input property
  ///
  /// This signal will notify the subscriber that the input connection specification changed.
  /// \param in the new value of the input specification.
  ///
  void inputChanged(yoyo::properties::connection_t in);

protected:
  ///
  /// \brief notify, that value on input has changed
  ///
  /// Use this method in derived classes to notify nodes connected to this node's input slot, that
  /// there is a new value available on the input slot.
  /// \param v the new value.
  ///
  auto notify_value_on_input_changed(QVariant v) -> void;
  ///
  /// \brief notify, that value on output has changed
  ///
  /// Use this method in derived classes to notify nodes connected to this node's output slot,
  /// that there is a new value available on the output slot.
  /// \param v the new value.
  ///
  auto notify_value_on_output_changed(QVariant v) -> void;

private:
  struct impl;
  std::unique_ptr<impl> _p;
};
} // namespace yoyo
