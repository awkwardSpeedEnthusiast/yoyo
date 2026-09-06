#pragma once

#include <QDialog>

#include <memory>

namespace Ui
{
class plugin_description_dialog;
}

namespace yoyo
{
namespace plugin
{
class plugin_data;
}
namespace gui
{
class plugin_description_dialog : public QDialog
{
  Q_OBJECT

public:
  explicit plugin_description_dialog(std::shared_ptr<plugin::plugin_data> data,
                                     QWidget* parent = nullptr);
  ~plugin_description_dialog() override;

private:
  std::unique_ptr<Ui::plugin_description_dialog> ui;
};

} // namespace gui
} // namespace yoyo
