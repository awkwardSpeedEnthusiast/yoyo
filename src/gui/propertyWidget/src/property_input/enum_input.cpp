#include "enum_input.hpp"
#include "ui_enum_input.h"

#include "yoyo/node_base.h"

#include <QMetaObject>
#include <QMetaProperty>

namespace yoyo::gui
{
enum_input::enum_input(std::shared_ptr<node_base> node, std::string propertyName, QWidget* parent)
  : property_input { parent, false }
  , ui { std::make_unique<Ui::enum_input>() }
{
  ui->setupUi(this);
  ui->data_box->setEnabled(false);
  ui->data_input->setEnabled(false);
  ui->value_input->setEnabled(false);
  ui->title_input->setEnabled(false);
  ui->matchevent_input->setEnabled(false);
  auto prop =
    node->metaObject()->property(node->metaObject()->indexOfProperty(propertyName.c_str()));
  connect(node.get(), prop.notifySignal(), this,
          staticMetaObject.method(staticMetaObject.methodOffset()));

  connect(ui->enabled, &QCheckBox::clicked, this, [this, propertyName]() {
    _buffer._enabled = ui->enabled->isChecked();
    Q_EMIT propertyChanged(propertyName, QVariant::fromValue(_buffer));
  });
  connect(ui->data_box, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, [this]() {
            if (ui->data_box->currentIndex() == _selected_index) {
              return;
            }

            _selected_index = ui->data_box->currentIndex();

            if (ui->data_box->currentIndex() < 0) {
              ui->value_input->setText("");
              ui->data_input->setText("");
              ui->title_input->setText("");
              ui->matchevent_input->setText("");
              return;
            }

            auto it = _buffer._values.begin();
            std::advance(it, ui->data_box->currentIndex());
            ui->data_input->setText(QString::number(it->first, 16));
            ui->value_input->setText(std::get<0>(it->second));
            ui->title_input->setText(std::get<1>(it->second));
            ui->matchevent_input->setText(std::get<2>(it->second)._text);
          });
  connect(ui->data_input, &QLineEdit::returnPressed, this, [this, propertyName]() {
    auto val = ui->data_input->text().toULongLong(nullptr, 16);

    auto it = _buffer._values.find(val);

    if (it == _buffer._values.end()) {
      auto copy = _buffer;
      copy._values.insert({ val, { "", "", { "", properties::script_t::type::NATIVE } } });
      _selected_index = std::distance(copy._values.begin(), copy._values.find(val));
      Q_EMIT propertyChanged(propertyName, QVariant::fromValue(copy));
      return;
    }

    ui->data_box->setCurrentIndex(
      std::distance(_buffer._values.begin(), _buffer._values.find(val)));
  });
  connect(ui->value_input, &QLineEdit::returnPressed, this, [this, propertyName]() {
    if (ui->data_box->currentIndex() < 0) {
      return;
    }

    auto it = _buffer._values.begin();
    std::advance(it, ui->data_box->currentIndex());
    std::get<0>(it->second) = ui->value_input->text();
    Q_EMIT propertyChanged(propertyName, QVariant::fromValue(_buffer));
  });
  connect(ui->title_input, &QLineEdit::returnPressed, this, [this, propertyName]() {
    if (ui->data_box->currentIndex() < 0) {
      return;
    }

    auto it = _buffer._values.begin();
    std::advance(it, ui->data_box->currentIndex());
    std::get<1>(it->second) = ui->title_input->text();
    Q_EMIT propertyChanged(propertyName, QVariant::fromValue(_buffer));
  });
  connect(ui->matchevent_input, &QLineEdit::returnPressed, this, [this, propertyName]() {
    if (ui->data_box->currentIndex() < 0) {
      return;
    }

    auto it = _buffer._values.begin();
    std::advance(it, ui->data_box->currentIndex());
    std::get<2>(it->second)._text = ui->matchevent_input->text();
    Q_EMIT propertyChanged(propertyName, QVariant::fromValue(_buffer));
  });
  onPropertyChanged(node->property(propertyName.c_str()).value<properties::enum_t>());
}

enum_input::~enum_input() = default;

auto enum_input::onPropertyChanged(properties::enum_t value) -> void
{
  _buffer = value;
  ui->enabled->setChecked(_buffer._enabled);

  if (_buffer._enabled) {
    if (static_cast<size_t>(ui->data_box->count()) != value._values.size()) {
      auto save = _selected_index;
      ui->data_box->clear();
      std::for_each(value._values.begin(), value._values.end(), [this](auto const& item) {
        ui->data_box->addItem("0x" + QString::number(item.first, 16));
      });

      if (save != -1) {
        ui->data_box->setCurrentIndex(save);
      }
    } else if (ui->data_box->currentIndex() >= 0) {
      auto it = _buffer._values.begin();
      std::advance(it, ui->data_box->currentIndex());

      if (it != _buffer._values.end()) {
        ui->data_input->setText(QString::number(it->first, 16));
        ui->value_input->setText(std::get<0>(it->second));
        ui->title_input->setText(std::get<1>(it->second));
        ui->matchevent_input->setText(std::get<2>(it->second)._text);
      }
    }
  } else {
    ui->data_box->clear();
    ui->data_input->setText("");
    ui->title_input->setText("");
    ui->value_input->setText("");
    ui->matchevent_input->setText("");
  }
}
} // namespace yoyo::gui
