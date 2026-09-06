#include "node_deserializer_json.hpp"
#include "json_constants.hpp"
#include "json_property.hpp"

#include <yoyo/node_factory.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QMetaProperty>

#include <boost/uuid/string_generator.hpp>

namespace
{
auto set_properties(QJsonObject const& props, std::shared_ptr<yoyo::node_base> node) -> void
{
  auto* meta = node->metaObject();

  for (auto i = yoyo::node_base::staticMetaObject.propertyOffset(); i < meta->propertyCount();
       i++) {
    if (!props.contains(meta->property(i).name())) {
      continue;
    }

    auto v = props.value(meta->property(i).name());
    auto ioIt = yoyo::io::json::property::io.find(meta->property(i).typeName());

    if (ioIt == yoyo::io::json::property::io.end()) {
      continue;
    }

    auto read_value = ioIt->second.second(v);
    node->setProperty(meta->property(i).name(), read_value);
  }
}

auto extract_node(QJsonObject const& object,
                  std::vector<std::shared_ptr<yoyo::node_factory>> factories)
  -> std::shared_ptr<yoyo::node_base>
{
  if (!(object.contains(yoyo::io::json::key_node_type)
        && object.contains(yoyo::io::json::key_identifier)
        && object.contains(yoyo::io::json::key_properties)
        && object.contains(yoyo::io::json::key_children))) {
    return {};
  }

  if (!(object.value(yoyo::io::json::key_properties).isObject()
        && object.value(yoyo::io::json::key_children).isArray())) {
    return {};
  }

  auto nodeTypeString = object.value(yoyo::io::json::key_node_type).toString().toStdString();
  auto type_id = boost::uuids::string_generator {}(nodeTypeString);
  auto nodeIdString = object.value(yoyo::io::json::key_identifier).toString().toStdString();
  auto node_id = boost::uuids::string_generator {}(nodeIdString);

  if (type_id.is_nil() || node_id.is_nil()) {
    return {};
  }

  std::shared_ptr<yoyo::node_base> node;

  for (auto const& factory : factories) {
    auto n = factory->createNode(type_id, node_id);

    if (n) {
      node = n;
      break;
    }
  }

  if (!node) {
    return {};
  }

  set_properties(object.value(yoyo::io::json::key_properties).toObject(), node);
  auto children = object.value(yoyo::io::json::key_children).toArray();

  for (auto c : children) {
    if (!c.isObject()) {
      continue;
    }

    auto n = extract_node(c.toObject(), factories);

    if (n) {
      node->addChild(n);
    }
  }

  return node;
}
} // namespace
namespace yoyo::io
{
node_deserializer_json::~node_deserializer_json() = default;

auto node_deserializer_json::deserialize(std::istream& input) const -> std::shared_ptr<node_base>
{
  std::string file_type;
  std::getline(input, file_type);

  if (file_type != json::io_type_identifier) {
    return {};
  }

  std::string content(std::istreambuf_iterator<char>(input), {});

  auto document = QJsonDocument::fromJson(QByteArray::fromStdString(content));

  if (document.isNull() || document.isEmpty() || !document.isObject()) {
    return {};
  }

  auto root_node = document.object();
  return extract_node(root_node, _factories);
}
} // namespace yoyo::io
