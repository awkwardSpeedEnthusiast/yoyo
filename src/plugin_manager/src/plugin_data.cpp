#include "plugin_data.h"

namespace yoyo::plugin
{
auto plugin_data::id() const -> boost::uuids::uuid
{
  return _id;
}

auto plugin_data::name() const -> QString
{
  return _name;
}

auto plugin_data::description() const -> QString
{
  return _description;
}

auto plugin_data::location() const -> boost::filesystem::path
{
  return _location;
}

auto plugin_data::description_location() const -> boost::filesystem::path
{
  return _description_location;
}

auto plugin_data::depends_on() const -> std::set<boost::uuids::uuid>
{
  return _depends_on;
}

plugin_data::plugin_data(boost::uuids::uuid id, QString name, QString description,
                         boost::filesystem::path location,
                         boost::filesystem::path description_location,
                         std::set<boost::uuids::uuid> depends_on)
  : _id { std::move(id) }
  , _name { std::move(name) }
  , _description { std::move(description) }
  , _location { std::move(location) }
  , _description_location { std::move(description_location) }
  , _depends_on { std::move(depends_on) }
{
}

plugin_data::builder::builder(boost::uuids::uuid id)
  : _id { std::move(id) }
{
}

auto plugin_data::builder::name(QString const& n) -> plugin_data::builder&
{
  _name = n;
  return *this;
}

auto plugin_data::builder::description(QString const& n) -> plugin_data::builder&
{
  _description = n;
  return *this;
}

auto plugin_data::builder::location(boost::filesystem::path const& p) -> plugin_data::builder&
{
  _location = p;
  return *this;
}

auto plugin_data::builder::description_location(boost::filesystem::path const& p)
  -> plugin_data::builder&
{
  _description_location = p;
  return *this;
}

auto plugin_data::builder::depend_on(boost::uuids::uuid const& id) -> plugin_data::builder&
{
  _depends_on.insert(id);
  return *this;
}

auto plugin_data::builder::build() -> std::shared_ptr<plugin_data>
{
  return std::shared_ptr<plugin_data> { new plugin_data(_id, _name, _description, _location,
                                                        _description_location, _depends_on) };
}
} // namespace yoyo::plugin
