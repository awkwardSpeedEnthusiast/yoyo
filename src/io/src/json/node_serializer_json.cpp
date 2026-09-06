#include "node_serializer_json.hpp"
#include "json_constants.hpp"
#include "json_property.hpp"

#include "yoyo/node_base.h"
#include "yoyo/node_factory.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QMetaProperty>
#include <QVariant>

#include <boost/uuid/uuid_io.hpp>

namespace
{
auto toObject(std::shared_ptr<yoyo::node_base> node,
              std::vector<std::shared_ptr<yoyo::node_factory>> factories) -> QJsonObject
{
  auto* meta = node->metaObject();
  QJsonObject properties;

  std::shared_ptr<yoyo::documentation> description;
  if (auto it = std::ranges::find_if(factories,
                                     [id = node->staticTypeId()](auto const& f) {
                                       return f->node_documentation(id) != nullptr;
                                     });
      it != factories.end()) {
    description = (*it)->node_documentation(node->staticTypeId());
  }

  for (auto i = yoyo::node_base::staticMetaObject.propertyOffset(); i < meta->propertyCount();
       i++) {
    if (!meta->property(i).isWritable()) {
      continue;
    }

    auto property = node->property(meta->property(i).name());

    if (description) {
      auto pd = description->property(meta->property(i).name());

      if (std::get<QVariant>(pd).isValid()
          && yoyo::value_equals(std::get<QVariant>(pd), property)) {
        continue;
      }
    }

    auto it = yoyo::io::json::property::io.find(property.typeName());

    if (it != yoyo::io::json::property::io.end()) {
      properties.insert(meta->property(i).name(), it->second.first(property));
    }
  }

  QJsonArray children;

  for (auto const& child : *node) {
    children.append(toObject(child, factories));
  }

  QJsonObject node_object;

  node_object.insert(yoyo::io::json::key_identifier,
                     QString::fromStdString(boost::uuids::to_string(node->identifier())));
  node_object.insert(yoyo::io::json::key_node_type,
                     QString::fromStdString(boost::uuids::to_string(node->staticTypeId())));
  node_object.insert(yoyo::io::json::key_properties, properties);
  node_object.insert(yoyo::io::json::key_children, children);

  return node_object;
}
} // namespace

namespace yoyo::io
{
auto node_serializer_json::serialize(std::ostream& output,
                                     std::shared_ptr<node_base> root_node) const -> void
{
  auto root_json = toObject(root_node, _factories);
  QJsonDocument document(root_json);

  output << json::io_type_identifier << "\n";
  output << document.toJson().toStdString();
}
} // namespace yoyo::io
