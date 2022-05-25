#pragma once

#include "property_input.hpp"

class QCheckBox;

namespace yoyo
{
class node_base;

namespace gui
{
class bool_input : public property_input
{
  Q_OBJECT

public:
  bool_input(std::shared_ptr<node_base> node, std::string property, QWidget* parent = nullptr);
  ~bool_input() override;

public Q_SLOTS:
  void onPropertyChanged(bool value);

private:
  std::unique_ptr<QCheckBox> _input;
};
} // namespace gui
} // namespace yoyo
