#include "yoyo_main_window.h"
#include "ui_yoyo_main_window.h"

#include <QSettings>

#include <string>

using std::string_literals::operator""s;

namespace yoyo
{
yoyo_main_window::yoyo_main_window(QWidget* parent)
  : QMainWindow { parent }
  , _ui { std::make_unique<Ui::yoyo_main_window>() }
{
  _ui->setupUi(this);
  if (!_ui->top_widget->layout()) {
    _ui->top_widget->setLayout(new QVBoxLayout(_ui->top_widget));
  }
  _ui->actionRecent_files->setMenu(new QMenu);

  connect(_ui->actionNew_Configuration, &QAction::triggered, this,
          &yoyo_main_window::newFile_requested);
  connect(_ui->actionLoad_Configuration, &QAction::triggered, this,
          &yoyo_main_window::openFile_requested);
  connect(_ui->actionSave_Configuration, &QAction::triggered, this,
          &yoyo_main_window::saveFile_requested);
  connect(_ui->actionSave_as_Configuration, &QAction::triggered, this,
          &yoyo_main_window::saveFileAs_requested);
  connect(_ui->actionClose_Configuration, &QAction::triggered, this,
          &yoyo_main_window::closeFile_requested);
  connect(_ui->actionExit, &QAction::triggered, this, &yoyo::yoyo_main_window::exit_requested);

  connect(_ui->actionEdit_mode, &QAction::toggled, this, [this](auto enabled) {
    Q_EMIT tree_requested(enabled && _ui->actionBrowser_Box->isChecked());
    Q_EMIT tools_requested(enabled && _ui->actionTool_Box->isChecked());
    Q_EMIT properties_requested(enabled && _ui->actionProperty_Box->isChecked());
    Q_EMIT editMode(enabled);
  });

  connect(_ui->actionBrowser_Box, &QAction::toggled, this, &yoyo_main_window::tree_requested);
  connect(_ui->actionLog_Box, &QAction::toggled, this, &yoyo_main_window::log_requested);
  connect(_ui->actionTool_Box, &QAction::toggled, this, &yoyo_main_window::tools_requested);
  connect(_ui->actionProperty_Box, &QAction::toggled, this,
          &yoyo_main_window::properties_requested);

  connect(_ui->actionAbout, &QAction::triggered, this, []() {

  });
  connect(_ui->actionHelp, &QAction::triggered, this, &yoyo_main_window::help_requested);
}

yoyo_main_window::~yoyo_main_window() = default;

auto yoyo_main_window::set_edit_mode(bool mode) -> void
{
  _ui->actionEdit_mode->setChecked(mode);
}

auto yoyo_main_window::edit_mode() const -> bool
{
  return _ui->actionEdit_mode->isChecked();
}

auto yoyo_main_window::set_central_widget(QWidget* widget) -> void
{
  if (_ui->top_widget->children().count() > 1) {
    auto old_widget = dynamic_cast<QWidget*>(_ui->top_widget->children().at(1));
    if (old_widget->metaObject()->className() == "QWidget"s) {
      delete old_widget;
    } else {
      old_widget->setParent(nullptr);
    }
  }
  if (widget) {
    _ui->top_widget->layout()->addWidget(widget);
  } else {
    _ui->top_widget->layout()->addWidget(new QWidget(_ui->top_widget));
  }
  _ui->scrollArea->show();
}

auto yoyo_main_window::history_changed(std::vector<QString> history) -> void
{
  auto menu = _ui->actionRecent_files->menu();
  menu->clear();
  for (auto const& file : history) {
    menu->addAction(file, this, [file, this]() { Q_EMIT this->recentFile_requested(file); });
  }
}
} // namespace yoyo
