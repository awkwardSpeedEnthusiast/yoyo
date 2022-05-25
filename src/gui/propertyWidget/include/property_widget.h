#pragma once
#include "propertywidget_export.h"

#include <QDockWidget>

#include <memory>
namespace Ui
{
class property_widget;
}

namespace yoyo
{
class node_base;
class documentation;

namespace gui
{
class YOYO_GUI_PROPERTY_SHARED_EXPORT property_widget : public QDockWidget
{
  Q_OBJECT

public:
  explicit property_widget(QWidget* parent = nullptr);
  ~property_widget() override;

  auto itemSelected(std::shared_ptr<node_base> item, std::shared_ptr<documentation> doc) -> void;

private:
  std::unique_ptr<Ui::property_widget> ui;

  class impl;
  std::unique_ptr<impl> _p;
};
} // namespace gui
} // namespace yoyo
