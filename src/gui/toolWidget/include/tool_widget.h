#pragma once
#include "toolwidget_export.h"

#include <QDockWidget>

#include <memory>

namespace Ui
{
class ToolWidget;
}

namespace yoyo
{
class node_factory;
namespace gui
{
class tool_model;
class YOYO_GUI_TOOL_SHARED_EXPORT tool_widget : public QDockWidget
{
  Q_OBJECT

public:
  tool_widget(std::shared_ptr<yoyo::node_factory> gui_factory, QWidget* parent = nullptr);
  ~tool_widget() override;

  auto update() -> void;

private:
  std::unique_ptr<Ui::ToolWidget> ui;
  std::unique_ptr<tool_model> _model;
};
} // namespace gui
} // namespace yoyo
