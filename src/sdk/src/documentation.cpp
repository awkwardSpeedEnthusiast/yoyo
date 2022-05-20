#include "yoyo/documentation.h"

namespace yoyo
{
struct documentation::impl {
  impl(QString const& t, QString const& d, QString const& s, std::map<QString, property_t> const& p)
    : _type { t }
    , _description { d }
    , _styling { s }
    , _properties { p }
  {
  }
  QString const _type;
  QString const _description;
  QString const _styling;
  std::map<QString, property_t> const _properties;
};

documentation::documentation(QString const& type, QString const& description,
                             QString const& styling,
                             std::map<QString, property_t> const& properties)
  : _p { std::make_unique<impl>(type, description, styling, properties) }
{
}
documentation::~documentation() = default;

documentation::documentation(documentation const& other)
  : _p { std::make_unique<impl>(other._p->_type, other._p->_description, other._p->_styling,
                                other._p->_properties) }
{
}

auto documentation::type() const -> QString
{
  return _p->_type;
}
auto documentation::typeDescription() const -> QString
{
  return _p->_description;
}
auto documentation::property(QString const& propertyName) const -> property_t
{
  auto it = _p->_properties.find(propertyName);

  if (it != std::end(_p->_properties)) {
    return it->second;
  }

  return {};
}

auto documentation::styling() const -> QString
{
  return _p->_styling;
}

documentation::builder::builder(QString const& type)
  : _type(type)
{
}
auto documentation::builder::property(QString const& propertyName, QString const& title,
                                      QString const& description, QString const& tooltip,
                                      QVariant const& defaultValue) -> builder&
{
  _properties[propertyName] = { title, description, tooltip, defaultValue };
  return *this;
}

auto documentation::builder::description(QString const& value) -> builder&
{
  _description = value;
  return *this;
}

auto documentation::builder::styling(QString const& value) -> builder&
{
  _styling = value;
  return *this;
}

auto documentation::builder::build() -> std::shared_ptr<documentation>
{
  auto doc = std::shared_ptr<documentation>(
    new documentation { _type, _description, _styling, _properties });
  return doc;
}
} // namespace yoyo
