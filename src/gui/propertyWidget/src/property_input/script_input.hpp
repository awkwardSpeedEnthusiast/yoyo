#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

#include <memory>

namespace yoyo
{
class node_base;
}
namespace Ui
{
class script_input;
}

namespace yoyo
{
namespace gui
{
class script_input : public property_input
{
  Q_OBJECT

public:
  explicit script_input(std::shared_ptr<node_base> node, std::string property,
                        QWidget* parent = nullptr);
  ~script_input() override;
public Q_SLOTS:
  void onPropertyChanged(yoyo::properties::script_t value);

private:
  std::unique_ptr<Ui::script_input> ui;
  properties::script_t _buffer { "", properties::script_t::type::FILE };
};
} // namespace gui
} // namespace yoyo
