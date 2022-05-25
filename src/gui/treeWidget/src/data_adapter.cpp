#include "data_adapter.hpp"
#include "yoyo/communication_node.h"
#include "yoyo/data_node.h"
#include "yoyo/gui_node.h"
#include "yoyo/protocol_node.h"

#include "tree_utils.hpp"

#include <QDataStream>
#include <QMetaObject>
#include <QMimeData>
#include <QVariant>

#include <string>
using std::string_literals::operator""s;

namespace
{
const constexpr auto mimetype_data_element = "application/yoyo/data-element";
const constexpr auto mimetype_gui_element = "application/yoyo/gui-element";
const constexpr auto mimetype_protocol_element = "application/yoyo/protocol-element";
const constexpr auto mimetype_communication_element = "application/yoyo/communication-element";
} // namespace

namespace yoyo
{
namespace gui
{
data_adapter::data_adapter(node_base* node)
  : _node { node }
{
}

class invalid_data_adapter : public data_adapter
{
public:
  invalid_data_adapter(node_base* node)
    : data_adapter(node)
  {
  }

  auto data(int /*column*/) const -> QVariant override
  {
    return {};
  }
  auto mimedata() const -> QMimeData* override
  {
    return nullptr;
  }
  auto deserialize(QByteArray, std::shared_ptr<node_base>) const
    -> std::shared_ptr<node_base> override
  {
    return nullptr;
  }
};
class default_data_adapter : public data_adapter
{
public:
  default_data_adapter(node_base* node)
    : data_adapter(node)
  {
  }

  auto data(int column) const -> QVariant override
  {
    if (column == 0) {
      return _node->type();
    }

    return {};
  }

  auto mimedata() const -> QMimeData* override
  {
    return nullptr;
  }
  auto deserialize(QByteArray data, std::shared_ptr<node_base> root) const
    -> std::shared_ptr<node_base> override
  {
    QDataStream out(&data, QIODevice::ReadOnly);
    QString path;
    out >> path;

    if (_node->metaObject()->className() == "yoyo::data_root"s) {
      return utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(path, root);
    }

    if (_node->metaObject()->className() == "yoyo::gui_root"s) {
      return utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(path, root);
    }

    return nullptr;
  }
};

class data_data_adapter : public data_adapter
{
public:
  data_data_adapter(node_base* node)
    : data_adapter(node)
  {
  }

  auto data(int column) const -> QVariant override
  {
    switch (column) {
    case 0: return _node->name();
    case 1: return _node->type();
    case 2: return _node->property("title");
    }

    return {};
  }

  auto mimedata() const -> QMimeData* override
  {
    auto data = new QMimeData;
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << utilities::calculatePath<utilities::path_strategy_t::INDEX>(_node->shared_from_this());
    data->setData(mimetype_data_element, ba);
    return data;
  }

  auto deserialize(QByteArray data, std::shared_ptr<node_base> root) const
    -> std::shared_ptr<node_base> override
  {
    QDataStream out(&data, QIODevice::ReadOnly);
    QString path;
    out >> path;
    return utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(path, root);
  }
};

class gui_data_adapter : public data_adapter
{
public:
  gui_data_adapter(node_base* node)
    : data_adapter(node)
  {
  }

  auto data(int column) const -> QVariant override
  {
    switch (column) {
    case 0: return _node->type();
    case 1: return _node->property("title");
    case 2: return _node->property("title");
    }

    return {};
  }

  auto mimedata() const -> QMimeData* override
  {
    auto data = new QMimeData;
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << utilities::calculatePath<utilities::path_strategy_t::INDEX>(_node->shared_from_this());
    data->setData(mimetype_gui_element, ba);
    return data;
  }

  auto deserialize(QByteArray data, std::shared_ptr<node_base> root) const
    -> std::shared_ptr<node_base> override
  {
    QDataStream out(&data, QIODevice::ReadOnly);
    QString path;
    out >> path;
    return utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(path, root);
  }
};

class protocol_data_adapter : public data_adapter
{
public:
  protocol_data_adapter(node_base* node)
    : data_adapter(node)
  {
  }

  auto data(int column) const -> QVariant override
  {
    switch (column) {
    case 0: return _node->type();
    case 1: return _node->property("target");
    }

    return {};
  }

  auto mimedata() const -> QMimeData* override
  {
    auto data = new QMimeData;
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << utilities::calculatePath<utilities::path_strategy_t::INDEX>(_node->shared_from_this());
    data->setData(mimetype_protocol_element, ba);
    return data;
  }

  auto deserialize(QByteArray data, std::shared_ptr<node_base> root) const
    -> std::shared_ptr<node_base> override
  {
    QDataStream out(&data, QIODevice::ReadOnly);
    QString path;
    out >> path;
    return utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(path, root);
  }
};

class communication_data_adapter : public data_adapter
{
public:
  communication_data_adapter(node_base* node)
    : data_adapter(node)
  {
  }

  auto data(int column) const -> QVariant override
  {
    switch (column) {
    case 0: return _node->type();
    case 1: return _node->name();
    }

    return {};
  }

  auto mimedata() const -> QMimeData* override
  {
    auto data = new QMimeData;
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << utilities::calculatePath<utilities::path_strategy_t::INDEX>(_node->shared_from_this());
    data->setData(mimetype_communication_element, ba);
    return data;
  }

  auto deserialize(QByteArray data, std::shared_ptr<node_base> root) const
    -> std::shared_ptr<node_base> override
  {
    QDataStream out(&data, QIODevice::ReadOnly);
    QString path;
    out >> path;
    return utilities::retrieveFromPath<utilities::path_strategy_t::INDEX>(path, root);
  }
};

auto create_data_adapter(node_base* node) -> std::shared_ptr<data_adapter>
{
  if (!node) {
    return std::make_shared<invalid_data_adapter>(node);
  }

  if ((node->metaObject()->className() == "yoyo::data_root"s)
      || (node->metaObject()->className() == "yoyo::gui_root"s)) {
    return std::make_shared<default_data_adapter>(node);
  }

  if (node->metaObject()->inherits(&data_node::staticMetaObject)) {
    return std::make_shared<data_data_adapter>(node);
  }

  if (node->metaObject()->inherits(&gui_node::staticMetaObject)) {
    return std::make_shared<gui_data_adapter>(node);
  }

  if (node->metaObject()->inherits(&protocol_node::staticMetaObject)) {
    return std::make_shared<protocol_data_adapter>(node);
  }

  if (node->metaObject()->inherits(&communication_node::staticMetaObject)) {
    return std::make_shared<communication_data_adapter>(node);
  }

  return std::make_shared<invalid_data_adapter>(node);
}
} // namespace gui
} // namespace yoyo
