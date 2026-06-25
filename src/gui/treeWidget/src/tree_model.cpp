#include "tree_model.hpp"

#include "data_adapter.hpp"
#include "tree_utils.hpp"

#include "command_handler.h"
#include "yoyo/communication_node.h"
#include "yoyo/data_node.h"
#include "yoyo/gui_node.h"
#include "yoyo/node_base.h"
#include "yoyo/protocol_node.h"

#include <QMimeData>

#include <string>

using std::string_literals::operator""s;
namespace
{
const constexpr auto mimetype_data_element = "application/yoyo/data-element";
const constexpr auto mimetype_gui_element = "application/yoyo/gui-element";
const constexpr auto mimetype_protocol_element = "application/yoyo/protocol-element";
const constexpr auto mimetype_communication_element = "application/yoyo/communication-element";
} // namespace

namespace yoyo::gui
{
tree_model::tree_model()
  : QAbstractItemModel {}
{
}

tree_model::~tree_model() = default;

auto tree_model::index(int row, int column, QModelIndex const& parent) const -> QModelIndex
{
  if (!_root_node) {
    return {};
  }

  node_base* item;

  if (!parent.isValid()) {
    item = _root_node.get();
  } else {
    auto p = parent.internalPointer();

    if (!p) {
      return {};
    }

    item = static_cast<node_base*>(p);
  }

  if (static_cast<size_t>(row) < item->childCount()) {
    return QAbstractItemModel::createIndex(row, column,
                                           static_cast<void*>(item->childAt(row).get()));
  }

  return {};
}

auto tree_model::createIndex(int row, int column, node_base* parent) const -> QModelIndex
{
  if (!parent) {
    return {};
  }

  if (parent == _root_node.get()) {
    return QAbstractItemModel::createIndex(row, column,
                                           static_cast<void*>(parent->childAt(row).get()));
  }

  auto parent_parent = parent->parent().lock();
  return index(row, column,
               createIndex(parent_parent->childIndex(parent->shared_from_this()), column,
                           parent_parent.get()));
}

auto tree_model::parent(QModelIndex const& index) const -> QModelIndex
{
  if (index.isValid()) {
    auto p = index.internalPointer();

    if (!p) {
      return {};
    }

    auto node = static_cast<node_base*>(p);
    auto parent = node->parent().lock();

    if (parent == _root_node) {
      return {};
    }

    auto parent_parent = parent->parent().lock();
    return createIndex(parent_parent->childIndex(parent->shared_from_this()), 0,
                       parent_parent.get());
  }

  return {};
}

auto tree_model::columnCount(QModelIndex const& /*parent*/) const -> int
{
  return _root_node ? 4 : 0;
}

auto tree_model::rowCount(QModelIndex const& parent) const -> int
{
  if (!parent.isValid()) {
    return _root_node ? _root_node->childCount() : 0;
  }

  auto p = parent.internalPointer();

  if (!p) {
    return {};
  }

  auto node = static_cast<node_base*>(p);
  return node->childCount();
}

auto tree_model::data(QModelIndex const& index, int role) const -> QVariant
{
  if (!index.isValid() || (role != Qt::DisplayRole)) {
    return {};
  }

  auto p = index.internalPointer();

  if (!p) {
    return {};
  }

  auto node = static_cast<node_base*>(p);
  return yoyo::gui::create_data_adapter(node)->data(index.column());
}

auto tree_model::headerData(int section, Qt::Orientation /*orientation*/, int role) const
  -> QVariant
{
  if (role != Qt::DisplayRole) {
    return {};
  }

  switch (section) {
  case 0: return tr("Path");
  case 1: return tr("Object");
  case 2: return tr("Title");
  case 3: return tr("Update");
  }

  return {};
}

auto tree_model::flags(const QModelIndex& index) const -> Qt::ItemFlags
{
  if (!index.isValid()) {
    return QAbstractItemModel::flags(index);
  }

  auto node = nodeForIndex(index);
  if (!node) {
    return QAbstractItemModel::flags(index);
  }

  if (node->metaObject()->inherits(&data_node::staticMetaObject)
      || node->metaObject()->inherits(&gui_node::staticMetaObject)) {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled
         | QAbstractItemModel::flags(index);
  }
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled
       | QAbstractItemModel::flags(index);
}

auto tree_model::mimeData(QModelIndexList const& indexes) const -> QMimeData*
{
  if (indexes.count() != 1) {
    return nullptr;
  }

  auto p = indexes[0].internalPointer();

  if (!p) {
    return nullptr;
  }

  auto node = static_cast<node_base*>(p);
  return yoyo::gui::create_data_adapter(node)->mimedata();
}

auto tree_model::mimeTypes() const -> QStringList
{
  return QStringList {} << mimetype_data_element << mimetype_gui_element
                        << mimetype_protocol_element << mimetype_communication_element;
}

auto tree_model::supportedDragActions() const -> Qt::DropActions
{
  return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
}

auto tree_model::supportedDropActions() const -> Qt::DropActions
{
  return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
}

auto tree_model::calculateTarget(int row, QModelIndex const& index, Qt::DropAction action) const
  -> std::tuple<int, std::shared_ptr<node_base>>
{
  if (!index.isValid()) {
    return { row, _root_node };
  }

  auto p = static_cast<node_base*>(index.internalPointer());

  if (!p) {
    return {};
  }

  if (row < 0) {
    if (p->acceptsChildren() && (action != Qt::LinkAction)) {
      return { -1, p->shared_from_this() };
    }

    if (auto parent = p->parent().lock()) {
      return { parent->childIndex(p->shared_from_this()) + 1, parent };
    }

    return {};
  }

  return { row, p->shared_from_this() };
}

auto tree_model::canDropMimeData(QMimeData const* data, Qt::DropAction action, int row,
                                 int /*column*/, QModelIndex const& parent) const -> bool
{
  if (!data
      || !(data->hasFormat(mimetype_data_element) || data->hasFormat(mimetype_gui_element)
           || data->hasFormat(mimetype_protocol_element)
           || data->hasFormat(mimetype_communication_element))) {
    return false;
  }

  if (action == Qt::CopyAction) {
    // cloning not yet available
    return false;
  }

  auto [i, target] = calculateTarget(row, parent, action);

  if (!target) {
    return false;
  }

  // not yet allowing to reorder protocols or communication.
  auto meta = target->metaObject();
  return (data->hasFormat(mimetype_data_element)
          && (meta->inherits(&data_node::staticMetaObject)
              || meta->className() == "yoyo::data_root"s))
      || (data->hasFormat(mimetype_gui_element)
          && (meta->inherits(&gui_node::staticMetaObject)
              || meta->className() == "yoyo::gui_root"s))
      || (data->hasFormat(mimetype_protocol_element)
          && meta->inherits(&protocol_node::staticMetaObject))
      || (data->hasFormat(mimetype_communication_element)
          && meta->inherits(&communication_node::staticMetaObject));
}

auto tree_model::dropMimeData(QMimeData const* data, Qt::DropAction action, int row, int /*column*/,
                              QModelIndex const& parent) -> bool
{
  if (!data
      || !(data->hasFormat(mimetype_data_element) || data->hasFormat(mimetype_gui_element)
           || data->hasFormat(mimetype_protocol_element)
           || data->hasFormat(mimetype_communication_element))) {
    return false;
  }

  if (action == Qt::CopyAction) {
    // cloning not yet available
    return false;
  }

  auto [i, target] = calculateTarget(row, parent, action);

  std::shared_ptr<node_base> source;

  if (data->hasFormat(mimetype_data_element)) {
    source =
      create_data_adapter(target.get())->deserialize(data->data(mimetype_data_element), _root_node);
  } else if (data->hasFormat(mimetype_gui_element)) {
    source =
      create_data_adapter(target.get())->deserialize(data->data(mimetype_gui_element), _root_node);
  } else if (data->hasFormat(mimetype_protocol_element)) {
    source = create_data_adapter(target.get())
               ->deserialize(data->data(mimetype_protocol_element), _root_node);
  } else if (data->hasFormat(mimetype_communication_element)) {
    source = create_data_adapter(target.get())
               ->deserialize(data->data(mimetype_communication_element), _root_node);
  }

  if (!source) {
    return false;
  }

  if (target == source->parent().lock()) {
    if (i >= target->childIndex(source)) {
      i--;
    }

    if (auto handler = command::commandhandler()) {
      handler->execute({
        tr("Move item %1 position").arg(source->name()._s),
        [t = target->weak_from_this(), s = source->weak_from_this(), index = i]() {
          auto target = t.lock();
          auto source = s.lock();

          if (target && source) {
            target->moveChild(source, (index < 0 ? target->childCount() : index));
          }
        },
        [t_path = utilities::calculatePath<utilities::path_strategy_t::INDEX>(target), at = i,
         index = target->childIndex(source), this]() {
          auto target =
            utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(t_path, _root_node);

          if (target) {
            auto source = target->childAt((at < 0 ? target->childCount() - 1 : at));
            target->moveChild(source, index);
          }
        },
        [t_path = utilities::calculatePath<utilities::path_strategy_t::INDEX>(target),
         at = target->childIndex(source), index = i, this]() {
          auto target =
            utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(t_path, _root_node);

          if (target) {
            auto source = target->childAt(at);
            target->moveChild(source, (index < 0 ? target->childCount() : index));
          }
        },
      });
    } else {
      target->moveChild(source, i);
    }
  } else {
    if (auto handler = command::commandhandler()) {
      auto parent = source->parent().lock();
      handler->execute({
        tr("Move item %1 position").arg(source->name()._s),
        [t = target->weak_from_this(), s = source->weak_from_this(), index = i]() {
          auto target = t.lock();
          auto source = s.lock();

          if (target && source) {
            target->addChild(source, index);
          }
        },
        [t_path = utilities::calculatePath<utilities::path_strategy_t::INDEX>(parent),
         p_path = utilities::calculatePath<utilities::path_strategy_t::INDEX>(target), at = i,
         index = parent->childIndex(source), this]() {
          auto target =
            utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(t_path, _root_node);
          auto parent =
            utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(p_path, _root_node);

          if (target && parent) {
            auto source = parent->childAt((at < 0 ? parent->childCount() - 1 : at));
            target->addChild(source, index);
          }
        },
        [t_path = utilities::calculatePath<utilities::path_strategy_t::INDEX>(target),
         s_path = utilities::calculatePath<utilities::path_strategy_t::INDEX>(source), index = i,
         this]() {
          auto target =
            utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(t_path, _root_node);
          auto source =
            utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(s_path, _root_node);

          if (target && source) {
            target->addChild(source, index);
          }
        },
      });
    } else {
      target->addChild(source, i);
    }
  }

  return true;
}

auto tree_model::setRootNode(std::shared_ptr<node_base> root) -> void
{
  beginResetModel();

  if (_root_node) {
    disconnect(_root_node.get(), &node_base::treeChanged, this, nullptr);
  }

  _root_node = root;
  if (_root_node) {
    connect(_root_node.get(), &node_base::treeChanged, this,
            [this](auto changed_node, auto child_node, auto old_index, auto mode) {
              auto node = changed_node.lock();
              auto child = child_node.lock();
              auto node_index = indexForNode(node);
              switch (mode) {
              case node_base::ChangeOperation::PRE_REMOVE: {
                beginRemoveRows(node_index, old_index, old_index);
                return;
              }
              case node_base::ChangeOperation::REMOVED: {
                endRemoveRows();
                return;
              }
              case node_base::ChangeOperation::PRE_ADD: {
                beginInsertRows(node_index, old_index, old_index);
                break;
              }
              case node_base::ChangeOperation::ADDED: {
                endInsertRows();
                break;
              }
              case node_base::ChangeOperation::PRE_MOVE: {
                auto ind = node->childIndex(child);
                beginMoveRows(node_index, ind, ind, node_index,
                              (old_index > ind ? old_index + 1 : old_index));
                break;
              }
              case node_base::ChangeOperation::MOVED: {
                endMoveRows();
                break;
              }
              }
            });
  }
  endResetModel();
}

auto tree_model::nodeForIndex(QModelIndex const& index) const -> std::shared_ptr<node_base>
{
  if (!index.isValid()) {
    return _root_node;
  }

  auto p = index.internalPointer();
  return static_cast<node_base*>(p)->shared_from_this();
}

auto tree_model::indexForNode(std::shared_ptr<node_base> const& node) const -> QModelIndex
{
  if (!node || (node == _root_node)) {
    return {};
  }

  auto parent = node->parent().lock();

  return index(parent->childIndex(node), 0, indexForNode(parent));
}
} // namespace yoyo::gui
