#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

namespace Ui
{
class text_input;
}

namespace yoyo
{
class node_base;

namespace gui
{
class text_input : public property_input
{
  Q_OBJECT

public:
  text_input(std::shared_ptr<node_base> node, std::string property, QWidget* parent = nullptr);
  ~text_input() override;

public Q_SLOTS:
  void onPropertyChanged(yoyo::properties::text_t value);

private:
  std::unique_ptr<Ui::text_input> _ui;
  properties::text_t _buffer;
};
} // namespace gui
} // namespace yoyo
