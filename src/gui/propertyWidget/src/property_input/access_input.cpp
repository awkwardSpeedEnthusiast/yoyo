#include "access_input.hpp"

#include "yoyo/node_base.h"

#include <QComboBox>
#include <QLayout>
#include <QMetaObject>
#include <QMetaProperty>

namespace yoyo::gui
{
access_input::access_input(std::shared_ptr<node_base> node, std::string propertyName,
                           QWidget* parent)
  : property_input { parent }
  , _input { std::make_unique<QComboBox>(this) }
{
  _input->addItems(QStringList {} << tr("None") << tr("Read") << tr("Write") << tr("Read-Write"));
  layout()->addWidget(_input.get());

  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));
  connect(node.get(), prop.notifySignal(), this,
          staticMetaObject.method(staticMetaObject.methodOffset()));
  connect(_input.get(), static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, [this, propertyName](auto) {
            auto value = static_cast<types::access_t>(_input->currentIndex());

            if (_buffer != value) {
              Q_EMIT propertyChanged(propertyName, QVariant::fromValue(value));
            }
          });
  onPropertyChanged(node->property(propertyName.c_str()).value<types::access_t>());
}

access_input::~access_input() = default;

auto access_input::onPropertyChanged(yoyo::types::access_t value) -> void
{
  _buffer = value;
  _input->setCurrentIndex(static_cast<int>(value));
}
} // namespace yoyo::gui
