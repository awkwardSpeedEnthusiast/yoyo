#include "plugin_model.hpp"

#include "plugin_manager.h"

#include <QMenu>

#include <boost/uuid/uuid_io.hpp>

namespace yoyo::gui
{
struct plugin_model::impl {
  impl(std::shared_ptr<plugin::plugin_manager> manager)
    : _manager { manager }
  {
  }

  auto update()
  {
    _allData.clear();
    auto data = _manager->known_plugins();
    std::transform(data.begin(), data.end(), std::back_inserter(_allData),
                   [](auto const& e) { return e.second; });
  }
  std::shared_ptr<plugin::plugin_manager> _manager;
  std::vector<std::shared_ptr<plugin::plugin_data>> _allData;
  std::shared_ptr<QMenu> _menu;
};

plugin_model::plugin_model(std::shared_ptr<plugin::plugin_manager> manager, QObject* parent)
  : QAbstractListModel { parent }
  , _p { std::make_unique<impl>(manager) }
{
  _p->_menu = std::make_unique<QMenu>();
  _p->update();
}

plugin_model::~plugin_model() = default;

auto plugin_model::index(int row, int /*column*/, QModelIndex const& parent) const -> QModelIndex
{
  if (parent.isValid()) {
    return {};
  }
  if (row < 0 || static_cast<size_t>(row) >= _p->_allData.size()) {
    return {};
  }
  return createIndex(row, 0);
}

auto plugin_model::parent(QModelIndex const& /*index*/) const -> QModelIndex
{
  return {};
}

auto plugin_model::columnCount(QModelIndex const& /*parent*/) const -> int
{
  return 1;
}

auto plugin_model::rowCount(QModelIndex const& parent) const -> int
{
  if (parent.isValid()) {
    return 0;
  }
  return _p->_allData.size();
}

auto plugin_model::flags(QModelIndex const& index) const -> Qt::ItemFlags
{
  if (!index.isValid()) {
    return Qt::NoItemFlags;
  }
  return Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsUserCheckable
       | Qt::ItemIsEnabled;
}

auto plugin_model::data(QModelIndex const& index, int role) const -> QVariant
{
  if (!index.isValid() || static_cast<size_t>(index.row()) >= _p->_allData.size()) {
    return {};
  }
  switch (role) {
  case Qt::DisplayRole: return _p->_allData[index.row()]->name();
  case Qt::ToolTipRole: return _p->_allData[index.row()]->description();
  case Qt::CheckStateRole:
    return _p->_manager->is_plugin_loaded(_p->_allData[index.row()]->id()) ? Qt::Checked
                                                                           : Qt::Unchecked;
  case Qt::UserRole:
    return QString::fromStdString(boost::uuids::to_string(_p->_allData[index.row()]->id()));
  }
  return {};
}

auto plugin_model::setData(QModelIndex const& index, QVariant const& value, int role) -> bool
{
  if (!index.isValid() || static_cast<size_t>(index.row()) >= _p->_allData.size()
      || role != Qt::CheckStateRole) {
    return false;
  }
  auto check = value.toBool();
  auto id = _p->_allData[index.row()]->id();
  auto result = false;
  if (check) {
    result = _p->_manager->load_plugin(id);
  } else {
    result = _p->_manager->unload_plugin(id);
  }
  if (result) {
    Q_EMIT dataChanged(index, index, { Qt::CheckStateRole });
  }
  return result;
}

auto plugin_model::add_plugin(std::shared_ptr<plugin::plugin_data> data) -> void
{
  auto plugins = _p->_manager->known_plugins();
  plugins.insert({ data->id(), data });
  auto dist = std::distance(plugins.begin(), plugins.find(data->id()));

  beginInsertRows({}, dist, dist + 1);
  _p->_manager->add_plugin(data);
  plugins = _p->_manager->known_plugins();
  _p->update();
  endInsertRows();
}

//  auto plugin_model::context_menu_requested(QModelIndex const& index)->void
//  {
//    if (!index.isValid() || static_cast<size_t>(index.row())>= _p->_allData.size()) {
//      return;
//    }
//    auto item = _p->_allData[index.row()];
//    QMenu menu;
//    if (_p->_manager->is_plugin_loaded(item->id())){
//      menu.addAction(tr("Unload plugin %1").arg(item->name()), [this, id = item->id(), index](){
//        if (_p->_manager->unload_plugin(id)) {
//          Q_EMIT dataChanged(index, index, {Qt::CheckStateRole});
//        }
//      });
//    } else {
//    menu.addAction(tr("Load plugin %1").arg(item->name()), [this, id = item->id(), index](){
//      if (_p->_manager->load_plugin(id)) {
//        Q_EMIT dataChanged(index, index, {Qt::CheckStateRole});
//      }
//      });
//    }
//    menu.addAction(tr("Remove plugin %1").arg(item->name()), [this, id = item->id(), index](){
//      beginRemoveRows({}, index.row(), index.row());
//      if (_p->_manager->remove_plugin(id)) {
//        _p->update();
//      }
//      endRemoveRows();
//    });
//    menu.exec();
//  }

auto plugin_model::remove(QModelIndex const& index) -> void
{
  if (!index.isValid() || static_cast<size_t>(index.row()) >= _p->_allData.size()) {
    return;
  }
  auto item = _p->_allData[index.row()];
  beginRemoveRows({}, index.row(), index.row());
  if (_p->_manager->remove_plugin(item->id())) {
    _p->update();
  }
  endRemoveRows();
}
} // namespace yoyo::gui
