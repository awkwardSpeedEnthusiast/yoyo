#pragma once
#include "test-plugin_export.h"

#include "yoyo/yoyo_plugin.h"

namespace yoyo
{
namespace test
{
class YOYO_TEST_PLUGIN_EXPORT test_plugin : public yoyo_plugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.yoyo.plugin.Interface.test" FILE "test_plugin.json")
  Q_INTERFACES(yoyo::yoyo_plugin)
public:
  auto name() const -> QString override;
  auto install_gui_nodes(node_factory& gui_factory) -> void override;
  auto install_data_nodes(node_factory& data_factory) -> void override;
  auto install_communication_nodes(node_factory& communication_factory) -> void override;
  auto install_protocol_nodes(node_factory& protocol_factory) -> void override;
  auto uninstall_gui_nodes(node_factory& gui_factory) -> void override;
  auto uninstall_data_nodes(node_factory& data_factory) -> void override;
  auto uninstall_communication_nodes(node_factory& communication_factory) -> void override;
  auto uninstall_protocol_nodes(node_factory& protocol_factory) -> void override;
};
} // namespace test
} // namespace yoyo
