#include "combo_box.hpp"

#include <QComboBox>
#include <QLabel>
#include <QMetaType>
#include <QVariant>

#include <boost/signals2.hpp>
#include <boost/uuid/string_generator.hpp>

#include <set>

namespace yoyo
{
boost::uuids::uuid const combo_box::_typeId =
  boost::uuids::string_generator {}("286a8cfb-46b2-487d-ae8a-6f614648a494");

struct combo_box::Impl {
  Impl()
    : _connection { [this](std::function<void(QVariant)> subscriber) {
                     return _signal.connect(subscriber);
                   },
                    [this](QVariant value) {
                      if (value.canConvert<uint64_t>()) {
                        auto it = _enum_values._values.find(value.toULongLong());

                        if (it != _enum_values._values.end()) {
                          _edit->setCurrentIndex(std::distance(_enum_values._values.begin(), it));
                        } else {
                          _edit->setCurrentIndex(-1);
                        }
                      }
                    },
                    [this](types::value_t t) { return supports(t); },
                    "",
                    "",
                    "" }
    , _edit(new QComboBox)
    , _title_widget(new QLabel)
    , _unit_widget(new QLabel)
  {
    _edit->setObjectName("__comboBox");
    _title_widget->setObjectName("__title");
    _unit_widget->setObjectName("__unit");
  }
  yoyo::properties::connected_string_t _title;
  yoyo::properties::connected_string_t _unit;
  boost::signals2::signal<void(QVariant)> _signal;
  properties::connection_t _connection;
  QComboBox* _edit { nullptr };
  QLabel* _title_widget { nullptr };
  QLabel* _unit_widget { nullptr };
  properties::enum_t _enum_values;

  auto supports(types::value_t t) -> bool
  {
    static std::set<types::value_t> const supported {
      types::value_t::BITFIELD, types::value_t::UINT8,  types::value_t::UINT16,
      types::value_t::UINT32,   types::value_t::UINT64,
    };
    return supported.find(t) != supported.end();
  }
};

combo_box::combo_box(boost::uuids::uuid identifier)
  : leaf_node { "combo_box", nullptr, identifier }
  , _p { std::make_unique<Impl>() }
{
  connect(_p->_edit, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
          [this](int index) {
            auto it = std::find_if(_p->_enum_values._values.begin(), _p->_enum_values._values.end(),
                                   [text = _p->_edit->itemText(index)](auto const& e) {
                                     return std::get<0>(e.second) == text;
                                   });

            if (it != _p->_enum_values._values.end()) {
              _p->_signal(QVariant::fromValue(it->first));
            }
          });
}

void combo_box::init()
{
  gui_node::init();
  addWidget(_p->_title_widget);
  addWidget(_p->_edit);
  addWidget(_p->_unit_widget);
  _p->_title_widget->hide();
  _p->_unit_widget->hide();
}

combo_box::~combo_box() = default;
auto combo_box::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto combo_box::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto combo_box::supportsValueType(types::value_t valueType) const -> bool
{
  return _p->supports(valueType);
}

auto combo_box::title() const -> yoyo::properties::connected_string_t
{
  return _p->_title;
}

auto combo_box::setTitle(yoyo::properties::connected_string_t const& t) -> void
{
  if (_p->_title == t) {
    return;
  }

  _p->_title = t;
  _p->_title_widget->setText(_p->_title.value());

  if (_p->_title.value().isEmpty()) {
    _p->_title_widget->hide();
  } else {
    _p->_title_widget->show();
  }

  Q_EMIT titleChanged(_p->_title);
}

auto combo_box::unit() const -> yoyo::properties::connected_string_t
{
  return _p->_unit;
}

auto combo_box::setUnit(yoyo::properties::connected_string_t const& t) -> void
{
  if (_p->_unit == t) {
    return;
  }

  _p->_unit = t;
  _p->_unit_widget->setText(_p->_unit.value());

  if (_p->_unit.value().isEmpty()) {
    _p->_unit_widget->hide();
  } else {
    _p->_unit_widget->show();
  }

  Q_EMIT unitChanged(_p->_unit);
}

auto combo_box::connection() const -> properties::connection_t
{
  return _p->_connection;
}

auto combo_box::setConnection(properties::connection_t s) -> void
{
  if (_p->_connection == s) {
    return;
  }

  _p->_connection.assign(s);
  Q_EMIT connectionChanged(_p->_connection);
}

auto combo_box::enumValues() const -> properties::enum_t
{
  return _p->_enum_values;
}

auto combo_box::setEnumValues(properties::enum_t const& values) -> void
{
  if (_p->_enum_values == values) {
    return;
  }

  _p->_enum_values = values;
  _p->_edit->clear();

  for (auto const& v : _p->_enum_values._values) {
    _p->_edit->addItem(std::get<0>(v.second));
  }

  _p->_edit->setCurrentIndex(-1);

  Q_EMIT enumValuesChanged(_p->_enum_values);
}
} // namespace yoyo
