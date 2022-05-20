#pragma once
#include "yoyo/yoyo_sdk_global.h"

#include "yoyo/node_factory.h"

namespace yoyo
{
///
/// \brief Provide plugins to the application.
///
/// Override this class to provide new node types to the application.
///
class YOYO_SDK_SHARED_EXPORT yoyo_plugin : public QObject
{
  Q_OBJECT

public:
  yoyo_plugin() = default;
  virtual ~yoyo_plugin() = default;
  yoyo_plugin(yoyo_plugin const& other) = delete;
  yoyo_plugin& operator=(yoyo_plugin const& other) = delete;
  yoyo_plugin(yoyo_plugin&& other) = delete;
  yoyo_plugin& operator=(yoyo_plugin&& other) = delete;

  ///
  /// \brief name
  ///
  /// Returns the name of the plugin to be displayed by the gui.
  /// \return the name of the plugin.
  ///
  virtual auto name() const -> QString = 0;
  ///
  /// \brief install gui nodes
  ///
  /// Implement this method to make gui nodes available in the application.
  /// \param gui_factory the factory to install the node creation to.
  ///
  virtual auto install_gui_nodes(node_factory& gui_factory) -> void = 0;
  ///
  /// \brief install data nodes
  ///
  /// Implement this method to make data nodes available in the application.
  /// \param data_factory the factory to install the node creation to.
  ///
  virtual auto install_data_nodes(node_factory& data_factory) -> void = 0;
  ///
  /// \brief install communication nodes
  ///
  /// Implement this method to make communication nodes available in the application.
  /// \param communication_factory the factory to install the node creation to.
  ///
  virtual auto install_communication_nodes(node_factory& communication_factory) -> void = 0;
  ///
  /// \brief install protocol nodes
  ///
  /// Implement this method to make protocol nodes available in the application.
  /// \param protocol_factory the factory to install the node creation to.
  ///
  virtual auto install_protocol_nodes(node_factory& protocol_factory) -> void = 0;

  ///
  /// \brief un-install gui nodes
  ///
  /// Implement this method to remove gui nodes in the application before unloading the plugin.
  /// \param gui_factory the factory to remove the node creation from.
  ///
  virtual auto uninstall_gui_nodes(node_factory& gui_factory) -> void = 0;
  ///
  /// \brief un-install data nodes
  ///
  /// Implement this method to remove data nodes in the application before unloading the plugin.
  /// \param data_factory the factory to remove the node creation from.
  ///
  virtual auto uninstall_data_nodes(node_factory& data_factory) -> void = 0;
  ///
  /// \brief un-install communication nodes
  ///
  /// Implement this method to remove communication nodes in the application before unloading the
  /// plugin.
  /// \param communication_factory the factory to remove the node creation from.
  ///
  virtual auto uninstall_communication_nodes(node_factory& communication_factory) -> void = 0;
  ///
  /// \brief un-install protocol nodes
  ///
  /// Implement this method to remove protocol nodes in the application before unloading the plugin.
  /// \param protocol_factory the factory to remove the node creation from.
  ///
  virtual auto uninstall_protocol_nodes(node_factory& protocol_factory) -> void = 0;
};
} // namespace yoyo

constexpr auto const yoyo_plugin_iid = "org.yoyo.plugin.Interface";
Q_DECLARE_INTERFACE(yoyo::yoyo_plugin, yoyo_plugin_iid)
