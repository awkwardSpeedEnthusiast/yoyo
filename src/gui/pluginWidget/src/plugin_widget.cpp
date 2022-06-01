#include "plugin_widget.h"
#include "plugin_description_dialog.hpp"
#include "plugin_model.hpp"
#include "ui_plugin_widget.h"

#include "plugin_manager.h"

#include <QFileDialog>
#include <QMenu>

#include <boost/uuid/string_generator.hpp>

namespace yoyo::gui
{
struct plugin_widget::impl {
  impl(std::shared_ptr<plugin::plugin_manager> manager, QWidget* parent)
    : _ui { std::make_unique<Ui::plugin_widget>() }
    , _model { std::make_unique<plugin_model>(manager) }
    , _manager { manager }
    , _menu { std::make_unique<QMenu>(parent) }
  {
  }

  std::unique_ptr<Ui::plugin_widget> _ui;
  std::unique_ptr<plugin_model> _model;
  std::shared_ptr<plugin::plugin_manager> _manager;
  std::unique_ptr<QMenu> _menu;
};

plugin_widget::plugin_widget(std::shared_ptr<plugin::plugin_manager> manager, QWidget* parent)
  : QWidget(parent)
  , _p { std::make_unique<impl>(manager, this) }
{
  _p->_ui->setupUi(this);
  _p->_menu->addAction(_p->_ui->load_plugin_action);
  _p->_menu->addAction(_p->_ui->unload_plugin_action);
  _p->_menu->addAction(_p->_ui->remove_plugin_action);
  _p->_ui->listView->setModel(_p->_model.get());

  connect(_p->_ui->load_button, &QPushButton::clicked, this, [this]() {
    auto path = QFileDialog::getOpenFileName(this, tr("Open plugin description"), {},
                                             tr("Plugin descriptions (*.json)"));
    if (path.isEmpty()) {
      return;
    }

    auto description = _p->_manager->read_plugin_description(path.toStdString());

    plugin_description_dialog dialog(description, this);
    dialog.exec();
    if (dialog.result() == QDialog::Accepted) {
      _p->_model->add_plugin(description);
    }
  });
  connect(_p->_ui->listView, &QListView::customContextMenuRequested, this,
          &plugin_widget::context_menu_requested);
}

plugin_widget::~plugin_widget() = default;

auto plugin_widget::context_menu_requested(QPoint const& p) -> void
{
  auto index = _p->_ui->listView->indexAt(p);
  if (!index.isValid() || index.row() >= _p->_model->rowCount()) {
    return;
  }

  auto id = _p->_model->data(index, Qt::UserRole).toString();

  connect(_p->_ui->load_plugin_action, &QAction::triggered, this,
          [this, index]() { _p->_model->setData(index, true, Qt::CheckStateRole); });
  connect(_p->_ui->unload_plugin_action, &QAction::triggered, this,
          [this, index]() { _p->_model->setData(index, false, Qt::CheckStateRole); });
  connect(_p->_ui->remove_plugin_action, &QAction::triggered, this,
          [this, index]() { _p->_model->remove(index); });

  auto isLoaded =
    _p->_manager->is_plugin_loaded(boost::uuids::string_generator {}(id.toStdString()));
  _p->_ui->load_plugin_action->setEnabled(!isLoaded);
  _p->_ui->unload_plugin_action->setEnabled(isLoaded);
  _p->_ui->load_plugin_action->setVisible(!isLoaded);
  _p->_ui->unload_plugin_action->setVisible(isLoaded);
  auto pos = _p->_ui->listView->mapToGlobal(p);
  _p->_menu->exec(pos);

  disconnect(_p->_ui->load_plugin_action, &QAction::triggered, this, nullptr);
  disconnect(_p->_ui->unload_plugin_action, &QAction::triggered, this, nullptr);
  disconnect(_p->_ui->remove_plugin_action, &QAction::triggered, this, nullptr);
}

} // namespace yoyo::gui
