#include "connected_boolean_input.hpp"

#include "yoyo/node_base.h"

#include <QComboBox>
#include <QLayout>
#include <QMetaObject>
#include <QMetaProperty>

#include <string>

using std::string_literals::operator""s;

namespace yoyo::gui
{
connected_boolean_input::connected_boolean_input(std::shared_ptr<node_base> node,
                                                 std::string propertyName, QWidget* parent)
  : property_input { parent }
  , _input { std::make_unique<QComboBox>(this) }
{
  _input->addItems(QStringList {} << tr("True") << tr("False") << tr("Defined by connection"));
  layout()->addWidget(_input.get());

  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));

  int method_index = staticMetaObject.methodOffset();
  std::function<void()> slotFunction;

  slotFunction = [this, propertyName]() {
    auto value = static_cast<properties::connected_boolean_t::boolean_t>(_input->currentIndex());

    if (_buffer._type != value) {
      _buffer._type = value;
      Q_EMIT propertyChanged(propertyName, QVariant::fromValue(_buffer));
    }
  };

  connect(node.get(), prop.notifySignal(), this, staticMetaObject.method(method_index));
  connect(_input.get(), static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, slotFunction);
  onPropertyChanged(node->property(propertyName.c_str()).value<properties::connected_boolean_t>());
}

connected_boolean_input::~connected_boolean_input() = default;

auto connected_boolean_input::onPropertyChanged(properties::connected_boolean_t value) -> void
{
  _buffer = value;
  _input->setCurrentIndex(static_cast<int>(value._type));
}
} // namespace yoyo::gui
