#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

class QComboBox;

namespace yoyo
{
class node_base;

namespace gui
{
class access_input : public property_input
{
  Q_OBJECT

public:
  access_input(std::shared_ptr<node_base> node, std::string property, QWidget* parent = nullptr);
  ~access_input() override;

public Q_SLOTS:
  void onPropertyChanged(yoyo::types::access_t value);

private:
  std::unique_ptr<QComboBox> _input;
  yoyo::types::access_t _buffer;
};
} // namespace gui
} // namespace yoyo
