#pragma once

#include "property_input.hpp"

#include "yoyo/property_types.h"

class QComboBox;

namespace yoyo
{
class node_base;

namespace gui
{
class enumeration_input : public property_input
{
  Q_OBJECT

public:
  enumeration_input(std::shared_ptr<node_base> node, std::string property,
                    QWidget* parent = nullptr);
  ~enumeration_input() override;

public Q_SLOTS:
  void onPropertyChanged(yoyo::types::access_t value);
  void onPropertyChanged(yoyo::properties::transmission_direction_t value);

private:
  template <typename T>
  auto setup(std::shared_ptr<node_base> node, std::string property) -> void;

private:
  std::unique_ptr<QComboBox> _input;
  std::variant<yoyo::types::access_t, yoyo::properties::transmission_direction_t> _buffer;
};
} // namespace gui
} // namespace yoyo
