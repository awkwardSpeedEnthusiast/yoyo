#include "contextmenu_handler.hpp"

#include "tree_utils.hpp"

#include "command_handler.h"
#include "yoyo/data_node.h"
#include "yoyo/gui_node.h"
#include "yoyo/node_base.h"
#include "yoyo/node_factory.h"

#include <QMenu>
#include <QMetaProperty>
#include <QWidget>

#include <set>
#include <string>
using std::string_literals::operator""s;

namespace
{
auto const data_root_name = "yoyo::data_root"s;
auto const gui_root_name = "yoyo::gui_root"s;
auto const configuration_name = "yoyo::configuration"s;

auto get_root(std::shared_ptr<yoyo::node_base> node) -> std::shared_ptr<yoyo::node_base>
{
  if (!node) {
    return {};
  }

  auto result = node;

  while (result->parent().lock()) {
    result = result->parent().lock();
  }

  return result;
}

auto isGui(std::shared_ptr<yoyo::node_base> node) -> bool
{
  if (!node) {
    return false;
  }

  auto meta = node->metaObject();
  return meta->className() == gui_root_name || meta->inherits(&yoyo::gui_node::staticMetaObject);
}

auto isData(std::shared_ptr<yoyo::node_base> node) -> bool
{
  if (!node) {
    return false;
  }

  auto meta = node->metaObject();
  return meta->className() == data_root_name || meta->inherits(&yoyo::data_node::staticMetaObject);
}

auto canBeRemoved(std::shared_ptr<yoyo::node_base> node) -> bool
{
  if (!node) {
    return false;
  }

  static std::set<std::string> const nonRemovables { data_root_name, configuration_name,
                                                     gui_root_name };
  return nonRemovables.find(node->metaObject()->className()) == nonRemovables.end();
}

auto hasChildrenToRemove(std::shared_ptr<yoyo::node_base> node) -> bool
{
  if (!node) {
    return false;
  }

  return !node->parent().expired() && node->childCount() > 0;
}

auto addChildCommand(std::shared_ptr<yoyo::node_base> target,
                     std::shared_ptr<yoyo::node_factory> factory, boost::uuids::uuid type_id,
                     size_t index) -> void
{
  auto source = factory->createNode(type_id, {});

  if (auto handler = yoyo::command::commandhandler()) {
    handler->execute({
      QObject::tr("Add new item to %1").arg(target->type()),
      [target, source, index]() {
        if (target && source) {
          target->addChild(source, index);
        }
      },
      [target, source]() {
        if (target && source) {
          target->removeChild(source);
        }
      },
      [target, source, index]() {
        if (target && source) {
          target->addChild(source, index);
        }
      },
    });
  } else {
    target->addChild(source, index);
  }
}

auto removeChildCommand(std::shared_ptr<yoyo::node_base> parent,
                        std::shared_ptr<yoyo::node_base> source) -> void
{
  if (auto handler = yoyo::command::commandhandler()) {
    handler->execute({
      QObject::tr("Remove item from %1").arg(parent->type()),
      [parent, source]() {
        if (parent && source) {
          parent->removeChild(source);
        }
      },
      [parent, source, index = parent->childIndex(source)]() {
        if (parent && source) {
          parent->addChild(source, index);
        }
      },
      [parent, source]() {
        if (parent && source) {
          parent->removeChild(source);
        }
      },
    });
  } else {
    parent->removeChild(source);
  }
}

auto removeChildrenCommand(std::shared_ptr<yoyo::node_base> node) -> void
{
  if (auto handler = yoyo::command::commandhandler()) {
    std::vector<std::shared_ptr<yoyo::node_base>> childlist;
    std::copy(node->begin(), node->end(), std::back_inserter(childlist));
    handler->execute({
      QObject::tr("Remove all children from %1").arg(node->type()),
      [node]() {
        if (node) {
          while (node->childCount() > 0) {
            node->removeChild(0);
          }
        }
      },
      [node, childlist = std::move(childlist)]() {
        if (node) {
          std::for_each(childlist.begin(), childlist.end(), [node](auto c) { node->addChild(c); });
        }
      },
      [node]() {
        if (node) {
          while (node->childCount() > 0) {
            node->removeChild(0);
          }
        }
      },
    });
  } else {
    while (node->childCount() > 0) {
      node->removeChild(0);
    }
  }
}

auto addActionsToAdd(QMenu* menu, std::shared_ptr<yoyo::node_base> parent,
                     std::shared_ptr<yoyo::node_factory> target_factory) -> void
{
  auto nodelist = target_factory->installed_nodes();
  std::for_each(nodelist.begin(), nodelist.end(), [menu, parent, target_factory](auto node) {
    auto add = menu->addAction(std::get<2>(node));
    QObject::connect(add, &QAction::triggered, parent.get(),
                     [parent, id = std::get<0>(node), target_factory]() {
                       addChildCommand(parent, target_factory, id, -1);
                     });
  });
}

auto addActionsToSibling(QMenu* menuBefore, QMenu* menuAfter,
                         std::shared_ptr<yoyo::node_base> parent, size_t index,
                         std::shared_ptr<yoyo::node_factory> source_factory,
                         std::shared_ptr<yoyo::node_factory> target_factory) -> void
{
  auto nodelist =
    source_factory->child_list(parent->staticTypeId(), target_factory->installed_nodes());
  std::for_each(nodelist.begin(), nodelist.end(),
                [menuBefore, menuAfter, parent, target_factory, index](auto node) {
                  auto before = menuBefore->addAction(std::get<2>(node));
                  QObject::connect(before, &QAction::triggered, parent.get(),
                                   [parent, id = std::get<0>(node), target_factory, index]() {
                                     addChildCommand(parent, target_factory, id, index);
                                   });
                  auto after = menuAfter->addAction(std::get<2>(node));
                  QObject::connect(after, &QAction::triggered, parent.get(),
                                   [parent, id = std::get<0>(node), target_factory, index]() {
                                     addChildCommand(parent, target_factory, id, index + 1);
                                   });
                });
}
} // namespace

namespace yoyo::gui
{
contextmenu_handler::contextmenu_handler(std::vector<std::shared_ptr<node_factory>> factories,
                                         QWidget* parent)
  : _factories { std::move(factories) }
{
  _menu = std::make_unique<QMenu>(parent);
}

contextmenu_handler::~contextmenu_handler() = default;

auto contextmenu_handler::update() -> void {}

auto contextmenu_handler::execute(std::shared_ptr<node_base> current, QPoint p) -> void
{
  if (!current) {
    return;
  }

  _menu->clear();

  if (current->acceptsChildren()) {
    auto addMenu = _menu->addMenu("Insert Child");
    auto meta = current->metaObject();

    if (meta->className() == configuration_name) {
      ::addActionsToAdd(addMenu, current, _factories[0]);
    } else if (isData(current)) {
      ::addActionsToAdd(addMenu, current, _factories[1]);
    } else if (isGui(current)) {
      ::addActionsToAdd(addMenu, current, _factories[2]);
    }
  }

  {
    if (auto parent = current->parent().lock()) {
      auto beforeMenu = _menu->addMenu("Insert Sibling before");
      auto afterMenu = _menu->addMenu("Insert Sibling after");
      auto meta = parent->metaObject();

      if (meta->className() == configuration_name) {
        ::addActionsToSibling(beforeMenu, afterMenu, parent, parent->childIndex(current),
                              _factories[0], _factories[0]);
      } else if (meta->className() == data_root_name) {
        ::addActionsToSibling(beforeMenu, afterMenu, parent, parent->childIndex(current),
                              _factories[0], _factories[1]);
      } else if (meta->className() == gui_root_name) {
        ::addActionsToSibling(beforeMenu, afterMenu, parent, parent->childIndex(current),
                              _factories[0], _factories[2]);
      } else if (meta->inherits(&data_node::staticMetaObject)) {
        ::addActionsToSibling(beforeMenu, afterMenu, parent, parent->childIndex(current),
                              _factories[1], _factories[1]);
      } else if (meta->inherits(&gui_node::staticMetaObject)) {
        ::addActionsToSibling(beforeMenu, afterMenu, parent, parent->childIndex(current),
                              _factories[2], _factories[2]);
      }
    }
  }

  if (canBeRemoved(current)) {
    auto remove = new QAction("Remove");
    _menu->addAction(remove);
    connect(remove, &QAction::triggered, this, [current]() {
      if (auto parent = current->parent().lock()) {
        removeChildCommand(parent, current);
      }
    });
  }

  if (hasChildrenToRemove(current)) {
    auto remove = new QAction("Remove all children");
    _menu->addAction(remove);
    connect(remove, &QAction::triggered, this, [current]() { removeChildrenCommand(current); });
  }

  _menu->exec(p);
}

auto contextmenu_handler::defaultForDataRequested(QString path, std::shared_ptr<node_base> parent,
                                                  int index, bool connectDefaultInput,
                                                  bool connectDefaultOutput,
                                                  bool connectDefaultAutoConnect) -> void
{
  if (!parent || !parent->acceptsChildren()) {
    return;
  }

  if (!isGui(parent)) {
    return;
  }

  auto data = std::dynamic_pointer_cast<data_node>(
    utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(path, get_root(parent)));

  if (!data) {
    return;
  }

  static std::map<types::value_t, boost::uuids::uuid> default_guis {
    { types::value_t::BIT,
      { 0x65, 0x38, 0xce, 0xf0, 0x65, 0x92, 0x47, 0x0f, 0x91, 0xf3, 0x80, 0xd4, 0x42, 0x3b, 0x46,
        0xce } },
    { types::value_t::UINT8,
      { 0x28, 0x6a, 0x8c, 0xfb, 0x46, 0xb2, 0x48, 0x7d, 0xae, 0x8a, 0x6f, 0x61, 0x46, 0x48, 0xa4,
        0x94 } },
    { types::value_t::UINT16,
      { 0x28, 0x6a, 0x8c, 0xfb, 0x46, 0xb2, 0x48, 0x7d, 0xae, 0x8a, 0x6f, 0x61, 0x46, 0x48, 0xa4,
        0x94 } },
    { types::value_t::UINT32,
      { 0x28, 0x6a, 0x8c, 0xfb, 0x46, 0xb2, 0x48, 0x7d, 0xae, 0x8a, 0x6f, 0x61, 0x46, 0x48, 0xa4,
        0x94 } },
    { types::value_t::UINT64,
      { 0x28, 0x6a, 0x8c, 0xfb, 0x46, 0xb2, 0x48, 0x7d, 0xae, 0x8a, 0x6f, 0x61, 0x46, 0x48, 0xa4,
        0x94 } },
  };
  auto node_id = boost::uuids::uuid { 0x81, 0xc2, 0xdf, 0x83, 0x24, 0x52, 0x42, 0x2c,
                                      0x88, 0x46, 0xd8, 0xbf, 0xa2, 0xd7, 0x56, 0xdf };
  auto it = default_guis.find(data->valueType());

  if (it != default_guis.end()) {
    node_id = it->second;
  }

  auto new_node = _factories[2]->createNode(node_id, {});

  auto c = new_node->property("connection");

  if (!c.isValid()) {
    return;
  }

  if (c.typeName() == "yoyo::properties::connection_t"s) {
    auto connection = c.value<yoyo::properties::connection_t>();

    if (connectDefaultAutoConnect) {
      connection._auto = path;
    }

    if (connectDefaultInput) {
      connection._in = path;
    }

    if (connectDefaultOutput) {
      connection._out = path;
    }

    new_node->setProperty("connection", QVariant::fromValue(connection));
  }

  if (c.typeName() == "yoyo::properties::in_connection_t"s) {
    auto connection = c.value<yoyo::properties::in_connection_t>();

    if (connectDefaultInput) {
      connection._in = path;
    }

    new_node->setProperty("connection", QVariant::fromValue(connection));
  }

  if (c.typeName() == "yoyo::properties::out_connection_t"s) {
    auto connection = c.value<yoyo::properties::out_connection_t>();

    if (connectDefaultOutput) {
      connection._out = path;
    }

    new_node->setProperty("connection", QVariant::fromValue(connection));
  }

  if (auto handler = yoyo::command::commandhandler()) {
    handler->execute({
      tr("Add new item to %1 for signal").arg(parent->type()),
      [parent, new_node, index]() {
        if (parent && new_node) {
          parent->addChild(new_node, index);
        }
      },
      [parent, new_node]() {
        if (parent && new_node) {
          parent->removeChild(new_node);
        }
      },
      [parent, new_node, index]() {
        if (parent && new_node) {
          parent->addChild(new_node, index);
        }
      },
    });
  } else {
    parent->addChild(new_node, index);
  }
}

auto contextmenu_handler::exchangeRequested(std::shared_ptr<node_base> nodeToExchange,
                                            boost::uuids::uuid nodeType) -> void
{
  if (!nodeToExchange || (nodeToExchange->staticTypeId() == nodeType)) {
    return;
  }

  auto parent = nodeToExchange->parent().lock();

  if (!parent) {
    return;
  }

  // exchange is only allowed for gui nodes
  auto meta = nodeToExchange->metaObject();

  if (!meta->inherits(&gui_node::staticMetaObject)) {
    return;
  }

  auto new_node = _factories[2]->createNode(nodeType, {});

  if (!new_node || !parent->acceptsChild(new_node) || !new_node->acceptsParent(parent)
      || (!new_node->acceptsChildren() && (nodeToExchange->childCount() > 0))) {
    return;
  }

  for (int i = 0; i < meta->propertyCount(); i++) {
    new_node->setProperty(meta->property(i).name(),
                          nodeToExchange->property(meta->property(i).name()));
  }

  auto do_action = [parent, nodeToExchange, new_node]() {
    std::for_each(nodeToExchange->begin(), nodeToExchange->end(),
                  [new_node](auto c) { new_node->addChild(c); });
    parent->addChild(new_node, parent->childIndex(nodeToExchange));
    parent->removeChild(nodeToExchange);
  };
  auto undo_action = [parent, nodeToExchange, new_node]() {
    std::for_each(new_node->begin(), new_node->end(),
                  [nodeToExchange](auto c) { nodeToExchange->addChild(c); });
    parent->addChild(nodeToExchange, parent->childIndex(new_node));
    parent->removeChild(new_node);
  };

  if (auto handler = yoyo::command::commandhandler()) {
    handler->execute({
      tr("Change item type from %1 to %2").arg(nodeToExchange->type()).arg(new_node->type()),
      do_action,
      undo_action,
      do_action,
    });
  } else {
    do_action();
  }
}

auto contextmenu_handler::addRequested(std::shared_ptr<node_base> parentNode,
                                       boost::uuids::uuid nodeType, int index) -> void
{
  if (!parentNode) {
    return;
  }

  if (!parentNode->acceptsChildren()) {
    return;
  }

  // add is only allowed for gui nodes
  if (!isGui(parentNode)) {
    return;
  }

  addChildCommand(parentNode, _factories[2], nodeType, index);
}
} // namespace yoyo::gui
