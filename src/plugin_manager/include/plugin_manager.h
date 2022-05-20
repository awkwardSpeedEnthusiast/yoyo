#pragma once

#include "plugin_data.h"

#include <QObject>

#include <memory>

namespace yoyo
{
class node_factory;

namespace plugin
{
///
/// \brief manage and load plugins
///
/// The plugin_manager class allows management of plugins for yoyo.
/// It keeps track over known plugins and loaded plugins over several lifetimes of yoyo instances.
///
/// In contrast to some applications, yoyo doesn't require plugins to be located in a specific
/// directory inside the yoyo installation directory, but they can be located in any accessible
/// space. Each plugin must come with a json file containing some description about the plugin
/// including the location/file-name of the plugin.
///
class YOYO_PLUGIN_MANAGER_SHARED_EXPORT plugin_manager
  : public QObject,
    public std::enable_shared_from_this<plugin_manager>
{
  Q_OBJECT

public:
  ///
  /// \brief constructor
  ///
  /// When constructing this object, the list of known plugins will be retrieved from settings
  /// file. Plugins marked as loaded will be loaded automatically.
  /// \param data the factory for data nodes
  /// \param gui the factory for gui nodes
  /// \param protocol the factory for protocol nodes
  /// \param communication the factory for communication nodes
  ///
  plugin_manager(std::shared_ptr<node_factory> data, std::shared_ptr<node_factory> gui,
                 std::shared_ptr<node_factory> protocol,
                 std::shared_ptr<node_factory> communication);
  ~plugin_manager() override;
  plugin_manager(plugin_manager const&) = delete;
  plugin_manager(plugin_manager&&) = delete;
  plugin_manager& operator=(plugin_manager const&) = delete;
  plugin_manager& operator=(plugin_manager&&) = delete;

  ///
  /// \brief access which plugins are known
  /// \return a list of known plugins
  ///
  auto known_plugins() const -> std::map<boost::uuids::uuid, std::shared_ptr<plugin_data>>;
  ///
  /// \brief read a plugin description
  ///
  /// This method reads a plugin description from a provided location on disk. In case the file
  /// could not be read, this method returns a nullptr.
  /// \param path the location of the plugin description to read,
  /// \return the read plugin description.
  ///
  auto read_plugin_description(boost::filesystem::path const& path) -> std::shared_ptr<plugin_data>;
  ///
  /// \brief query if plugin is loaded
  ///
  /// This method will tell the caller whether a plugin of the specified id is loaded.
  /// \param id the identifier of the plugin to query,
  /// \return true if the plugin is loaded.
  ///
  auto is_plugin_loaded(boost::uuids::uuid id) const -> bool;
  ///
  /// \brief load plugin
  ///
  /// This method will load a plugin. Nodes the plugin provides will be added to the respective
  /// factories.
  /// It will also update the settings file, so this plugin will be loaded automatically on next
  /// startup.
  /// \param id the identifier of the plugin to load,
  /// \return true if the plugin could be loaded.
  ///
  auto load_plugin(boost::uuids::uuid id) -> bool;
  ///
  /// \brief unload plugin
  ///
  /// This method will unload a plugin. Nodes the plugin provides will be removed from the
  /// respective factories.
  /// It will also update the settings file, so this plugin will no longer automatically load on
  /// next startup.
  /// \param id the identifier of the plugin to unload,
  /// \return true if the plugin could be unloaded.
  ///
  auto unload_plugin(boost::uuids::uuid id) -> bool;
  ///
  /// \brief add plugin
  ///
  /// This method will add a plugin description to the list of known plugins. This will also
  /// update the settings file, so the plugin is known on next startup.
  /// \param data the plugin description to add to the manager,
  /// \return true if the operation was successful.
  ///
  auto add_plugin(std::shared_ptr<plugin_data> data) -> bool;
  ///
  /// \brief remove plugin
  ///
  /// This method will remove a plugin from the list of known plugins. If the plugin is currently
  /// loaded, it will be unloaded first.
  /// This will also update the settings file, so the plugin is no longer visible even after the
  /// next startup.
  /// \param id the identifier of the plugin to remove,
  /// \return true if the operation was successful.
  ///
  auto remove_plugin(boost::uuids::uuid id) -> bool;

private:
  class impl;
  std::unique_ptr<impl> _p;
};
} // namespace plugin
} // namespace yoyo
