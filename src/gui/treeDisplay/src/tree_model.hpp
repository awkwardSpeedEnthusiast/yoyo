#pragma once

#include <QAbstractItemModel>

#include <map>
#include <memory>

namespace yoyo
{
class node_base;
namespace gui
{
class tree_model : public QAbstractItemModel
{
  Q_OBJECT
public:
  explicit tree_model();
  ~tree_model() override;

  auto setRootNode(std::shared_ptr<node_base> root) -> void;

  auto index(int row, int column, QModelIndex const& parent = QModelIndex()) const
    -> QModelIndex override;
  auto parent(QModelIndex const& index) const -> QModelIndex override;

  auto columnCount(QModelIndex const& parent = QModelIndex()) const -> int override;
  auto rowCount(QModelIndex const& parent = QModelIndex()) const -> int override;
  auto data(QModelIndex const& index, int role = Qt::DisplayRole) const -> QVariant override;
  auto headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const
    -> QVariant override;

  auto mimeData(QModelIndexList const& indexes) const -> QMimeData* override;
  auto mimeTypes() const -> QStringList override;
  auto supportedDragActions() const -> Qt::DropActions override;
  auto supportedDropActions() const -> Qt::DropActions override;
  auto canDropMimeData(QMimeData const* data, Qt::DropAction action, int row, int column,
                       QModelIndex const& parent) const -> bool override;
  auto dropMimeData(QMimeData const* data, Qt::DropAction action, int row, int column,
                    QModelIndex const& parent) -> bool override;

  auto nodeForIndex(QModelIndex const& index) const -> std::shared_ptr<node_base>;
  auto indexForNode(std::shared_ptr<node_base> const& node) const -> QModelIndex;

private:
  auto createIndex(int row, int column, node_base* parent) const -> QModelIndex;
  auto calculateTarget(int row, QModelIndex const& index, Qt::DropAction action) const
    -> std::tuple<int, std::shared_ptr<node_base>>;

private:
  std::shared_ptr<node_base> _root_node;
};
} // namespace gui
} // namespace yoyo
