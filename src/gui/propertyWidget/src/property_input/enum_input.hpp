#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

#include <memory>

namespace Ui
{
class enum_input;
}

namespace yoyo
{
class node_base;

namespace gui
{
class enum_input : public property_input
{
  Q_OBJECT

public:
  explicit enum_input(std::shared_ptr<node_base> node, std::string property,
                      QWidget* parent = nullptr);
  ~enum_input() override;

public Q_SLOTS:
  void onPropertyChanged(yoyo::properties::enum_t value);

private:
  int _selected_index = -1;
  std::unique_ptr<Ui::enum_input> ui;
  yoyo::properties::enum_t _buffer;
};
} // namespace gui
} // namespace yoyo
