#include "bool_input.hpp"

#include "yoyo/node_base.h"

#include <QCheckBox>
#include <QLayout>
#include <QMetaObject>
#include <QMetaProperty>

namespace yoyo::gui
{
bool_input::bool_input(std::shared_ptr<node_base> node, std::string propertyName, QWidget* parent)
  : property_input { parent }
  , _input { std::make_unique<QCheckBox>(this) }
{
  layout()->addWidget(_input.get());

  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));
  connect(node.get(), prop.notifySignal(), this,
          staticMetaObject.method(staticMetaObject.methodOffset()));
  connect(_input.get(), &QCheckBox::clicked, this,
          [this, propertyName]() { Q_EMIT propertyChanged(propertyName, _input->isChecked()); });
  _input->setChecked(node->property(propertyName.c_str()).toBool());
}

bool_input::~bool_input() = default;

auto bool_input::onPropertyChanged(bool value) -> void
{
  _input->setChecked(value);
}
} // namespace yoyo::gui
