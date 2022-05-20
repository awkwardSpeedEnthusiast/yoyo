#pragma once
#include "nodes_export.h"
#include "yoyo/node_factory.h"

namespace yoyo
{
auto YOYO_NODES_SHARED_EXPORT install_gui_nodes(node_factory& gui_factory) -> void;
auto YOYO_NODES_SHARED_EXPORT install_data_nodes(node_factory& data_factory) -> void;
auto YOYO_NODES_SHARED_EXPORT install_communication_nodes(node_factory& communication_factory)
  -> void;
auto YOYO_NODES_SHARED_EXPORT install_protocol_nodes(node_factory& protocol_factory) -> void;
auto YOYO_NODES_SHARED_EXPORT install_fundamental_nodes(node_factory& factory) -> void;
} // namespace yoyo
