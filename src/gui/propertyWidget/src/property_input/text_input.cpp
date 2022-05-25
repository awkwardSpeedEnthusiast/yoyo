#include "text_input.hpp"
#include "ui_text_input.h"

#include "yoyo/node_base.h"

#include <QMetaObject>
#include <QMetaProperty>

namespace yoyo::gui
{
text_input::text_input(std::shared_ptr<node_base> node, std::string propertyName, QWidget* parent)
  : property_input { parent, false }
  , _ui { std::make_unique<Ui::text_input>() }
{
  _ui->setupUi(this);
  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));
  connect(node.get(), prop.notifySignal(), this,
          staticMetaObject.method(staticMetaObject.methodOffset()));
  connect(_ui->save_button, &QPushButton::clicked, this, [this, propertyName]() {
    _buffer._s = _ui->input->toPlainText();
    Q_EMIT propertyChanged(propertyName, QVariant::fromValue(_buffer));
  });

  onPropertyChanged(node->property(propertyName.c_str()).value<properties::text_t>());
}

text_input::~text_input() = default;

auto text_input::onPropertyChanged(properties::text_t value) -> void
{
  _buffer = value;
  _ui->input->setText(_buffer._s);
}
} // namespace yoyo::gui
