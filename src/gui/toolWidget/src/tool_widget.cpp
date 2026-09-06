#include "tool_widget.h"
#include "tool_model.hpp"
#include "ui_toolwidget.h"

#include "yoyo/node_factory.h"

namespace yoyo::gui
{
tool_widget::tool_widget(std::shared_ptr<node_factory> gui_factory, QWidget* parent)
  : QDockWidget(parent)
  , ui { std::make_unique<Ui::ToolWidget>() }
  , _model { std::make_unique<tool_model>(gui_factory) }
{
  ui->setupUi(this);
  ui->listView->setModel(_model.get());
}

auto tool_widget::update() -> void
{
  _model->update();
}

tool_widget::~tool_widget() = default;
} // namespace yoyo::gui
