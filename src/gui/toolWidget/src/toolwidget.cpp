#include "toolwidget.h"
#include "toolModel.hpp"
#include "ui_toolwidget.h"

#include "yoyo/node_factory.h"

namespace yoyo::gui
{
ToolWidget::ToolWidget(std::shared_ptr<node_factory> gui_factory, QWidget* parent)
  : QDockWidget(parent)
  , ui { std::make_unique<Ui::ToolWidget>() }
  , _model { std::make_unique<toolModel>(gui_factory) }
{
  ui->setupUi(this);
  ui->listView->setModel(_model.get());
}

auto ToolWidget::update() -> void
{
  _model->update();
}

ToolWidget::~ToolWidget() = default;
} // namespace yoyo::gui
