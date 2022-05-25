#pragma once

#include "property_input.hpp"

class QLineEdit;

namespace yoyo
{
class node_base;

namespace gui
{
class qstring_input : public property_input
{
  Q_OBJECT

public:
  qstring_input(std::shared_ptr<node_base> node, std::string property, QWidget* parent = nullptr);
  ~qstring_input() override;

public Q_SLOTS:
  void onPropertyChanged(QString value);

private:
  std::unique_ptr<QLineEdit> _input;
};
} // namespace gui
} // namespace yoyo
