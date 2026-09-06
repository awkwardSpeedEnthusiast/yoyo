#pragma once
#include "yoyo/node_base.h"
#include "yoyo/yoyo_sdk_global.h"

namespace yoyo
{
///
/// \brief the base class for all nodes below a communication node
///
/// This class has been introduced to allow simpler child nodes (still related to communication
/// interfaces) to be created with the communication node factory.
///
class YOYO_SDK_SHARED_EXPORT communication_node_base : public node_base
{
  Q_OBJECT
public:
  using node_base::node_base;
  communication_node_base(communication_node_base const& other) = delete;
  communication_node_base& operator=(communication_node_base const& other) = delete;
  communication_node_base(communication_node_base&& other) = delete;
  communication_node_base& operator=(communication_node_base&& other) = delete;
};

///
/// \brief base class for all communication defining nodes
///
class YOYO_SDK_SHARED_EXPORT communication_node : public communication_node_base
{
  Q_OBJECT
public:
  using communication_node_base::communication_node_base;
  communication_node(communication_node const& other) = delete;
  communication_node& operator=(communication_node const& other) = delete;
  communication_node(communication_node&& other) = delete;
  communication_node& operator=(communication_node&& other) = delete;

  ///
  /// \brief transfer data to the remote object
  ///
  /// This method will be called, if there is data to be transfered to the connected remote object.
  /// Being purely virtual, this method needs to be implemented by deriving classes.
  /// \param data
  ///
  virtual auto send_data(QByteArray const& data) -> void = 0;

Q_SIGNALS:
  ///
  /// \brief received data from the remote object
  ///
  /// This signal is to be emitted by deriving classes, to notify yoyo, that data has been received
  /// from the connected remote object.
  /// General version. If an id for the message can be determined at this point, use
  /// \a data_with_id_received.
  /// \param data the received data.
  ///
  void data_received(QByteArray data);

  ///
  /// \brief data has been received from the remote object
  ///
  /// This signal is to be emitted by deriving classes, to notify yoyo, that data has been received
  /// from the connected remote object. This signal is to be used, if the communication channel
  /// provides already information about the message (id) which is being sent. This id will be
  /// matched to the id of the respective message in the DataMessageHandler (message_container).
  /// \param data the received data,
  /// \param id the data or message id.
  ///
  void data_with_id_received(QByteArray data, uint32_t id);
};
} // namespace yoyo
