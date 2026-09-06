#include "yoyo_main_window.h"
#include "ui_yoyo_main_window.h"

#include "command_handler.h"

#include <QSettings>
#include <QLabel>

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
  _l = new QLabel;
  dynamic_cast<QLabel*>(_l)->setText("Edit mode enabled");

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
    using namespace std::chrono_literals;
    if (enabled) {
      statusBar()->addWidget(_l);
      _l->show();
      set_status_message("Edit mode enabled", 2000ms);
    } else {
      statusBar()->removeWidget(_l);
      set_status_message("Edit mode disabled", 2000ms);
    }
  });

  auto prototype_label = new QLabel;
  prototype_label->setText("Prototype");
  prototype_label->setStyleSheet("color: white; background-color: red; font-size: 20px");
  statusBar()->addPermanentWidget(prototype_label);

  connect(_ui->actionBrowser_Box, &QAction::toggled, this, &yoyo_main_window::tree_requested);
  connect(_ui->actionLog_Box, &QAction::toggled, this, &yoyo_main_window::log_requested);
  connect(_ui->actionTool_Box, &QAction::toggled, this, &yoyo_main_window::tools_requested);
  connect(_ui->actionProperty_Box, &QAction::toggled, this,
          &yoyo_main_window::properties_requested);
  connect(_ui->actionPlugins, &QAction::triggered, this, &yoyo_main_window::plugins_requested);

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

auto yoyo_main_window::set_file_open(bool is_open) -> void
{
  _ui->actionPlugins->setEnabled(!is_open);
}

auto yoyo_main_window::set_status_message(QString const& message, std::chrono::milliseconds timeout) -> void
{
  statusBar()->showMessage(message, timeout.count());
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

auto yoyo_main_window::store_state(QSettings& settings) -> void
{
  settings.setValue("main_window/geometry", saveGeometry());
  settings.setValue("main_window/windowState", saveState());
  settings.setValue("main_window/editMode", _ui->actionEdit_mode->isChecked());
  settings.setValue("main_window/treeWidgetVisible", _ui->actionBrowser_Box->isChecked());
  settings.setValue("main_window/toolWidgetVisible", _ui->actionTool_Box->isChecked());
  settings.setValue("main_window/propertyWidgetVisible", _ui->actionProperty_Box->isChecked());
  settings.setValue("main_window/logWidgetVisible", _ui->actionLog_Box->isChecked());
  settings.setValue("main_window/helpWidgetVisible", _ui->actionHelp->isChecked());
}

auto yoyo_main_window::restore_state(QSettings& settings) -> void
{
  restoreGeometry(settings.value("main_window/geometry").toByteArray());
  restoreState(settings.value("main_window/windowState").toByteArray());
  _ui->actionEdit_mode->setChecked(settings.value("main_window/editMode", false).toBool());
  _ui->actionBrowser_Box->setChecked(
    settings.value("main_window/treeWidgetVisible", false).toBool());
  _ui->actionTool_Box->setChecked(settings.value("main_window/toolWidgetVisible", false).toBool());
  _ui->actionProperty_Box->setChecked(
    settings.value("main_window/propertyWidgetVisible", false).toBool());
  _ui->actionLog_Box->setChecked(settings.value("main_window/logWidgetVisible", false).toBool());
  _ui->actionHelp->setChecked(settings.value("main_window/helpWidgetVisible", false).toBool());
}

auto yoyo_main_window::setup_undo() -> void
{
  if (auto handler = command::commandhandler()) {
    connect(handler, &command::command_handler::topRedoItemChanged, this,
            [this](bool isAvailable, QString name) {
              _ui->actionRedo->setEnabled(isAvailable);
              _ui->actionRedo->setText(tr("Redo %1").arg(name));
            });
    connect(handler, &command::command_handler::topUndoItemChanged, this,
            [this](bool isAvailable, QString name) {
              _ui->actionUndo->setEnabled(isAvailable);
              _ui->actionUndo->setText(tr("Undo %1").arg(name));
            });
    connect(_ui->actionUndo, &QAction::triggered, handler, &command::command_handler::undo);
    connect(_ui->actionRedo, &QAction::triggered, handler, &command::command_handler::redo);
  } else {
    _ui->actionUndo->setVisible(false);
    _ui->actionRedo->setVisible(false);
  }
}

} // namespace yoyo
