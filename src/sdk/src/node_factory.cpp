#include "yoyo/node_factory.h"

namespace yoyo
{
class node_factory::impl
{
public:
  impl(QMetaObject const* c)
    : _category { c }
  {
  }
  auto createNode(boost::uuids::uuid type_id, boost::uuids::uuid node_id) const
    -> std::shared_ptr<node_base>;
  auto node_documentation(boost::uuids::uuid type_id) const -> std::shared_ptr<documentation> const;
  auto child_list(boost::uuids::uuid type_id, node_id_list const& list) -> node_id_list;
  auto install_node(boost::uuids::uuid type_id, QString type_name, QMetaObject const* meta_object,
                    std::shared_ptr<documentation> doc, node_creator creation_function,
                    potential_child_list child_list_accessor) -> bool;
  auto uninstall_node(boost::uuids::uuid type_id) -> bool;
  auto installed_nodes() -> std::vector<node_factory::node_info>;

  struct node_info {
    QString _name;
    std::shared_ptr<documentation> _documentation;
    node_creator _creation_function;
    potential_child_list _child_list;
  };
  std::map<boost::uuids::uuid, impl::node_info> _nodes;
  node_id_list _installed_nodes;
  QMetaObject const* const _category;
};

node_factory::node_factory(QMetaObject const* category)
  : _p { std::make_unique<impl>(category) }
{
}
node_factory::~node_factory() = default;
auto node_factory::createNode(boost::uuids::uuid type_id, boost::uuids::uuid node_id) const
  -> std::shared_ptr<node_base>
{
  return _p->createNode(std::move(type_id), std::move(node_id));
}

auto node_factory::impl::createNode(boost::uuids::uuid type_id, boost::uuids::uuid node_id) const
  -> std::shared_ptr<node_base>
{
  auto it = _nodes.find(type_id);

  if (it == std::end(_nodes)) {
    return {};
  }

  auto n = it->second._creation_function(node_id);
  n->init();
  return n;
}

auto node_factory::node_documentation(boost::uuids::uuid type_id) const
  -> std::shared_ptr<documentation> const
{
  return _p->node_documentation(std::move(type_id));
}

auto node_factory::impl::node_documentation(boost::uuids::uuid type_id) const
  -> std::shared_ptr<documentation> const
{
  auto it = _nodes.find(type_id);

  if (it == std::end(_nodes)) {
    return {};
  }

  return it->second._documentation;
}

auto node_factory::child_list(boost::uuids::uuid type_id, node_id_list const& list) -> node_id_list
{
  return _p->child_list(std::move(type_id), list);
}

auto node_factory::impl::child_list(boost::uuids::uuid type_id, node_id_list const& list)
  -> node_id_list
{
  auto it = _nodes.find(type_id);

  if (it == std::end(_nodes)) {
    return {};
  }

  if (list.empty()) {
    return it->second._child_list(installed_nodes());
  } else {
    return it->second._child_list(list);
  }
}

auto node_factory::install_node(boost::uuids::uuid type_id, QString type_name,
                                QMetaObject const* meta_object, std::shared_ptr<documentation> doc,
                                node_creator creation_function,
                                potential_child_list child_list_accessor) -> bool
{
  return _p->install_node(std::move(type_id), std::move(type_name), meta_object, std::move(doc),
                          std::move(creation_function), std::move(child_list_accessor));
}

auto node_factory::impl::install_node(boost::uuids::uuid type_id, QString type_name,
                                      QMetaObject const* meta_object,
                                      std::shared_ptr<documentation> doc,
                                      node_creator creation_function,
                                      potential_child_list child_list_accessor) -> bool
{
  auto it = _nodes.find(type_id);

  if (it != std::end(_nodes)) {
    return false;
  }

  if (type_id.is_nil() || type_name.isEmpty() || !doc) {
    return false;
  }

  auto node = creation_function({});

  if (!node || !node->metaObject()->inherits(_category)) {
    return false;
  }

  _installed_nodes.push_back({ type_id, meta_object, type_name });
  _nodes.insert(std::make_pair(std::move(type_id), node_info { std::move(type_name), std::move(doc),
                                                               std::move(creation_function),
                                                               std::move(child_list_accessor) }));
  return true;
}

auto node_factory::uninstall_node(boost::uuids::uuid type_id) -> bool
{
  return _p->uninstall_node(std::move(type_id));
}

auto node_factory::impl::uninstall_node(boost::uuids::uuid type_id) -> bool
{
  auto it = _nodes.find(type_id);
  auto it2 = std::find_if(std::begin(_installed_nodes), std::end(_installed_nodes),
                          [type_id](auto const& n) { return std::get<0>(n) == type_id; });

  if (it == std::end(_nodes)) {
    return false;
  }

  _nodes.erase(it);
  _installed_nodes.erase(it2);
  return true;
}

auto node_factory::installed_nodes() -> node_id_list
{
  return _p->installed_nodes();
}

auto node_factory::impl::installed_nodes() -> std::vector<node_factory::node_info>
{
  return _installed_nodes;
}
} // namespace yoyo
