#include "tool_model.hpp"

#include <QDataStream>
#include <QFile>
#include <QIcon>
#include <QMimeData>

#include <boost/uuid/uuid_io.hpp>

namespace
{
const constexpr auto mimetype_gui_new = "application/yoyo/gui-new";
}
namespace yoyo
{
namespace gui
{
tool_model::tool_model(std::shared_ptr<node_factory> gui_factory)
  : QAbstractListModel {}
  , _gui_factory { gui_factory }
{
  _cached_list = _gui_factory->installed_nodes();
}

auto tool_model::update() -> void
{
  beginResetModel();
  _cached_list = _gui_factory->installed_nodes();
  endResetModel();
}

auto tool_model::parent(const QModelIndex&) const -> QModelIndex
{
  return {};
}

auto tool_model::columnCount(const QModelIndex&) const -> int
{
  return 1;
}

auto tool_model::rowCount(const QModelIndex&) const -> int
{
  return static_cast<int>(_cached_list.size());
}

auto tool_model::data(const QModelIndex& index, int role) const -> QVariant
{
  if (!index.isValid() || (static_cast<size_t>(index.row()) > _cached_list.size())) {
    return {};
  }

  switch (role) {
  case Qt::DisplayRole: return std::get<2>(_cached_list[index.row()]);
  case Qt::DecorationRole:
    auto fn =
      ":/yoyo/nodes/"
      + QString::fromStdString(boost::uuids::to_string(std::get<0>(_cached_list[index.row()])))
      + ".png";

    if (QFile::exists(fn)) {
      return QIcon(fn);
    }

    break;
  }

  return {};
}

auto tool_model::mimeData(const QModelIndexList& indexes) const -> QMimeData*
{
  if (indexes.count() != 1) {
    return nullptr;
  }

  if (!indexes[0].isValid() || (static_cast<size_t>(indexes[0].row()) > _cached_list.size())) {
    return nullptr;
  }

  auto mimedata = new QMimeData;
  mimedata->setData(mimetype_gui_new, QByteArray::fromStdString(boost::uuids::to_string(
                                        std::get<0>(_cached_list[indexes[0].row()]))));

  return mimedata;
}

auto tool_model::mimeTypes() const -> QStringList
{
  return QStringList {} << mimetype_gui_new;
}
} // namespace gui
} // namespace yoyo
