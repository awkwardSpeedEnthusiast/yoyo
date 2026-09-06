#include "plugin_description_dialog.hpp"
#include "ui_plugin_description_dialog.h"

#include "plugin_data.h"

#include <boost/uuid/uuid_io.hpp>

namespace yoyo::gui
{
plugin_description_dialog::plugin_description_dialog(std::shared_ptr<plugin::plugin_data> data,
                                                     QWidget* parent)
  : QDialog { parent }
  , ui { std::make_unique<Ui::plugin_description_dialog>() }
{
  ui->setupUi(this);
  ui->name->setText(data->name());
  ui->id->setText(QString::fromStdString(boost::uuids::to_string(data->id())));
  ui->description->setPlainText(data->description());
  ui->description_path->setText(QString::fromStdString(data->description_location().string()));
  ui->plugin_path->setText(QString::fromStdString(data->location().string()));
}

plugin_description_dialog::~plugin_description_dialog() = default;
} // namespace yoyo::gui
