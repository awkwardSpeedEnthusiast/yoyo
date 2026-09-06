#pragma once
#include "yoyo/documentation.h"
#include "yoyo/node_base.h"
#include "yoyo/yoyo_sdk_global.h"

#include <functional>

namespace yoyo
{
///
/// \brief A factory class for nodes.
///
/// This class provides factory functionality for nodes to relieve the nodes from the necessity to
/// create children themselves. The factory (or factories, one per object category) is passed to
/// a plugin at loading to enable the plugin to install nodes into the factory.
///
/// The factory not only provides creation of nodes, but also a documentation object for each node
/// type and a list of potential nodes to be added as child.
///
class YOYO_SDK_SHARED_EXPORT node_factory final
{
public:
  ///
  /// \brief Alias for information on a specific node
  ///
  /// The information contains the uuid with which the node is referenced, the Qt meta-object
  /// describing the node type (the static meta object) and a name to be displayed in the UI.
  ///
  using node_info = std::tuple<boost::uuids::uuid, QMetaObject const*, QString>;

  ///
  /// \brief Alias for a list of nodes
  ///
  using node_id_list = std::vector<node_info>;

  ///
  /// \brief Alias for creation function for nodes
  ///
  /// This creation function has to be provided with the install_node method and will be called,
  /// when the creation of a node of that type is requested. The id provided with the invocation
  /// is the identifier unique to the node, not the type id. This functor must return a shared_ptr
  /// to the new node.
  ///
  using node_creator = std::function<std::shared_ptr<node_base>(boost::uuids::uuid)>;

  ///
  /// \brief Alias for the retrieval of the list of potential children
  ///
  /// This functor has to be provided with the install_node method and will be called, when the
  /// list of potential children for one node-type is retrieved.
  /// The one parameter is the list of currently installed nodes.
  /// The return value shall only contain elements of the provided list and the elements of the
  /// returned list will be available for adding as children.
  /// Please verify, when creating new node-types, that this list is compatible with the
  /// implementation of the methods node_base::acceptsChild and node_base::acceptsParent.
  ///
  using potential_child_list = std::function<node_id_list(node_id_list const&)>;

public:
  ///
  /// \brief Constructor
  ///
  /// Creates a node factory for the provided node category.
  /// \param node_category the category of nodes to store in this factory.
  ///
  explicit node_factory(QMetaObject const* node_category);
  ~node_factory();
  node_factory(node_factory const& other) = delete;
  node_factory& operator=(node_factory const& other) = delete;
  node_factory(node_factory&& other) = delete;
  node_factory& operator=(node_factory&& other) = delete;

  ///
  /// \brief create node
  ///
  /// Creates a node of the mentioned type.
  /// \param type_id the id of the requested node type,
  /// \param node_id the identifier for the new node to create,
  /// \return a new node of the requested type or an empty shared-ptr, if the type is not known.
  ///
  auto createNode(boost::uuids::uuid type_id, boost::uuids::uuid node_id) const
    -> std::shared_ptr<node_base>;

  ///
  /// \brief node documentation
  ///
  /// Returns the documentation for a specific node type.
  /// \param type_id the id of the requested node type,
  /// \return the documentation of the requested node type, if that type is installed, an empty
  ///     shared-ptr else.
  ///
  auto node_documentation(boost::uuids::uuid type_id) const -> std::shared_ptr<documentation> const;

  ///
  /// \brief child list
  ///
  /// Returns the list of potential children for this node type.
  /// \see potential_child_list
  /// \param type_id the node type for which the list is requested,
  /// \param list the list of nodes to find possible children, if empty, the local list will be
  ///     used.
  /// \return the list of children which is currently allowed for the provided node type.
  ///
  auto child_list(boost::uuids::uuid type_id, node_id_list const& list = {}) -> node_id_list;

  ///
  /// \brief install node
  ///
  /// Installs a node into the factory. For this purpose not only a creation function is necessary
  /// but also a documentation object and a functor to calculate the list of allowed children from
  /// the list of installed nodes.
  /// Will invoke once the creation function to verify node category and reject the procedure if
  /// the node doesn't match the factories category.
  /// \param type_id the id of the node type,
  /// \param type_name the name of the node type as shown in the UI,
  /// \param meta_object the static meta object for the installed node type,
  /// \param doc the documentation of the node type,
  /// \param creation_function the function to be used to create a new node,
  /// \param child_list_accessor the function to calculate the list of potential child node types
  ///     from the list of installed nodes,
  /// \return true if the operation was successful. If the node type had been installed already,
  ///     this method will return false.
  ///
  auto install_node(boost::uuids::uuid type_id, QString type_name, QMetaObject const* meta_object,
                    std::shared_ptr<documentation> doc, node_creator creation_function,
                    potential_child_list child_list_accessor) -> bool;

  ///
  /// \brief uninstall node
  ///
  /// This method will uninstall the node of the specified type from the factory.
  /// \param type_id the id of the node type to uninstall,
  /// \return true if it could be uninstalled, false if the node type is unknown.
  ///
  auto uninstall_node(boost::uuids::uuid type_id) -> bool;

  ///
  /// \brief installed nodes
  /// \return a list of all installed nodes in this factory.
  ///
  auto installed_nodes() -> node_id_list;

private:
  class impl;
  std::unique_ptr<impl> _p;
};
} // namespace yoyo
