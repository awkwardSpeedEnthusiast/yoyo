#include "file_management.hpp"

#include "factory_provider.hpp"
#include "io_manager.h"

#include "yoyo/node_factory.h"

#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QMetaMethod>
#include <QSettings>

#include <boost/filesystem.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <fstream>

namespace
{
auto create_filter_list(std::vector<yoyo::io::io_manager::io_info_t> const& ios) -> QString
{
  QString filters;
  for (auto const& info : ios) {
    filters += QString::fromStdString(std::get<1>(info) + " (" + std::get<3>(info) + ");;");
  }
  if (filters.length() > 2) {
    filters = filters.left(filters.length() - 2);
  }
  return filters;
}

auto id_from_filter(std::vector<yoyo::io::io_manager::io_info_t> const& ios,
                    QString const& selected) -> boost::uuids::uuid
{
  for (auto const& info : ios) {
    if (selected == QString::fromStdString(std::get<1>(info) + " (" + std::get<3>(info) + ")")) {
      return std::get<0>(info);
    }
  }
  return {};
}
} // namespace

namespace yoyo
{
class file_management::impl
{
public:
  impl(std::vector<std::shared_ptr<node_factory>> factories)
    : _factories { factories }
  {
    io::init_manager(std::make_shared<yoyo::io::io_manager>(_factories));
    io::add_default();
  }

  auto store_file(std::shared_ptr<node_base> root, boost::filesystem::path const& filename,
                  boost::uuids::uuid const& serializerId) -> void;
  auto read_file(boost::filesystem::path const& filename, boost::uuids::uuid const& deserializer)
    -> std::shared_ptr<node_base>;
  auto add_file_to_history(QString const& filename, boost::uuids::uuid id, file_management* parent)
    -> void;

  std::vector<std::shared_ptr<node_factory>> _factories;

  QString _last_directory;
  std::vector<std::pair<QString, boost::uuids::uuid>> _file_history;
  size_t _max_history;
};

file_management::file_management(std::vector<std::shared_ptr<node_factory>> factories)
  : _p { std::make_unique<impl>(factories) }
{
  QSettings settings { QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName() };
  _p->_last_directory = settings.value("file_management/lastSelectedDirectory").toString();
  _p->_max_history = settings.value("file_management/maxHistory", 20).toULongLong();
  int size = settings.beginReadArray("file_management/history");
  for (int i = 0; i < size; ++i) {
    settings.setArrayIndex(i);
    auto filename = settings.value("path").toString();
    auto id = settings.value("id").toString();
    _p->_file_history.push_back({ filename, boost::uuids::string_generator {}(id.toStdString()) });
  }
  settings.endArray();
}

file_management::~file_management() = default;

auto file_management::new_file() -> std::shared_ptr<node_base>
{
  auto configuration = _p->_factories[0]->createNode(fundamental::configuration_id, {});
  configuration->addChild(_p->_factories[0]->createNode(fundamental::data_root_id, {}));
  configuration->addChild(_p->_factories[0]->createNode(fundamental::gui_root_id, {}));
  return configuration;
}

auto file_management::save_file(std::shared_ptr<node_base> root, QString const& filename,
                                boost::uuids::uuid io_id) -> void
{
  auto manager = io::manager().lock();
  if (!manager) {
    return;
  }
  if (filename.isEmpty() || io_id.is_nil()) {
    return;
  }
  auto file = boost::filesystem::path { filename.toStdString() };
  _p->store_file(root, file, io_id);
}

auto file_management::save_file_as(std::shared_ptr<node_base> root)
  -> std::tuple<QString, boost::uuids::uuid>
{
  auto manager = io::manager().lock();
  if (!manager) {
    return {};
  }
  QString selectedFilter;
  auto url = QFileDialog::getSaveFileName(nullptr, "Save configuration as", _p->_last_directory,
                                          create_filter_list(manager->installed_serializers()),
                                          &selectedFilter);

  if (url.isEmpty() || selectedFilter.isEmpty()) {
    return {};
  }

  auto id = id_from_filter(manager->installed_serializers(), selectedFilter);
  if (!id.is_nil()) {
    _p->store_file(root, url.toStdString(), id);
    _p->add_file_to_history(url, id, this);
    return { url, id };
  }
  return {};
}

auto file_management::open_file()
  -> std::tuple<QString, std::shared_ptr<node_base>, boost::uuids::uuid>
{
  auto manager = io::manager().lock();
  if (!manager) {
    return {};
  }
  QString selectedFilter;
  auto url = QFileDialog::getOpenFileName(nullptr, "Open configuration", _p->_last_directory,
                                          create_filter_list(manager->installed_serializers()),
                                          &selectedFilter);

  if (url.isEmpty() || selectedFilter.isEmpty()) {
    return {};
  }

  auto id = id_from_filter(manager->installed_serializers(), selectedFilter);
  if (!id.is_nil()) {
    auto content = _p->read_file(url.toStdString(), id);
    _p->add_file_to_history(url, id, this);
    return { url, content, id };
  }
  return {};
}

auto file_management::open_file(QString const& filename)
  -> std::tuple<std::shared_ptr<node_base>, boost::uuids::uuid>
{
  auto it = std::find_if(_p->_file_history.begin(), _p->_file_history.end(),
                         [filename](auto item) { return (item.first == filename); });
  if (it == _p->_file_history.end() || it->second.is_nil()) {
    return {};
  }
  auto content = _p->read_file(filename.toStdString(), it->second);
  _p->add_file_to_history(filename, it->second, this);
  return { content, it->second };
}

auto file_management::connectNotify(QMetaMethod const& signal) -> void
{
  if (signal.name() == "historyChanged") {
    std::vector<QString> history;
    std::transform(_p->_file_history.begin(), _p->_file_history.end(), std::back_inserter(history),
                   [](auto item) { return item.first; });
    Q_EMIT historyChanged(history);
  }
}

auto file_management::impl::store_file(std::shared_ptr<node_base> root,
                                       boost::filesystem::path const& filename,
                                       boost::uuids::uuid const& serializerId) -> void
{
  auto manager = io::manager().lock();
  if (!manager) {
    return;
  }
  if (auto serializer = manager->serializer(serializerId)) {
    std::ofstream output;
    output.open(filename.string());
    serializer->serialize(output, root);
    output.flush();
    output.close();
  }
}

auto file_management::impl::read_file(boost::filesystem::path const& filename,
                                      boost::uuids::uuid const& deserializerId)
  -> std::shared_ptr<node_base>
{
  auto manager = io::manager().lock();
  if (!manager) {
    return {};
  }
  if (auto deserializer = manager->deserializer(deserializerId)) {
    std::ifstream input;
    input.open(filename.string());
    auto result = deserializer->deserialize(input);
    input.close();
    return result;
  }
  return {};
}

auto file_management::impl::add_file_to_history(QString const& filename, boost::uuids::uuid id,
                                                file_management* parent) -> void
{
  auto it = std::find_if(_file_history.begin(), _file_history.end(),
                         [filename](auto element) { return element.first == filename; });
  if (it == _file_history.end()) {
    _file_history.insert(_file_history.begin(), std::make_pair(filename, id));
  } else if (it->second == id) {
    std::move(it, it, _file_history.begin());
  } else {
    _file_history.erase(it);
    _file_history.insert(_file_history.begin(), std::make_pair(filename, id));
  }
  while (_file_history.size() > _max_history) {
    _file_history.pop_back();
  }
  _last_directory = QFileInfo { filename }.absoluteDir().path();

  QSettings settings { QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(),
                       QApplication::applicationName() };
  settings.setValue("file_management/lastSelectedDirectory", _last_directory);

  settings.beginWriteArray("file_management/history");
  for (size_t i = 0; i < _file_history.size(); ++i) {
    settings.setArrayIndex(i);
    settings.setValue("path", _file_history[i].first);
    settings.setValue("id",
                      QString::fromStdString(boost::uuids::to_string(_file_history[i].second)));
  }
  settings.endArray();

  std::vector<QString> history;
  std::transform(_file_history.begin(), _file_history.end(), std::back_inserter(history),
                 [](auto item) { return item.first; });
  Q_EMIT parent->historyChanged(history);
}
} // namespace yoyo
