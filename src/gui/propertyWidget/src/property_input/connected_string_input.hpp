#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

class QLineEdit;

namespace yoyo
{
class node_base;

namespace gui
{
class connected_string_input : public property_input
{
  Q_OBJECT

public:
  connected_string_input(std::shared_ptr<node_base> node, std::string property,
                         QWidget* parent = nullptr);
  ~connected_string_input() override;

public Q_SLOTS:
  void onPropertyChanged(yoyo::properties::connected_string_t value);

private:
  std::unique_ptr<QLineEdit> _input;
  properties::connected_string_t _buffer;
};
} // namespace gui
} // namespace yoyo
