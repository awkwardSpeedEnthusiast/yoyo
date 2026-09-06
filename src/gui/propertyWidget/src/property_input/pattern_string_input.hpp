#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

class QLineEdit;
class QRegExpValidator;

namespace yoyo
{
class node_base;

namespace gui
{
class pattern_string_input : public property_input
{
  Q_OBJECT

public:
  pattern_string_input(std::shared_ptr<node_base> node, std::string property,
                       QWidget* parent = nullptr);
  ~pattern_string_input() override;

public Q_SLOTS:
  void onPropertyChanged(yoyo::properties::patterned_string_t value);

private:
  std::unique_ptr<QLineEdit> _input;
  properties::patterned_string_t _buffer;
};
} // namespace gui
} // namespace yoyo
