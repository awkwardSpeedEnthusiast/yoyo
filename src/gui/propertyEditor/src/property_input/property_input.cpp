#include "property_input.hpp"

#include <QBoxLayout>
namespace yoyo::gui
{
property_input::property_input(QWidget* parent, bool needs_layout)
  : QWidget { parent }
{
  if (needs_layout) {
    setLayout(new QBoxLayout { QBoxLayout::LeftToRight, this });
  }
}
} // namespace yoyo::gui
