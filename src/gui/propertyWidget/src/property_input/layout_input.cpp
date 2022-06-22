#include "layout_input.hpp"

#include "yoyo/node_base.h"

#include <QComboBox>
#include <QLayout>
#include <QMetaObject>
#include <QMetaProperty>
#include <QTimer>

#include <string>

using std::string_literals::operator""s;

namespace yoyo::gui
{
layout_input::layout_input(std::shared_ptr<node_base> node, std::string propertyName,
                           QWidget* parent)
  : property_input { parent }
  , _input { std::make_unique<QComboBox>(this) }
{
  _input->addItems(QStringList {} << tr("Floating") << tr("Horizontal") << tr("Vertical"));
  layout()->addWidget(_input.get());

  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));

  int method_index = staticMetaObject.methodOffset();
  std::function<void()> slotFunction;

  if (prop.typeName() == "yoyo::types::layout_direction_t"s) {
    slotFunction = [this, propertyName]() {
      auto value = static_cast<types::layout_direction_t>(_input->currentIndex());

      if (_buffer != value) {
        Q_EMIT propertyChanged(propertyName, QVariant::fromValue(value));
      }
    };
    onPropertyChanged(node->property(propertyName.c_str()).value<types::layout_direction_t>());
  } else if (prop.typeName() == "yoyo::properties::invisible_layout_direction_t"s) {
    method_index++;
    slotFunction = [this, propertyName]() {
      auto value = static_cast<types::layout_direction_t>(_input->currentIndex());

      if (_invisible_buffer._s != value) {
        _invisible_buffer._s = value;
        Q_EMIT propertyChanged(propertyName, QVariant::fromValue(_invisible_buffer));
      }
    };

    QTimer::singleShot(1, this, [this, node, propertyName]() {
      onPropertyChanged(
        node->property(propertyName.c_str()).value<properties::invisible_layout_direction_t>());
      Q_EMIT visibilityChanged(_invisible_buffer._visible);
    });
  }

  connect(node.get(), prop.notifySignal(), this, staticMetaObject.method(method_index));
  connect(_input.get(), static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, slotFunction);
}

layout_input::~layout_input() = default;

auto layout_input::onPropertyChanged(types::layout_direction_t value) -> void
{
  _buffer = value;
  _input->setCurrentIndex(static_cast<int>(value));
}

auto layout_input::onPropertyChanged(properties::invisible_layout_direction_t value) -> void
{
  if (_invisible_buffer._visible != value._visible) {
    Q_EMIT visibilityChanged(value._visible);
  }

  _invisible_buffer = value;
  _input->setCurrentIndex(static_cast<int>(value._s));

  setVisible(_invisible_buffer._visible);

  if (_invisible_buffer._visible) {
    show();
  } else {
    hide();
  }
}
} // namespace yoyo::gui
