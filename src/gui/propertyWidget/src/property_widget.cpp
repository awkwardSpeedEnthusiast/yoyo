#include "property_widget.h"
#include "property_input/property_input_factory.hpp"
#include "ui_property_widget.h"

#include "command_handler.h"
#include "tree_utils.hpp"
#include "yoyo/documentation.h"
#include "yoyo/node_base.h"

#include <QMetaObject>
#include <QMetaProperty>

#include <vector>

namespace yoyo::gui
{
class property_widget::impl
{
public:
  std::vector<std::shared_ptr<QWidget>> _inputs;
};

property_widget::property_widget(QWidget* parent)
  : QDockWidget { parent }
  , ui { std::make_unique<Ui::property_widget>() }
  , _p { std::make_unique<impl>() }
{
  ui->setupUi(this);
  itemSelected({}, {});
}

property_widget::~property_widget() = default;

auto property_widget::itemSelected(std::shared_ptr<node_base> item,
                                   std::shared_ptr<documentation> docu) -> void
{
  ui->type_label->setText("");
  _p->_inputs.clear();

  ui->type_label->setVisible(item != nullptr);
  ui->label->setVisible(item != nullptr);

  if (!item) {
    return;
  }
  ui->type_label->setText(item->type());
  auto meta = item->metaObject();

  int count = 1;

  auto addProperty = [this, docu](auto item, auto property, auto row) {
    auto it = property::factory.find(property.typeName());

    if (it != property::factory.end()) {
      try {
        auto input = it->second(item, property.name(), ui->content_widget);

        if (!input) {
          return;
        }

        auto [title, d, tooltip, dd] = docu->property(property.name());
        auto label = std::make_shared<QLabel>(ui->content_widget);
        ui->formLayout->setWidget(row, QFormLayout::LabelRole, label.get());
        ui->formLayout->setWidget(row, QFormLayout::FieldRole, input.get());
        label->setText(title);
        label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
        label->setToolTip(tooltip);
        input->setToolTip(tooltip);

        connect(input.get(), &property_input::propertyChanged, this, [item](auto p, auto value) {
          auto exec = [item, value, p]() { item->setProperty(p.c_str(), value); };

          if (auto handler = command::commandhandler()) {
            auto id = utilities::calculatePath<utilities::path_strategy_t::INDEX>(item);
            auto root = utilities::get_root(item);
            auto undo = [id, value = item->property(p.c_str()), p, root]() {
              auto item = utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(id, root);
              item->setProperty(p.c_str(), value);
            };
            auto redo = [id, value, p, root]() {
              auto item = utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(id, root);
              item->setProperty(p.c_str(), value);
            };
            handler->execute({ tr("Set property %1").arg(p.c_str()), exec, undo, redo });
          } else {
            exec();
          }
        });
        connect(input.get(), &property_input::visibilityChanged, label.get(), [label](auto v) {
          label->setVisible(v);

          if (v) {
            label->show();
          } else {
            label->hide();
          }
        });
        _p->_inputs.push_back(label);
        _p->_inputs.push_back(input);
      } catch (std::runtime_error&) {
        return;
      }
    }
  };

  addProperty(item, meta->property(meta->indexOfProperty("name")), count++);

  for (int i = node_base::staticMetaObject.propertyCount(); i < meta->propertyCount(); i++) {
    addProperty(item, meta->property(i), count++);
  }
}
} // namespace yoyo::gui
