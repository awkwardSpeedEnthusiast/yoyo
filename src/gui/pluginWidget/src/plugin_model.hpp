#pragma once

#include <QAbstractListModel>

#include <memory>

namespace yoyo
{
namespace plugin
{
class plugin_data;
class plugin_manager;
} // namespace plugin
namespace gui
{
class plugin_model : public QAbstractListModel
{
public:
  plugin_model(std::shared_ptr<plugin::plugin_manager> manager, QObject* parent = nullptr);
  ~plugin_model() override;

  auto index(int row, int column, QModelIndex const& parent = QModelIndex()) const
    -> QModelIndex override;
  auto parent(QModelIndex const& index) const -> QModelIndex override;

  auto columnCount(QModelIndex const& parent = QModelIndex()) const -> int override;
  auto rowCount(QModelIndex const& parent = QModelIndex()) const -> int override;
  auto flags(QModelIndex const& index) const -> Qt::ItemFlags override;
  auto data(QModelIndex const& index, int role = Qt::DisplayRole) const -> QVariant override;

  auto setData(QModelIndex const& index, QVariant const& value, int role = Qt::EditRole)
    -> bool override;

  auto add_plugin(std::shared_ptr<plugin::plugin_data> data) -> void;
  //  auto context_menu_requested(QModelIndex const& index)->void;
  auto remove(QModelIndex const& index) -> void;

private:
  struct impl;
  std::unique_ptr<impl> _p;
};

} // namespace gui
} // namespace yoyo
