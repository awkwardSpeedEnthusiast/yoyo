#include "qstring_input.hpp"

#include "yoyo/node_base.h"

#include <QLayout>
#include <QLineEdit>
#include <QMetaObject>
#include <QMetaProperty>

namespace yoyo::gui
{
qstring_input::qstring_input(std::shared_ptr<node_base> node, std::string propertyName,
                             QWidget* parent)
  : property_input { parent }
  , _input { std::make_unique<QLineEdit>(this) }
{
  layout()->addWidget(_input.get());

  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));
  connect(node.get(), prop.notifySignal(), this,
          staticMetaObject.method(staticMetaObject.methodOffset()));
  connect(_input.get(), &QLineEdit::returnPressed, this,
          [this, propertyName]() { Q_EMIT propertyChanged(propertyName, _input->text()); });
  _input->setText(node->property(propertyName.c_str()).toString());
}

qstring_input::~qstring_input() = default;

auto qstring_input::onPropertyChanged(QString value) -> void
{
  _input->setText(value);
}
} // namespace yoyo::gui
