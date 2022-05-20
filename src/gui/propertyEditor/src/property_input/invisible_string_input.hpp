#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

class QLineEdit;

namespace yoyo
{
class node_base;

namespace gui
{
class invisible_string_input : public property_input
{
  Q_OBJECT

public:
  invisible_string_input(std::shared_ptr<node_base> node, std::string property,
                         QWidget* parent = nullptr);
  ~invisible_string_input() override;

public Q_SLOTS:
  void onPropertyChanged(yoyo::properties::invisible_string_t value);

private:
  std::unique_ptr<QLineEdit> _input;
  properties::invisible_string_t _buffer;
};
} // namespace gui
} // namespace yoyo
