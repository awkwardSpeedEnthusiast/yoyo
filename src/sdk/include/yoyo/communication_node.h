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

  virtual auto send_data(QByteArray const& data) -> void = 0;

Q_SIGNALS:
  void data_received(QByteArray data);
  void data_with_id_received(QByteArray data, uint32_t);
};
} // namespace yoyo
