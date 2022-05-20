#pragma once
#include "treewidget_export.h"

#include <QDockWidget>

#include <memory>
namespace Ui
{
class TreeWidget;
}
namespace yoyo
{
class node_base;
class node_factory;
namespace gui
{
class tree_model;
class contextmenu_handler;

class YOYO_GUI_TREE_SHARED_EXPORT TreeWidget : public QDockWidget
{
  Q_OBJECT

public:
  explicit TreeWidget(std::vector<std::shared_ptr<node_factory>> factories,
                      QWidget* parent = nullptr);
  ~TreeWidget() override;

  auto setConfiguration(std::shared_ptr<node_base> root_node) -> void;
  auto updateFactories() -> void;

Q_SIGNALS:
  void itemSelected(std::shared_ptr<node_base> selectedItem);

private:
  std::unique_ptr<Ui::TreeWidget> ui;
  std::unique_ptr<tree_model> _model;
  std::unique_ptr<contextmenu_handler> _menu;
  std::weak_ptr<node_base> _root;
};
} // namespace gui
} // namespace yoyo
