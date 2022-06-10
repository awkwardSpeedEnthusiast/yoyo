#pragma once
#include "nodes_export.h"
#include "yoyo/node_factory.h"

namespace yoyo
{
namespace fundamental
{
constexpr boost::uuids::uuid const configuration_id = { 0xce, 0xd3, 0x5b, 0xdc, 0xc7, 0xf6,
                                                        0x44, 0x68, 0x9b, 0x61, 0x57, 0xfe,
                                                        0xfe, 0x02, 0x2e, 0x9d };
constexpr boost::uuids::uuid const data_root_id = {
  0xca, 0x48, 0xe9, 0x19, 0x0e, 0xc4, 0x4f, 0x73, 0x9d, 0xfa, 0x20, 0x3e, 0xcb, 0x85, 0x8b, 0xd5
};
constexpr boost::uuids::uuid const gui_root_id = { 0x38, 0x81, 0xcf, 0xaa, 0x6f, 0x62, 0x47, 0x30,
                                                   0x8c, 0xd,  0x41, 0x6c, 0x9b, 0xb0, 0x11, 0xbc };

} // namespace fundamental

auto YOYO_NODES_SHARED_EXPORT install_gui_nodes(node_factory& gui_factory) -> void;
auto YOYO_NODES_SHARED_EXPORT install_data_nodes(node_factory& data_factory) -> void;
auto YOYO_NODES_SHARED_EXPORT install_communication_nodes(node_factory& communication_factory)
  -> void;
auto YOYO_NODES_SHARED_EXPORT install_protocol_nodes(node_factory& protocol_factory) -> void;
auto YOYO_NODES_SHARED_EXPORT install_fundamental_nodes(node_factory& factory) -> void;
} // namespace yoyo
