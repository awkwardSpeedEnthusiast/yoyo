#include "plugin_manager.h"
#include "plugin_data_reader.hpp"
#include "plugin_settings.hpp"

#include "yoyo/node_factory.h"
#include "yoyo/yoyo_plugin.h"

#include <QPluginLoader>

namespace yoyo::plugin
{
class plugin_manager::impl
{
public:
  impl(std::shared_ptr<node_factory> data, std::shared_ptr<node_factory> gui,
       std::shared_ptr<node_factory> protocol, std::shared_ptr<node_factory> communication)
    : _data { std::move(data) }
    , _gui { std::move(gui) }
    , _protocol { std::move(protocol) }
    , _communication { std::move(communication) }
  {
  }

  ~impl()
  {
    while (!_loaded_plugins.empty()) {
      unload_the_plugin(_loaded_plugins.begin()->first);
    }
  }

  auto load_the_plugin(boost::uuids::uuid id) -> bool;
  auto unload_the_plugin(boost::uuids::uuid id) -> bool;
  std::shared_ptr<node_factory> _data;
  std::shared_ptr<node_factory> _gui;
  std::shared_ptr<node_factory> _protocol;
  std::shared_ptr<node_factory> _communication;

  std::map<boost::uuids::uuid, std::shared_ptr<plugin_data>> _known_plugins;
  std::map<boost::uuids::uuid, std::shared_ptr<QPluginLoader>> _loaded_plugins;
  std::vector<std::pair<boost::filesystem::path, bool>> paths;
};

plugin_manager::plugin_manager(std::shared_ptr<node_factory> data,
                               std::shared_ptr<node_factory> gui,
                               std::shared_ptr<node_factory> protocol,
                               std::shared_ptr<node_factory> communication)
  : _p { std::make_unique<impl>(std::move(data), std::move(gui), std::move(protocol),
                                std::move(communication)) }
{
  _p->paths = read_settings();
  std::for_each(_p->paths.begin(), _p->paths.end(), [this](auto const& path) {
    if (auto description = read_plugin_description(path.first)) {
      _p->_known_plugins.insert({ description->id(), description });

      if (path.second) {
        _p->load_the_plugin(description->id());
      }
    }
  });
}

plugin_manager::~plugin_manager() = default;

auto plugin_manager::known_plugins() const
  -> std::map<boost::uuids::uuid, std::shared_ptr<plugin_data>>
{
  return _p->_known_plugins;
}

auto plugin_manager::read_plugin_description(boost::filesystem::path const& path)
  -> std::shared_ptr<plugin_data>
{
  return read(path);
}

auto plugin_manager::is_plugin_loaded(boost::uuids::uuid id) const -> bool
{
  return _p->_loaded_plugins.find(id) != _p->_loaded_plugins.end();
}

auto plugin_manager::load_plugin(boost::uuids::uuid id) -> bool
{
  auto it = _p->_known_plugins.find(id);

  if (it == _p->_known_plugins.end()) {
    return false;
  }

  if (_p->_loaded_plugins.find(id) != _p->_loaded_plugins.end()) {
    return false;
  }

  if (_p->load_the_plugin(id)) {
    auto pit =
      std::find_if(_p->paths.begin(), _p->paths.end(),
                   [f = it->second->description_location()](auto p) { return p.first == f; });
    pit->second = true;
    update_settings(_p->paths);
    return true;
  }

  return false;
}

auto plugin_manager::unload_plugin(boost::uuids::uuid id) -> bool
{
  if (_p->_loaded_plugins.find(id) == _p->_loaded_plugins.end()) {
    return false;
  }

  auto it = _p->_known_plugins.find(id);

  if (it == _p->_known_plugins.end()) {
    return false;
  }

  if (_p->unload_the_plugin(id)) {
    auto pit =
      std::find_if(_p->paths.begin(), _p->paths.end(),
                   [f = it->second->description_location()](auto p) { return p.first == f; });
    pit->second = false;
    update_settings(_p->paths);
    return true;
  }

  return false;
}

auto plugin_manager::add_plugin(std::shared_ptr<plugin_data> data) -> bool
{
  if (_p->_known_plugins.find(data->id()) != _p->_known_plugins.end()) {
    return false;
  }

  _p->_known_plugins.insert({ data->id(), data });
  _p->paths.push_back(std::make_pair(data->description_location(), false));
  update_settings(_p->paths);
  return true;
}

auto plugin_manager::remove_plugin(boost::uuids::uuid id) -> bool
{
  auto it = _p->_known_plugins.find(id);

  if (it == _p->_known_plugins.end()) {
    return false;
  }

  auto pit =
    std::find_if(_p->paths.begin(), _p->paths.end(),
                 [f = it->second->description_location()](auto p) { return p.first == f; });

  if (is_plugin_loaded(id)) {
    if (!unload_plugin(id)) {
      return false;
    }
  }

  if (pit != _p->paths.end()) {
    _p->paths.erase(pit);
    update_settings(_p->paths);
  }

  _p->_known_plugins.erase(it);
  return true;
}

auto plugin_manager::impl::load_the_plugin(boost::uuids::uuid id) -> bool
{
  auto it = _known_plugins.find(id);
  auto loader = std::make_shared<QPluginLoader>(
    QString::fromStdString(it->second->location().string<std::string>()));

  if (!loader->load()) {
    return false;
  }

  auto obj = loader->instance();

  if (auto myPlugin = dynamic_cast<yoyo_plugin*>(obj)) {
    myPlugin->install_data_nodes(*_data);
    myPlugin->install_gui_nodes(*_gui);
    myPlugin->install_protocol_nodes(*_protocol);
    myPlugin->install_communication_nodes(*_communication);
    _loaded_plugins.insert({ id, loader });
    return true;
  }

  loader->unload();
  return false;
}

auto plugin_manager::impl::unload_the_plugin(boost::uuids::uuid id) -> bool
{
  auto it = _loaded_plugins.find(id);

  if (it == _loaded_plugins.end()) {
    return false;
  }

  auto obj = it->second->instance();

  if (auto myPlugin = dynamic_cast<yoyo_plugin*>(obj)) {
    myPlugin->uninstall_data_nodes(*_data);
    myPlugin->uninstall_gui_nodes(*_gui);
    myPlugin->uninstall_protocol_nodes(*_protocol);
    myPlugin->uninstall_communication_nodes(*_communication);
  }

  it->second->unload();
  _loaded_plugins.erase(it);
  return true;
}
} // namespace yoyo::plugin
