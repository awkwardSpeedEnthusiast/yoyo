#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

class QComboBox;

namespace yoyo
{
class node_base;

namespace gui
{
class connected_boolean_input : public property_input
{
  Q_OBJECT

public:
  connected_boolean_input(std::shared_ptr<node_base> node, std::string property,
                          QWidget* parent = nullptr);
  ~connected_boolean_input() override;

public Q_SLOTS:
  void onPropertyChanged(yoyo::properties::connected_boolean_t value);

private:
  properties::connected_boolean_t _buffer;
  std::unique_ptr<QComboBox> _input;
};
} // namespace gui
} // namespace yoyo
