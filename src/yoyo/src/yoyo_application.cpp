#include "yoyo_application.hpp"
#include "file_management.hpp"
#include "yoyo_main_window.h"

#include "plugin_widget.h"
#include "property_widget.h"
#include "tool_widget.h"
#include "tree_widget.h"

#include "factory_provider.hpp"

#include "command_handler.h"
#include "connectivity_manager.hpp"
#include "plugin_manager.h"

#include "yoyo/communication_node.h"
#include "yoyo/data_node.h"
#include "yoyo/gui_node.h"
#include "yoyo/protocol_node.h"

#include <QSettings>

#include <string>

using std::string_literals::operator""s;

namespace
{
constexpr auto const index_data = 1;
constexpr auto const index_gui = 2;
constexpr auto const index_protocol = 3;
constexpr auto const index_communication = 4;
constexpr auto const index_fundamental = 0;
} // namespace
namespace yoyo
{
class yoyo_application::impl
{
public:
  impl();

  auto read_settings() -> void;
  auto store_settings() -> void;

  auto set_edit_mode(bool mode) -> void;
  auto show_plugins() -> void;

  auto close_configuration() -> void;
  auto add_configuration(std::shared_ptr<node_base> root) -> void;

  std::vector<std::shared_ptr<node_factory>> _factories;
  std::shared_ptr<plugin::plugin_manager> _plugin_manager;

  std::shared_ptr<connectivity_manager> _connectivity_manager;
  std::shared_ptr<node_base> _configuration;
  QString _configuration_file;
  boost::uuids::uuid _storage_type;

  std::unique_ptr<yoyo_main_window> _main_window;
  std::unique_ptr<gui::tool_widget> _tools;
  std::unique_ptr<gui::tree_widget> _tree;
  std::unique_ptr<gui::property_widget> _properties;
  std::unique_ptr<gui::plugin_widget> _plugins;
  std::unique_ptr<file_management> _file_management;

  bool _editmode { false };
};

yoyo_application::impl::impl()
{
  registerAllPropertyTypes();
  _factories.push_back(std::make_shared<node_factory>(&node_base::staticMetaObject));
  _factories.push_back(std::make_shared<node_factory>(&data_node::staticMetaObject));
  _factories.push_back(std::make_shared<node_factory>(&gui_node::staticMetaObject));
  _factories.push_back(std::make_shared<node_factory>(&protocol_node::staticMetaObject));
  _factories.push_back(std::make_shared<node_factory>(&communication_node::staticMetaObject));

  install_fundamental_nodes(*_factories[index_fundamental]);
  install_data_nodes(*_factories[index_data]);
  install_gui_nodes(*_factories[index_gui]);
  install_protocol_nodes(*_factories[index_protocol]);
  install_communication_nodes(*_factories[index_communication]);

  _plugin_manager = std::make_shared<plugin::plugin_manager>(
    _factories[index_data], _factories[index_gui], _factories[index_protocol],
    _factories[index_communication]);
}

yoyo_application::yoyo_application(int& argc, char** argv)
  : QApplication { argc, argv }
  , _p { std::make_unique<impl>() }
{
  setOrganizationName("Infineon");
  setOrganizationDomain("Yoyo");
  setApplicationName("Yoyo");
  setApplicationVersion(YOYO_VERSION);

  _p->_file_management = std::make_unique<file_management>(_p->_factories);
}

yoyo_application::~yoyo_application() = default;

auto yoyo_application::impl::read_settings() -> void
{
  QSettings settings { QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName() };
  if (!command::commandhandler()) {
    command::initialize_commandhandler(
      new command::command_handler(settings.value("command/stackSize", 10000).toULongLong()));
  }

  _main_window->restore_state(settings);
  set_edit_mode(_main_window->edit_mode());
}

auto yoyo_application::impl::store_settings() -> void
{
  QSettings settings { QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName() };
  _main_window->store_state(settings);
}

auto yoyo_application::impl::set_edit_mode(bool active) -> void
{
  _editmode = active;
  if (auto handler = command::commandhandler()) {
    handler->setEditMode(active);
  }
  if (!_configuration || _configuration->childCount() < 2) {
    return;
  }
  if (auto guiroot = std::dynamic_pointer_cast<gui_node>(_configuration->childAt(1))) {
    guiroot->setEditMode(active);
  }
}

auto yoyo_application::impl::show_plugins() -> void
{
  _tools->update();
}

auto yoyo_application::impl::close_configuration() -> void
{
  _main_window->set_central_widget(nullptr);
  _tree->setConfiguration({});
  _properties->itemSelected({}, {});
  _configuration.reset();
  _connectivity_manager.reset();
  _configuration_file = QString {};
  if (auto handler = command::commandhandler()) {
    handler->clearStack();
  }
}

auto yoyo_application::impl::add_configuration(std::shared_ptr<node_base> root) -> void
{
  _configuration = root;
  if (_configuration) {
    auto w = std::dynamic_pointer_cast<gui_node>(_configuration->childAt(1))->widget();
    _main_window->set_central_widget(w);
    _connectivity_manager = std::make_shared<connectivity_manager>(_configuration);
    _tree->setConfiguration(_configuration);
    if (auto guiroot = std::dynamic_pointer_cast<gui_node>(_configuration->childAt(1))) {
      guiroot->setEditMode(_editmode);
    }
  }
}

auto yoyo_application::setup() -> void
{
  _p->_main_window = std::make_unique<yoyo_main_window>();

  _p->_tree = std::make_unique<gui::tree_widget>(_p->_factories);
  _p->_tree->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  _p->_main_window->addDockWidget(Qt::LeftDockWidgetArea, _p->_tree.get(), Qt::Horizontal);

  _p->_tools = std::make_unique<gui::tool_widget>(_p->_factories[index_gui]);
  _p->_tools->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  _p->_tools->update();
  _p->_main_window->addDockWidget(Qt::LeftDockWidgetArea, _p->_tools.get(), Qt::Horizontal);

  _p->_properties = std::make_unique<gui::property_widget>();
  _p->_properties->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
  _p->_main_window->addDockWidget(Qt::LeftDockWidgetArea, _p->_properties.get(), Qt::Horizontal);

  connect(_p->_tree.get(), &gui::tree_widget::itemSelected, _p->_properties.get(),
          [this](auto item) {
            if (!item) {
              _p->_properties->itemSelected({}, {});
              return;
            }
            auto doc = [this, id = item->staticTypeId()]() -> std::shared_ptr<documentation> {
              for (auto const& f : _p->_factories) {
                if (auto d = f->node_documentation(id)) {
                  return d;
                }
              }
              return {};
            }();

            _p->_properties->itemSelected(item, doc);
          });

  connect(_p->_main_window.get(), &yoyo_main_window::newFile_requested, this, [this]() {
    _p->close_configuration();
    _p->add_configuration(_p->_file_management->new_file());
  });
  connect(_p->_main_window.get(), &yoyo_main_window::openFile_requested, this, [this]() {
    auto c = _p->_file_management->open_file();
    if (std::get<1>(c)) {
      _p->close_configuration();
      _p->add_configuration(std::get<1>(c));
      _p->_configuration_file = std::get<0>(c);
      _p->_storage_type = std::get<2>(c);
    }
  });
  connect(_p->_main_window.get(), &yoyo_main_window::saveFile_requested, this, [this]() {
    _p->_file_management->save_file(_p->_configuration, _p->_configuration_file, _p->_storage_type);
  });
  connect(_p->_main_window.get(), &yoyo_main_window::saveFileAs_requested, this, [this]() {
    auto c = _p->_file_management->save_file_as(_p->_configuration);
    _p->_configuration_file = std::get<0>(c);
    _p->_storage_type = std::get<1>(c);
  });
  connect(_p->_main_window.get(), &yoyo_main_window::closeFile_requested, this,
          [this]() { _p->close_configuration(); });
  connect(_p->_main_window.get(), &yoyo_main_window::recentFile_requested, this,
          [this](auto filename) {
            auto [c, s] = _p->_file_management->open_file(filename);
            if (!c) {
              return;
            }
            _p->close_configuration();
            _p->add_configuration(c);
            _p->_configuration_file = filename;
            _p->_storage_type = s;
          });
  connect(_p->_main_window.get(), &yoyo_main_window::exit_requested, this, [this]() {
    _p->store_settings();
    _p->close_configuration();
    this->quit();
  });
  connect(_p->_main_window.get(), &yoyo_main_window::editMode, this,
          [this](bool active) { _p->set_edit_mode(active); });
  connect(_p->_main_window.get(), &yoyo_main_window::tools_requested, this,
          [this](bool active) { _p->_tools->setVisible(active); });
  connect(_p->_main_window.get(), &yoyo_main_window::tree_requested, this,
          [this](bool active) { _p->_tree->setVisible(active); });
  connect(_p->_main_window.get(), &yoyo_main_window::properties_requested, this,
          [this](bool active) { _p->_properties->setVisible(active); });
  connect(_p->_main_window.get(), &yoyo_main_window::plugins_requested, this,
          [this]() { _p->show_plugins(); });
  connect(_p->_main_window.get(), &yoyo_main_window::log_requested, this, [](bool /*active*/) {});
  connect(_p->_main_window.get(), &yoyo_main_window::help_requested, this, [](bool /*active*/) {});

  connect(this, &QCoreApplication::aboutToQuit, this, [this]() {
    _p->store_settings();
    _p->close_configuration();
  });

  connect(_p->_file_management.get(), &file_management::historyChanged, _p->_main_window.get(),
          &yoyo_main_window::history_changed);

  _p->read_settings();
  _p->_main_window->setup_undo();
  _p->_main_window->show();
}

} // namespace yoyo
