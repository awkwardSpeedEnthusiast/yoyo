#pragma once

#include <QWidget>

#include <memory>

namespace Ui {
class HelpWidget;
}
namespace yoyo {
namespace gui {

class help_widget : public QWidget
{
  Q_OBJECT

public:
  explicit help_widget(QWidget *parent = nullptr);
  ~help_widget();

private:
  std::unique_ptr<Ui::HelpWidget> _ui;
};

}
}
