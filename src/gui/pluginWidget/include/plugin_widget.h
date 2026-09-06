#pragma once

#include <QWidget>

#include <memory>

namespace Ui
{
class plugin_widget;
}

namespace yoyo
{
namespace plugin
{
class plugin_manager;
}

namespace gui
{
class plugin_model;

class plugin_widget : public QWidget
{
  Q_OBJECT

public:
  explicit plugin_widget(std::shared_ptr<plugin::plugin_manager> manager,
                         QWidget* parent = nullptr);
  ~plugin_widget() override;

private:
  auto context_menu_requested(QPoint const&) -> void;

private:
  struct impl;
  std::unique_ptr<impl> _p;
};

} // namespace gui
} // namespace yoyo
