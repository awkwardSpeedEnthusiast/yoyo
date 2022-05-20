#pragma once
#include "yoyo/node_factory.h"

#include <QAbstractListModel>

namespace yoyo
{
namespace gui
{
class toolModel : public QAbstractListModel
{
  Q_OBJECT

public:
  toolModel(std::shared_ptr<node_factory> gui_factory);
  ~toolModel() override = default;
  toolModel(toolModel const&) = delete;
  toolModel(toolModel&&) = delete;
  toolModel& operator=(toolModel const&) = delete;
  toolModel& operator=(toolModel&&) = delete;

  auto update() -> void;

  auto parent(const QModelIndex& index) const -> QModelIndex override;
  auto columnCount(const QModelIndex& parent = QModelIndex()) const -> int override;
  auto rowCount(const QModelIndex& parent = QModelIndex()) const -> int override;
  auto data(const QModelIndex& index, int role = Qt::DisplayRole) const -> QVariant override;
  auto mimeData(const QModelIndexList& indexes) const -> QMimeData* override;
  auto mimeTypes() const -> QStringList override;

private:
  std::shared_ptr<node_factory> _gui_factory;
  node_factory::node_id_list _cached_list;
};
} // namespace gui
} // namespace yoyo
