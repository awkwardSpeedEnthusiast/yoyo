#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

class QLineEdit;

namespace yoyo
{
class node_base;

namespace gui
{
class connection_in_input : public property_input
{
  Q_OBJECT

public:
  connection_in_input(std::shared_ptr<node_base> node, std::string property,
                      QWidget* parent = nullptr);
  ~connection_in_input() override;

public Q_SLOTS:
  void onPropertyChanged(yoyo::properties::in_connection_t value);

private:
  std::unique_ptr<QLineEdit> _input;
  properties::in_connection_t _buffer;
};
} // namespace gui
} // namespace yoyo
