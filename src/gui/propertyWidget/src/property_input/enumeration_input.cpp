#include "enumeration_input.hpp"

#include "yoyo/node_base.h"

#include <QComboBox>
#include <QLayout>
#include <QMetaObject>
#include <QMetaProperty>

#include <string>

using std::string_literals::operator""s;

namespace yoyo::gui
{
enumeration_input::enumeration_input(std::shared_ptr<node_base> node, std::string propertyName,
                                     QWidget* parent)
  : property_input { parent }
  , _input { std::make_unique<QComboBox>(this) }
{
  layout()->addWidget(_input.get());

  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));
  if (prop.typeName() == "yoyo::types::access_t"s) {
    setup<yoyo::types::access_t>(node, propertyName);
  } else if (prop.typeName() == "yoyo::properties::transmission_direction_t"s) {
    setup<yoyo::properties::transmission_direction_t>(node, propertyName);
  }
}

template <typename T>
auto enumeration_input::setup(std::shared_ptr<node_base> node, std::string property) -> void
{
  auto prop = node->metaObject()->property(node->metaObject()->indexOfProperty(property.c_str()));

  if constexpr (std::is_same_v<T, types::access_t>) {
    _input->addItems(QStringList {} << tr("None") << tr("Read") << tr("Write") << tr("Read-Write"));

    connect(node.get(), prop.notifySignal(), this,
            staticMetaObject.method(staticMetaObject.methodOffset()));
  } else if constexpr (std::is_same_v<T, properties::transmission_direction_t>) {
    _input->addItems(QStringList {} << tr("Tx") << tr("Rx"));

    connect(node.get(), prop.notifySignal(), this,
            staticMetaObject.method(staticMetaObject.methodOffset() + 1));
  }

  connect(_input.get(), static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, [this, property](auto) {
            auto value = static_cast<T>(_input->currentIndex());
            if (std::get<T>(_buffer) != value) {
              Q_EMIT propertyChanged(property, QVariant::fromValue(value));
            }
          });
  onPropertyChanged(node->property(property.c_str()).value<T>());
}

enumeration_input::~enumeration_input() = default;

auto enumeration_input::onPropertyChanged(yoyo::types::access_t value) -> void
{
  _buffer = value;
  _input->setCurrentIndex(static_cast<int>(value));
}

auto enumeration_input::onPropertyChanged(yoyo::properties::transmission_direction_t value) -> void
{
  _buffer = value;
  _input->setCurrentIndex(static_cast<int>(value));
}
} // namespace yoyo::gui
