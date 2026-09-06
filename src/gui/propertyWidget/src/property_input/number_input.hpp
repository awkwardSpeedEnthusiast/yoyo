#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

#include <type_traits>

class QLineEdit;

namespace yoyo
{
class node_base;

namespace gui
{
class number_input : public property_input
{
  Q_OBJECT

public:
  number_input(std::shared_ptr<node_base> node, std::string property, QWidget* parent = nullptr);
  ~number_input() override;

public Q_SLOTS:
  void onPropertyChanged_fixed(int8_t value);
  void onPropertyChanged_fixed(int16_t value);
  void onPropertyChanged_fixed(int32_t value);
  void onPropertyChanged_fixed(int64_t value);
  void onPropertyChanged_fixed(uint8_t value);
  void onPropertyChanged_fixed(uint16_t value);
  void onPropertyChanged_fixed(uint32_t value);
  void onPropertyChanged_fixed(uint64_t value);
  void onPropertyChanged(char value);
  void onPropertyChanged(short value);
  void onPropertyChanged(int value);
  void onPropertyChanged(long value);
  void onPropertyChanged(long long value);
  void onPropertyChanged(unsigned char value);
  void onPropertyChanged(unsigned short value);
  void onPropertyChanged(unsigned int value);
  void onPropertyChanged(unsigned long value);
  void onPropertyChanged(unsigned long long value);
  void onPropertyChanged(yoyo::properties::limited_int8_t value);
  void onPropertyChanged(yoyo::properties::limited_int16_t value);
  void onPropertyChanged(yoyo::properties::limited_int32_t value);
  void onPropertyChanged(yoyo::properties::limited_int64_t value);
  void onPropertyChanged(yoyo::properties::limited_uint8_t value);
  void onPropertyChanged(yoyo::properties::limited_uint16_t value);
  void onPropertyChanged(yoyo::properties::limited_uint32_t value);
  void onPropertyChanged(yoyo::properties::limited_uint64_t value);
  void onPropertyChanged(float value);
  void onPropertyChanged(double value);
  void onPropertyChanged(yoyo::properties::limited_float_t value);

private:
  auto setup(std::shared_ptr<node_base> node, std::string propertyName, int method_index,
             QString input_mask) -> void;
  auto setup(std::shared_ptr<node_base> node, std::string propertyName, int method_index, long min,
             long max) -> void;
  auto setup(std::shared_ptr<node_base> node, std::string propertyName, int method_index,
             double min, double max) -> void;
  template <typename T>
  auto setup(std::shared_ptr<node_base> node, std::string propertyName, int method_index,
             properties::limited_value_t<T> value) -> void;
  template <typename T>
  void do_setup(std::shared_ptr<node_base> node, std::string const& property_name,
                int method_index);

private:
  std::unique_ptr<QLineEdit> _input;
  QVariant _buffer;
};
} // namespace gui
} // namespace yoyo
