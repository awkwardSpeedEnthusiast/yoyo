#include "number_input.hpp"

#include "yoyo/node_base.h"

#include <QDoubleValidator>
#include <QIntValidator>
#include <QLayout>
#include <QLineEdit>
#include <QMetaObject>
#include <QMetaProperty>

#include <string>

#include <iostream>

#define MY_METHOD(a) #a

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

  _input->setText(QLocale::system().toString(node->property(propertyName.c_str()).toLongLong()));
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
    text = text.right(text.length() - 2);
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

  connect(_input.get(), &QLineEdit::returnPressed, this, [this, propertyName]() {
    auto text = _input->text();
    auto b = _buffer.value<properties::limited_value_t<T>>();
    if constexpr (std::is_floating_point_v<T>) {
      b._value = QLocale::system().toDouble(text);
    } else if constexpr (std::is_signed_v<T>) {
      b._value = QLocale::system().toLongLong(text);
    } else if constexpr (std::is_unsigned_v<T>) {
      if (text.length() >= 2) {
        b._value = text.right(text.length() - 2).toULongLong(nullptr, 16);
      }
    }
    Q_EMIT propertyChanged(propertyName, QVariant::fromValue(b));
  });

  if constexpr (std::is_signed_v<T>) {
    _input->setText(QLocale::system().toString(value._value));
  } else {
    _input->setText("0x" + QString::number(value._value, 16));
  }
  if constexpr (std::is_floating_point_v<T>) {
    _input->setValidator(new QDoubleValidator(value._min, value._max, 6, _input.get()));
  } else if constexpr (std::is_signed_v<T>) {
    _input->setValidator(new QIntValidator(value._min, value._max, _input.get()));
  }
  if constexpr (std::is_unsigned_v<T>) {
    _input->setInputMask("\\0\\x" + QString(2 * sizeof(T), 'h'));
  }
}

template <typename T>
void number_input::do_setup(std::shared_ptr<node_base> node, std::string const& property_name,
                            int method_index)
{
  if constexpr (std::same_as<T, int8_t> || std::same_as<T, int16_t> || std::same_as<T, int32_t>
                || std::same_as<T, int64_t> || std::same_as<T, char> || std::same_as<T, short>
                || std::same_as<T, int> || std::same_as<T, long> || std::same_as<T, qlonglong>) {
    setup(node, property_name, method_index, static_cast<long>(std::numeric_limits<T>::min()),
          static_cast<long>(std::numeric_limits<T>::max()));
  } else if constexpr (std::same_as<T, float> || std::same_as<T, double>) {
    setup(node, property_name, method_index, std::numeric_limits<T>::min(),
          std::numeric_limits<T>::max());
  } else if constexpr (std::same_as<T, yoyo::properties::limited_uint8_t>
                       || std::same_as<T, yoyo::properties::limited_uint16_t>
                       || std::same_as<T, yoyo::properties::limited_uint32_t>
                       || std::same_as<T, yoyo::properties::limited_uint64_t>
                       || std::same_as<T, yoyo::properties::limited_int8_t>
                       || std::same_as<T, yoyo::properties::limited_int16_t>
                       || std::same_as<T, yoyo::properties::limited_int32_t>
                       || std::same_as<T, yoyo::properties::limited_int64_t>) {
    setup(node, property_name, method_index, node->property(property_name.c_str()).value<T>());
  } else if constexpr (std::same_as<T, yoyo::properties::limited_float_t>) {
    setup(node, property_name, method_index, node->property(property_name.c_str()).value<T>());
  } else if constexpr (std::same_as<T, uint8_t> || std::same_as<T, uchar>
                       || std::same_as<T, uint16_t> || std::same_as<T, ushort>
                       || std::same_as<T, uint32_t> || std::same_as<T, uint>
                       || std::same_as<T, uint64_t> || std::same_as<T, ulong>
                       || std::same_as<T, qulonglong>) {
    QString m = "\\0\\x" + QString { sizeof(T) * 2, 'h' };
    setup(node, property_name, method_index, m);
  }
}

number_input::number_input(std::shared_ptr<node_base> node, std::string propertyName,
                           QWidget* parent)
  : property_input { parent }
  , _input { std::make_unique<QLineEdit>(this) }
{
  layout()->addWidget(_input.get());

  // clang-format off
  static std::map<std::string, int> slot_indices = {
    {"int8_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged_fixed(int8_t)))},
    {"int16_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged_fixed(int16_t)))},
    {"int32_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged_fixed(int32_t)))},
    {"int64_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged_fixed(int64_t)))},
    {"uint8_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged_fixed(uint8_t)))},
    {"uint16_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged_fixed(uint16_t)))},
    {"uint32_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged_fixed(uint32_t)))},
    {"uint64_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged_fixed(uint64_t)))},
    {QMetaType::fromType<yoyo::properties::limited_int8_t>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_int8_t)))},
    {QMetaType::fromType<yoyo::properties::limited_int16_t>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_int16_t)))},
    {QMetaType::fromType<yoyo::properties::limited_int32_t>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_int32_t)))},
    {QMetaType::fromType<yoyo::properties::limited_int64_t>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_int64_t)))},
    {QMetaType::fromType<yoyo::properties::limited_uint8_t>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_uint8_t)))},
    {QMetaType::fromType<yoyo::properties::limited_uint16_t>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_uint16_t)))},
    {QMetaType::fromType<yoyo::properties::limited_uint32_t>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_uint32_t)))},
    {QMetaType::fromType<yoyo::properties::limited_uint64_t>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_uint64_t)))},
    {"yoyo::properties::limited_int8_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_int8_t)))},
    {"yoyo::properties::limited_int16_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_int16_t)))},
    {"yoyo::properties::limited_int32_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_int32_t)))},
    {"yoyo::properties::limited_int64_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_int64_t)))},
    {"yoyo::properties::limited_uint8_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_uint8_t)))},
    {"yoyo::properties::limited_uint16_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_uint16_t)))},
    {"yoyo::properties::limited_uint32_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_uint32_t)))},
    {"yoyo::properties::limited_uint64_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_uint64_t)))},
    {QMetaType::fromType<float>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(float)))},
    {QMetaType::fromType<double>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(double)))},
    {QMetaType::fromType<yoyo::properties::limited_float_t>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_float_t)))},
    {"yoyo::properties::limited_float_t", staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(yoyo::properties::limited_float_t)))},
    {QMetaType::fromType<char>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(char)))},
    {QMetaType::fromType<short>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(short)))},
    {QMetaType::fromType<int>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(int)))},
    {QMetaType::fromType<long>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(long)))},
    {QMetaType::fromType<qlonglong>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(qlonglong)))},
    {QMetaType::fromType<uchar>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(uchar)))},
    {QMetaType::fromType<ushort>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(ushort)))},
    {QMetaType::fromType<uint>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(uint)))},
    {QMetaType::fromType<ulong>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(ulong)))},
    {QMetaType::fromType<qulonglong>().name(), staticMetaObject.indexOfMethod(MY_METHOD(onPropertyChanged(qulonglong)))},
  };
  // clang-format on
  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));

  auto notifyType = [](auto const& signature) -> std::string {
    static QRegularExpression reg { ".*\\((.*)\\)" };
    if (auto match = reg.match(signature); match.hasMatch()) {
      return match.captured(1).toStdString();
    }
    return "";
  };
  auto it = slot_indices.find(notifyType(prop.notifySignal().methodSignature()));
  if (it == slot_indices.end()) {
    throw std::runtime_error("unknown type");
  }

  if (prop.typeName() == QMetaType::fromType<int8_t>().name()) {
    do_setup<uint8_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<int16_t>().name()) {
    do_setup<int16_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<int32_t>().name()) {
    do_setup<int32_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<int64_t>().name()) {
    do_setup<int32_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<uint8_t>().name()) {
    do_setup<uint8_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<uint16_t>().name()) {
    do_setup<uint16_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<uint32_t>().name()) {
    do_setup<uint32_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<uint64_t>().name()) {
    do_setup<uint64_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<char>().name()) {
    do_setup<char>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<short>().name()) {
    do_setup<short>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<int>().name()) {
    do_setup<int>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<long>().name()) {
    do_setup<long>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<qlonglong>().name()) {
    do_setup<qlonglong>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<uchar>().name()) {
    do_setup<uchar>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<ushort>().name()) {
    do_setup<ushort>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<uint>().name()) {
    do_setup<uint>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<ulong>().name()) {
    do_setup<ulong>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<qulonglong>().name()) {
    do_setup<qulonglong>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<yoyo::properties::limited_int8_t>().name()) {
    do_setup<yoyo::properties::limited_int8_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<yoyo::properties::limited_int16_t>().name()) {
    do_setup<yoyo::properties::limited_int16_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<yoyo::properties::limited_int32_t>().name()) {
    do_setup<yoyo::properties::limited_int32_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<yoyo::properties::limited_int64_t>().name()) {
    do_setup<yoyo::properties::limited_int64_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<yoyo::properties::limited_uint8_t>().name()) {
    do_setup<yoyo::properties::limited_uint8_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<yoyo::properties::limited_uint16_t>().name()) {
    do_setup<yoyo::properties::limited_uint16_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<yoyo::properties::limited_uint32_t>().name()) {
    do_setup<yoyo::properties::limited_uint32_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<yoyo::properties::limited_uint64_t>().name()) {
    do_setup<yoyo::properties::limited_uint64_t>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<float>().name()) {
    do_setup<float>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<double>().name()) {
    do_setup<double>(node, propertyName, it->second);
  } else if (prop.typeName() == QMetaType::fromType<yoyo::properties::limited_float_t>().name()) {
    do_setup<yoyo::properties::limited_float_t>(node, propertyName, it->second);
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
  _input->setText("0x" + QString::number(value._value, 16));
}

auto number_input::onPropertyChanged(properties::limited_uint16_t value) -> void
{
  _buffer = QVariant::fromValue(value);
  _input->setText("0x" + QString::number(value._value, 16));
}

auto number_input::onPropertyChanged(properties::limited_uint32_t value) -> void
{
  _buffer = QVariant::fromValue(value);
  _input->setText("0x" + QString::number(value._value, 16));
}

auto number_input::onPropertyChanged(properties::limited_uint64_t value) -> void
{
  _buffer = QVariant::fromValue(value);
  _input->setText("0x" + QString::number(value._value, 16));
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
