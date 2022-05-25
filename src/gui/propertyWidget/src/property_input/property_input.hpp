#pragma once

#include <QVariant>
#include <QWidget>

namespace yoyo
{
namespace gui
{
class property_input : public QWidget
{
  Q_OBJECT
public:
  property_input(QWidget* parent = nullptr, bool needs_layout = true);

Q_SIGNALS:
  void propertyChanged(std::string property, QVariant value);
  void visibilityChanged(bool visible);
};
} // namespace gui
} // namespace yoyo
