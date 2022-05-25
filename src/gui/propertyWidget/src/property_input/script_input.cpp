#include "script_input.hpp"
#include "ui_script_input.h"

#include "yoyo/node_base.h"

#include <QMetaObject>
#include <QMetaProperty>

namespace yoyo::gui
{
script_input::script_input(std::shared_ptr<node_base> node, std::string property, QWidget* parent)
  : property_input { parent, false }
  , ui { std::make_unique<Ui::script_input>() }
{
  ui->setupUi(this);
  auto prop = node->metaObject()->property(node->metaObject()->indexOfProperty(property.c_str()));
  connect(node.get(), prop.notifySignal(), this,
          staticMetaObject.method(staticMetaObject.methodOffset()));
  ui->script_definition->setVisible(false);
  ui->save->setVisible(false);

  connect(ui->script_type, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, [this, property]() {
            auto value = static_cast<properties::script_t::type>(ui->script_type->currentIndex());

            if (value != _buffer._script_type) {
              Q_EMIT propertyChanged(
                property, QVariant::fromValue(properties::script_t { _buffer._text, value }));
            }
          });
  connect(ui->file_input, &QLineEdit::returnPressed, this, [this, property]() {
    _buffer._text = ui->file_input->text();
    Q_EMIT propertyChanged(property, QVariant::fromValue(_buffer));
  });
  connect(ui->save, &QPushButton::clicked, this, [this, property]() {
    _buffer._text = ui->script_definition->toPlainText();
    Q_EMIT propertyChanged(property, QVariant::fromValue(_buffer));
  });
  setVisible(true);
  show();
  onPropertyChanged(node->property(property.c_str()).value<properties::script_t>());
}

script_input::~script_input() = default;

auto script_input::onPropertyChanged(yoyo::properties::script_t value) -> void
{
  auto vt_old = (_buffer._script_type == properties::script_t::type::FILE
                 || _buffer._script_type == properties::script_t::type::IMAGE);
  auto vt_new = (value._script_type == properties::script_t::type::FILE
                 || value._script_type == properties::script_t::type::IMAGE);
  auto toggleVisibility = vt_old != vt_new;
  _buffer = value;
  ui->script_type->setCurrentIndex(static_cast<int>(_buffer._script_type));

  if (toggleVisibility) {
    ui->file_input->setVisible(!ui->file_input->isVisible());
    ui->script_definition->setVisible(!ui->script_definition->isVisible());
    ui->save->setVisible(!ui->save->isVisible());
  }

  ui->file_input->setText(_buffer._text);
  ui->script_definition->setText(_buffer._text);
  ui->script_type->setCurrentIndex(static_cast<int>(_buffer._script_type));
}
} // namespace yoyo::gui
