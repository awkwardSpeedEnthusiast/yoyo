#pragma once
#include "yoyo/node_factory.h"

#include <QAbstractListModel>

namespace yoyo
{
namespace gui
{
class tool_model : public QAbstractListModel
{
  Q_OBJECT

public:
  tool_model(std::shared_ptr<node_factory> gui_factory);
  ~tool_model() override = default;
  tool_model(tool_model const&) = delete;
  tool_model(tool_model&&) = delete;
  tool_model& operator=(tool_model const&) = delete;
  tool_model& operator=(tool_model&&) = delete;

  auto update() -> void;

  auto parent(const QModelIndex& index) const -> QModelIndex override;
  auto columnCount(const QModelIndex& parent = QModelIndex()) const -> int override;
  auto rowCount(const QModelIndex& parent = QModelIndex()) const -> int override;
  auto data(const QModelIndex& index, int role = Qt::DisplayRole) const -> QVariant override;
  auto mimeData(const QModelIndexList& indexes) const -> QMimeData* override;
  auto mimeTypes() const -> QStringList override;
  auto flags(QModelIndex const& index) const -> Qt::ItemFlags override;

private:
  std::shared_ptr<node_factory> _gui_factory;
  node_factory::node_id_list _cached_list;
};
} // namespace gui
} // namespace yoyo
