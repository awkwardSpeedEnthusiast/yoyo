#include "invisible_string_input.hpp"

#include "yoyo/node_base.h"

#include <QLayout>
#include <QLineEdit>
#include <QMetaObject>
#include <QMetaProperty>

namespace yoyo::gui
{
invisible_string_input::invisible_string_input(std::shared_ptr<node_base> node,
                                               std::string propertyName, QWidget* parent)
  : property_input { parent }
  , _input { std::make_unique<QLineEdit>(this) }
{
  layout()->addWidget(_input.get());

  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));
  connect(node.get(), prop.notifySignal(), this,
          staticMetaObject.method(staticMetaObject.methodOffset()));
  connect(_input.get(), &QLineEdit::returnPressed, this, [this, propertyName]() {
    _buffer._s = _input->text();
    Q_EMIT propertyChanged(propertyName, QVariant::fromValue(_buffer));
  });

  onPropertyChanged(node->property(propertyName.c_str()).value<properties::invisible_string_t>());
}

invisible_string_input::~invisible_string_input() = default;

auto invisible_string_input::onPropertyChanged(properties::invisible_string_t value) -> void
{
  if (_buffer._visible != value._visible) {
    Q_EMIT visibilityChanged(value._visible);
  }

  _buffer = value;
  _input->setText(_buffer._s);
  setVisible(_buffer._visible);

  if (_buffer._visible) {
    show();
  } else {
    hide();
  }
}
} // namespace yoyo::gui
