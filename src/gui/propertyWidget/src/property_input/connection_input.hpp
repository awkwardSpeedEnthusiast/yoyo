#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

namespace Ui
{
class connection_input;
}

namespace yoyo
{
class node_base;

namespace gui
{
class connection_input : public property_input
{
  Q_OBJECT

public:
  connection_input(std::shared_ptr<node_base> node, std::string property,
                   QWidget* parent = nullptr);
  ~connection_input() override;

public Q_SLOTS:
  void onPropertyChanged(yoyo::properties::connection_t value);

private:
  std::unique_ptr<Ui::connection_input> _ui;
  properties::connection_t _buffer;
};
} // namespace gui
} // namespace yoyo
