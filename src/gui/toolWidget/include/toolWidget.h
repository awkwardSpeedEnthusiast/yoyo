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
class toolModel;
class YOYO_GUI_TOOL_SHARED_EXPORT ToolWidget : public QDockWidget
{
  Q_OBJECT

public:
  ToolWidget(std::shared_ptr<yoyo::node_factory> gui_factory, QWidget* parent = nullptr);
  ~ToolWidget() override;

  auto update() -> void;

private:
  std::unique_ptr<Ui::ToolWidget> ui;
  std::unique_ptr<toolModel> _model;
};
} // namespace gui
} // namespace yoyo
