#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

class QComboBox;

namespace yoyo
{
class node_base;

namespace gui
{
class layout_input : public property_input
{
  Q_OBJECT

public:
  layout_input(std::shared_ptr<node_base> node, std::string property, QWidget* parent = nullptr);
  ~layout_input() override;

public Q_SLOTS:
  void onPropertyChanged(yoyo::types::layout_direction_t value);
  void onPropertyChanged(yoyo::properties::invisible_layout_direction_t value);

private:
  types::layout_direction_t _buffer;
  properties::invisible_layout_direction_t _invisible_buffer;
  std::unique_ptr<QComboBox> _input;
};
} // namespace gui
} // namespace yoyo
