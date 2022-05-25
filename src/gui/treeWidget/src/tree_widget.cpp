#include "tree_widget.h"
#include "ui_treewidget.h"

#include "contextmenu_handler.hpp"
#include "tree_model.hpp"

#include "yoyo/gui_node.h"

#include <QItemSelectionModel>

namespace yoyo::gui
{
tree_widget::tree_widget(std::vector<std::shared_ptr<node_factory>> factories, QWidget* parent)
  : QDockWidget { parent }
  , ui { std::make_unique<Ui::TreeWidget>() }
  , _model { std::make_unique<tree_model>() }
  , _menu { std::make_unique<contextmenu_handler>(factories, this) }
{
  ui->setupUi(this);
  ui->treeView->setModel(_model.get());

  connect(ui->treeView, &QWidget::customContextMenuRequested, this, [this](auto point) {
    _menu->execute(_model->nodeForIndex(ui->treeView->indexAt(point)),
                   ui->treeView->viewport()->mapToGlobal(point));
  });
  connect(this, &QWidget::customContextMenuRequested, this, [this](auto point) {
    auto p = ui->treeView->viewport()->mapFrom(this, point);
    _menu->execute(_model->nodeForIndex(ui->treeView->indexAt(p)), mapToGlobal(point));
  });
  connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this,
          [this](QItemSelection selected, auto) {
            if (selected.isEmpty() || (selected.count() > 1)) {
              Q_EMIT itemSelected({});
              return;
            }

            Q_EMIT itemSelected(_model->nodeForIndex(selected.indexes().at(0)));
          });
}

auto tree_widget::setConfiguration(std::shared_ptr<node_base> root_node) -> void
{
  if (auto root = _root.lock()) {
    disconnect(root.get(), &node_base::addRequested, this, nullptr);
    disconnect(root.get(), &node_base::exchangeRequested, this, nullptr);
    disconnect(std::dynamic_pointer_cast<gui_node>(root->childAt(1)).get(), &gui_node::itemSelected,
               this, nullptr);
    disconnect(std::dynamic_pointer_cast<gui_node>(root->childAt(1)).get(),
               &gui_node::defaultForDataRequested, this, nullptr);
  }

  _model->setRootNode(root_node);
  _root = root_node;

  if (root_node) {
    connect(root_node.get(), &node_base::addRequested, this,
            [this](auto p, auto id, auto index) { _menu->addRequested(p, id, index); });
    connect(root_node.get(), &node_base::exchangeRequested, this,
            [this](auto p, auto id) { _menu->exchangeRequested(p, id); });

    if (root_node->childCount() >= 2) {
      if (auto gui = std::dynamic_pointer_cast<gui_node>(root_node->childAt(1))) {
        connect(gui.get(), &gui_node::itemSelected, this, [this](auto i) {
          ui->treeView->selectionModel()->clearSelection();
          ui->treeView->selectionModel()->select(
            _model->indexForNode(i), QItemSelectionModel::Select | QItemSelectionModel::Rows);
        });
        connect(gui.get(), &gui_node::defaultForDataRequested, this,
                [this](auto path, auto p, auto index, auto ci, auto co, auto ca) {
                  _menu->defaultForDataRequested(path, p, index, ci, co, ca);
                });
      }
    }
  }
}

auto tree_widget::updateFactories() -> void
{
  _menu->update();
}

tree_widget::~tree_widget() = default;
} // namespace yoyo::gui
