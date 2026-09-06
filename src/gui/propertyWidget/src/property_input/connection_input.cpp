#include "connection_input.hpp"
#include "ui_connection_input.h"

#include "yoyo/node_base.h"

#include <QLayout>
#include <QLineEdit>
#include <QMetaObject>
#include <QMetaProperty>

namespace yoyo::gui
{
connection_input::connection_input(std::shared_ptr<node_base> node, std::string propertyName,
                                   QWidget* parent)
  : property_input { parent, false }
  , _ui { std::make_unique<Ui::connection_input>() }
{
  _ui->setupUi(this);
  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));
  connect(node.get(), prop.notifySignal(), this,
          staticMetaObject.method(staticMetaObject.methodOffset()));
  connect(_ui->in_input, &QLineEdit::returnPressed, this, [this, propertyName]() {
    _buffer._in = _ui->in_input->text();
    Q_EMIT propertyChanged(propertyName, QVariant::fromValue(_buffer));
  });
  connect(_ui->out_input, &QLineEdit::returnPressed, this, [this, propertyName]() {
    _buffer._out = _ui->out_input->text();
    Q_EMIT propertyChanged(propertyName, QVariant::fromValue(_buffer));
  });
  connect(_ui->auto_input, &QLineEdit::returnPressed, this, [this, propertyName]() {
    _buffer._auto = _ui->auto_input->text();
    Q_EMIT propertyChanged(propertyName, QVariant::fromValue(_buffer));
  });

  onPropertyChanged(node->property(propertyName.c_str()).value<properties::connection_t>());
}

connection_input::~connection_input() = default;

auto connection_input::onPropertyChanged(properties::connection_t value) -> void
{
  _buffer = value;
  _ui->in_input->setText(_buffer._in);
  _ui->out_input->setText(_buffer._out);
  _ui->auto_input->setText(_buffer._auto);
}
} // namespace yoyo::gui
