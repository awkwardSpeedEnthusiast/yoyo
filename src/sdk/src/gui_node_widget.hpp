#pragma once
#include "yoyo/property_types.h"

#include <QWidget>
#include <Qt>

class QMouseEvent;
class QMimeData;

namespace yoyo
{
class gui_node;
class node_base;

class gui_node_widget : public QWidget
{
  Q_OBJECT
public:
  gui_node_widget(std::weak_ptr<gui_node> parent, QWidget* container_widget);
  ~gui_node_widget() override;

  auto containerWidget() -> QWidget*;

  auto addWidget(QWidget* widget, int index) -> void;
  auto removeWidget(QWidget* widget) -> void;

  auto setLayoutDirection(types::layout_direction_t direction) -> void;

  auto setEditMode(bool editModeOn) -> void;

private:
  auto eventFilter(QObject* obj, QEvent* event) -> bool override;
  auto dragEnterEvent(QDragEnterEvent* event) -> void override;
  auto dragMoveEvent(QDragMoveEvent* event) -> void override;
  auto dropEvent(QDropEvent* event) -> void override;
  auto mousePressEvent(QMouseEvent* event) -> void override;
  auto mouseMoveEvent(QMouseEvent* event) -> void override;

  auto checkDragEvent_new(QMimeData const* data, Qt::MouseButtons buttons) -> bool;
  auto checkDragEvent_data(QMimeData const* data, Qt::MouseButtons buttons) -> bool;
  auto checkDragEvent_other(QMimeData const* data, Qt::MouseButtons buttons, node_base* source)
    -> bool;

  auto contextMenuEvent(QContextMenuEvent* event) -> void override;

  std::weak_ptr<gui_node> _parent;

  bool _active;

  std::unique_ptr<QWidget> _container { nullptr };
  std::vector<QWidget*> _children;

  QPoint _startDrag;
};
} // namespace yoyo
