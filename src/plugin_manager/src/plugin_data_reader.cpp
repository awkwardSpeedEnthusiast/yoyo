#include "plugin_data_reader.hpp"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <boost/uuid/string_generator.hpp>

namespace
{
constexpr auto const key_id = "id";
constexpr auto const key_name = "name";
constexpr auto const key_description = "description";
constexpr auto const key_path = "path";
constexpr auto const key_dependencies = "dependencies";
} // namespace

namespace yoyo::plugin
{
std::shared_ptr<plugin_data> read(boost::filesystem::path const& path)
{
  try {
    if (!boost::filesystem::exists(path)) {
      return {};
    }

    QFile file(path.string().c_str());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      return {};
    }

    auto doc = QJsonDocument::fromJson(file.readAll());

    if (!doc.isObject()) {
      return {};
    }

    auto object = doc.object();

    if (!object.contains(key_id)) {
      return {};
    }

    auto id = object.value(key_id).toString();
    plugin_data::builder builder(boost::uuids::string_generator {}(id.toStdString()));
    builder.description_location(path);

    if (object.contains(key_name)) {
      builder.name(object.value(key_name).toString());
    }

    if (object.contains(key_description)) {
      builder.description(object.value(key_description).toString());
    }

    if (object.contains(key_path)) {
      boost::filesystem::path p = object.value(key_path).toString().toStdString();

      if (p.is_relative() && boost::filesystem::exists(path.parent_path() / p)) {
        p = path.parent_path() / p;
      }

      builder.location(p);
    }

    if (object.contains(key_dependencies) && object.value(key_dependencies).isArray()) {
      auto dependencies = object.value(key_dependencies).toArray();

      for (auto d : dependencies) {
        builder.depend_on(boost::uuids::string_generator {}(d.toString().toStdString()));
      }
    }

    return builder.build();
  } catch (boost::filesystem::filesystem_error&) {
  }
  return {};
}
} // namespace yoyo::plugin
