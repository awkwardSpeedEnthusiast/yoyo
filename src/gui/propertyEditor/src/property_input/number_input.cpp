#include "number_input.hpp"

#include "yoyo/node_base.h"

#include <QDoubleValidator>
#include <QIntValidator>
#include <QLayout>
#include <QLineEdit>
#include <QMetaObject>
#include <QMetaProperty>

#include <string>

using std::string_literals::operator""s;

namespace yoyo::gui
{
auto number_input::setup(std::shared_ptr<node_base> node, std::string propertyName,
                         int method_index, long min, long max) -> void
{
  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));
  connect(node.get(), prop.notifySignal(), this, staticMetaObject.method(method_index));
  std::function<void()> slot = [this, propertyName]() {
    auto text = _input->text();
    Q_EMIT propertyChanged(propertyName, QLocale::system().toLongLong(text));
  };

  connect(_input.get(), &QLineEdit::returnPressed, this, slot);

  _input->setText(QString::number(node->property(propertyName.c_str()).toLongLong(), 10));
  _input->setValidator(new QIntValidator(min, max, _input.get()));
}

auto number_input::setup(std::shared_ptr<node_base> node, std::string propertyName,
                         int method_index, double min, double max) -> void
{
  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));
  connect(node.get(), prop.notifySignal(), this, staticMetaObject.method(method_index));
  std::function<void()> slot = [this, propertyName]() {
    auto text = _input->text();
    Q_EMIT propertyChanged(propertyName, QLocale::system().toDouble(text));
  };

  connect(_input.get(), &QLineEdit::returnPressed, this, slot);

  _input->setText(QLocale::system().toString(node->property(propertyName.c_str()).toDouble()));
  _input->setValidator(new QDoubleValidator(min, max, 6, _input.get()));
}

auto number_input::setup(std::shared_ptr<node_base> node, std::string propertyName,
                         int method_index, QString input_mask) -> void
{
  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));
  connect(node.get(), prop.notifySignal(), this, staticMetaObject.method(method_index));
  std::function<void()> slot = [this, propertyName]() {
    auto text = _input->text();

    if (text.length() <= 2) {
      return;
    }

    Q_EMIT propertyChanged(propertyName, text.toULongLong(nullptr, 16));
  };

  connect(_input.get(), &QLineEdit::returnPressed, this, slot);

  _input->setInputMask(input_mask);
  _input->setText("0x" + QString::number(node->property(propertyName.c_str()).toULongLong(), 16));
}

template <typename T>
auto number_input::setup(std::shared_ptr<node_base> node, std::string propertyName,
                         int method_index, properties::limited_value_t<T> value) -> void
{
  _buffer = QVariant::fromValue(value);
  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));
  connect(node.get(), prop.notifySignal(), this, staticMetaObject.method(method_index));
  std::function<void()> slot = [this, propertyName]() {
    auto text = _input->text();
    auto b = _buffer.value<properties::limited_value_t<T>>();
    if constexpr (std::is_floating_point_v<T>) {
      b._value = QLocale::system().toDouble(text);
    } else if constexpr (std::is_signed_v<T>) {
      b._value = QLocale::system().toLongLong(text);
    } else {
      b._value = QLocale::system().toULongLong(text);
    }
    Q_EMIT propertyChanged(propertyName, QVariant::fromValue(b));
  };

  connect(_input.get(), &QLineEdit::returnPressed, this, slot);

  _input->setText(QLocale::system().toString(value._value));
  if constexpr (std::is_floating_point_v<T>) {
    _input->setValidator(new QDoubleValidator(value._min, value._max, 6, _input.get()));
  } else {
    _input->setValidator(new QIntValidator(value._min, value._max, _input.get()));
  }
}

number_input::number_input(std::shared_ptr<node_base> node, std::string propertyName,
                           QWidget* parent)
  : property_input { parent }
  , _input { std::make_unique<QLineEdit>(this) }
{
  layout()->addWidget(_input.get());

  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));

  if (prop.typeName() == "int8_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset(),
          static_cast<long>(std::numeric_limits<int8_t>::min()),
          std::numeric_limits<int8_t>::max());
  } else if (prop.typeName() == "int16_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 1,
          static_cast<long>(std::numeric_limits<int16_t>::min()),
          std::numeric_limits<int16_t>::max());
  } else if (prop.typeName() == "int32_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 2,
          static_cast<long>(std::numeric_limits<int32_t>::min()),
          std::numeric_limits<int32_t>::max());
  } else if (prop.typeName() == "int64_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 3,
          static_cast<long>(std::numeric_limits<int32_t>::min()),
          std::numeric_limits<int32_t>::max());
  } else if (prop.typeName() == "uint8_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 4, "\\0\\xhh");
  } else if (prop.typeName() == "uint16_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 5, "\\0\\xhhhh");
  } else if (prop.typeName() == "uint32_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 6, "\\0\\xhhhhhhhh");
  } else if (prop.typeName() == "uint64_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 7, "\\0\\xhhhhhhhhhhhhhhhh");
  } else if (prop.typeName() == "char"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 8,
          static_cast<long>(std::numeric_limits<char>::min()), std::numeric_limits<char>::max());
  } else if (prop.typeName() == "short"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 9,
          static_cast<long>(std::numeric_limits<short>::min()), std::numeric_limits<short>::max());
  } else if (prop.typeName() == "int"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 10,
          static_cast<long>(std::numeric_limits<int>::min()), std::numeric_limits<int>::max());
  } else if (prop.typeName() == "long"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 11,
          std::numeric_limits<long>::min(), std::numeric_limits<long>::max());
  } else if (prop.typeName() == "qlonglong"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 12,
          std::numeric_limits<long>::min(), std::numeric_limits<long>::max());
  } else if (prop.typeName() == "uchar"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 13, "\\0\\xhh");
  } else if (prop.typeName() == "ushort"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 14, "\\0\\xhhhh");
  } else if (prop.typeName() == "uint"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 15, "\\0\\xhhhhhhhh");
  } else if (prop.typeName() == "ulong"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 16, "\\0\\xhhhhhhhhhhhhhhhh");
  } else if (prop.typeName() == "qulonglong"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 17, "\\0\\xhhhhhhhhhhhhhhhh");
  } else if (prop.typeName() == "yoyo::properties::limited_int8_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 18,
          node->property(propertyName.c_str()).value<properties::limited_int8_t>());
  } else if (prop.typeName() == "yoyo::properties::limited_int16_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 19,
          node->property(propertyName.c_str()).value<properties::limited_int16_t>());
  } else if (prop.typeName() == "yoyo::properties::limited_int32_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 20,
          node->property(propertyName.c_str()).value<properties::limited_int32_t>());
  } else if (prop.typeName() == "yoyo::properties::limited_int64_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 21,
          node->property(propertyName.c_str()).value<properties::limited_int64_t>());
  } else if (prop.typeName() == "yoyo::properties::limited_uint8_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 22,
          node->property(propertyName.c_str()).value<properties::limited_uint8_t>());
  } else if (prop.typeName() == "yoyo::properties::limited_uint16_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 23,
          node->property(propertyName.c_str()).value<properties::limited_uint16_t>());
  } else if (prop.typeName() == "yoyo::properties::limited_uint32_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 24,
          node->property(propertyName.c_str()).value<properties::limited_uint32_t>());
  } else if (prop.typeName() == "yoyo::properties::limited_uint64_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 25,
          node->property(propertyName.c_str()).value<properties::limited_uint64_t>());
  } else if (prop.typeName() == "float"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 26,
          std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
  } else if (prop.typeName() == "double"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 27,
          std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
  } else if (prop.typeName() == "yoyo::properties::limited_float_t"s) {
    setup(node, propertyName, staticMetaObject.methodOffset() + 28,
          node->property(propertyName.c_str()).value<properties::limited_float_t>());
  } else {
    throw std::runtime_error("unknown type");
  }
}

number_input::~number_input() = default;

auto number_input::onPropertyChanged_fixed(int8_t value) -> void
{
  _input->setText(QLocale::system().toString(value));
}

auto number_input::onPropertyChanged_fixed(int16_t value) -> void
{
  _input->setText(QLocale::system().toString(value));
}

auto number_input::onPropertyChanged_fixed(int32_t value) -> void
{
  _input->setText(QLocale::system().toString(value));
}

auto number_input::onPropertyChanged_fixed(int64_t value) -> void
{
  _input->setText(QLocale::system().toString(value));
}

auto number_input::onPropertyChanged_fixed(uint8_t value) -> void
{
  _input->setText("0x" + QString::number(value, 16));
}

auto number_input::onPropertyChanged_fixed(uint16_t value) -> void
{
  _input->setText("0x" + QString::number(value, 16));
}

auto number_input::onPropertyChanged_fixed(uint32_t value) -> void
{
  _input->setText("0x" + QString::number(value, 16));
}

auto number_input::onPropertyChanged_fixed(uint64_t value) -> void
{
  _input->setText("0x" + QString::number(value, 16));
}

auto number_input::onPropertyChanged(char value) -> void
{
  _input->setText(QLocale::system().toString(value));
}

auto number_input::onPropertyChanged(short value) -> void
{
  _input->setText(QLocale::system().toString(value));
}

auto number_input::onPropertyChanged(int value) -> void
{
  _input->setText(QLocale::system().toString(value));
}

auto number_input::onPropertyChanged(long value) -> void
{
  _input->setText(QLocale::system().toString(value));
}

auto number_input::onPropertyChanged(long long value) -> void
{
  _input->setText(QLocale::system().toString(value));
}

auto number_input::onPropertyChanged(unsigned char value) -> void
{
  _input->setText("0x" + QString::number(value, 16));
}

auto number_input::onPropertyChanged(unsigned short value) -> void
{
  _input->setText("0x" + QString::number(value, 16));
}

auto number_input::onPropertyChanged(unsigned int value) -> void
{
  _input->setText("0x" + QString::number(value, 16));
}

auto number_input::onPropertyChanged(unsigned long value) -> void
{
  _input->setText("0x" + QString::number(value, 16));
}

auto number_input::onPropertyChanged(unsigned long long value) -> void
{
  _input->setText("0x" + QString::number(value, 16));
}

auto number_input::onPropertyChanged(properties::limited_int8_t value) -> void
{
  _buffer = QVariant::fromValue(value);
  _input->setText(QLocale::system().toString(value._value));
}

auto number_input::onPropertyChanged(properties::limited_int16_t value) -> void
{
  _buffer = QVariant::fromValue(value);
  _input->setText(QLocale::system().toString(value._value));
}

auto number_input::onPropertyChanged(properties::limited_int32_t value) -> void
{
  _buffer = QVariant::fromValue(value);
  _input->setText(QLocale::system().toString(value._value));
}

auto number_input::onPropertyChanged(properties::limited_int64_t value) -> void
{
  _buffer = QVariant::fromValue(value);
  _input->setText(QLocale::system().toString(value._value));
}

auto number_input::onPropertyChanged(properties::limited_uint8_t value) -> void
{
  _buffer = QVariant::fromValue(value);
  _input->setText(QLocale::system().toString(value._value));
}

auto number_input::onPropertyChanged(properties::limited_uint16_t value) -> void
{
  _buffer = QVariant::fromValue(value);
  _input->setText(QLocale::system().toString(value._value));
}

auto number_input::onPropertyChanged(properties::limited_uint32_t value) -> void
{
  _buffer = QVariant::fromValue(value);
  _input->setText(QLocale::system().toString(value._value));
}

auto number_input::onPropertyChanged(properties::limited_uint64_t value) -> void
{
  _buffer = QVariant::fromValue(value);
  _input->setText(QLocale::system().toString(value._value));
}

auto number_input::onPropertyChanged(float value) -> void
{
  _input->setText(QLocale::system().toString(value));
}

auto number_input::onPropertyChanged(double value) -> void
{
  _input->setText(QLocale::system().toString(value));
}

auto number_input::onPropertyChanged(yoyo::properties::limited_float_t value) -> void
{
  _buffer = QVariant::fromValue(value);
  _input->setText(QLocale::system().toString(value._value));
}
} // namespace yoyo::gui
