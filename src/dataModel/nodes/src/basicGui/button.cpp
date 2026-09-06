#include "button.hpp"

#include <QPushButton>

#include <boost/uuid/string_generator.hpp>

namespace yoyo
{
struct button::Impl {
  QString _title;
  properties::script_t _script;
  QPushButton* _button { nullptr };
};
boost::uuids::uuid const button::_typeId =
  boost::uuids::string_generator {}("ed464bef-321c-424a-8acd-5dfba4d345b4");

button::button(boost::uuids::uuid identifier)
  : leaf_node { "button", nullptr, identifier }
  , _p { std::make_unique<Impl>() }
{
  _p->_button = new QPushButton;
  _p->_button->setObjectName("__pushButton");
  connect(_p->_button, &QPushButton::clicked, this, [this]() {
    if (!_p->_script._text.isEmpty()) {
      Q_EMIT scriptExecutionRequested(_p->_script);
    }
  });
}

void button::init()
{
  gui_node::init();
  addWidget(_p->_button);
}

button::~button() = default;

auto button::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto button::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto button::supportsValueType(types::value_t /*valueType*/) const -> bool
{
  return false;
}

auto button::title() const -> QString
{
  return _p->_title;
}

auto button::setTitle(QString const& t) -> void
{
  if (_p->_title == t) {
    return;
  }

  _p->_title = t;
  _p->_button->setText(_p->_title);
  Q_EMIT titleChanged(_p->_title);
}

auto button::onClick() const -> properties::script_t
{
  return _p->_script;
}

auto button::setOnClick(properties::script_t s) -> void
{
  if (_p->_script == s) {
    return;
  }

  _p->_script = s;
  Q_EMIT onClickChanged(_p->_script);
}
} // namespace yoyo
