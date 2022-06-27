#pragma once
#include "yoyo/property_types.h"
#include "yoyo/yoyo_sdk_global.h"

#include <QObject>
#include <QString>

#include <boost/uuid/uuid.hpp>

#include <memory>
#include <vector>

namespace yoyo
{
///
/// \brief Base class of all nodes.
///
/// This class is the base for nodes in the configuration tree. It handles the
/// parent-child relationship inside the tree.
///
class YOYO_SDK_SHARED_EXPORT node_base : public QObject,
                                         public std::enable_shared_from_this<node_base>
{
  Q_OBJECT

  ///
  /// \brief name property
  ///
  /// This is the name of the object, which is to be available for Gui access.
  Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

  ///
  /// \brief type property
  ///
  /// This is the read-only type property, which is to be available for Gui
  /// access.
  Q_PROPERTY(QString type READ type)
public:
  ///
  /// Convenience alias to iterator over children
  ///
  using iterator = std::vector<std::shared_ptr<node_base>>::iterator;
  ///
  /// Convenience alias to const iterator over children
  ///
  using const_iterator = std::vector<std::shared_ptr<node_base>>::const_iterator;
  ///
  /// \brief describes the operation performed on the node
  ///
  /// This enum is used in the treeChanged signal to specify which operation
  /// had been performed on the changed node .
  ///
  enum class ChangeOperation { PRE_ADD, ADDED, PRE_REMOVE, REMOVED, PRE_MOVE, MOVED };

public:
  ///
  /// \brief Constructor
  /// \param type the node type to be displayed in gui
  /// \param identifier the id for the node to create.
  ///
  explicit node_base(QString type, boost::uuids::uuid identifier = boost::uuids::uuid {});
  virtual ~node_base();
  node_base(node_base const& other) = delete;
  node_base& operator=(node_base const& other) = delete;
  node_base(node_base&& other) = delete;
  node_base& operator=(node_base&& other) = delete;

  ///
  /// \brief init
  ///
  /// Some of the derived classes will need some initialization past the
  /// constructor before they can be fully used. Please make sure that by
  /// deriving from one of the child classes of this class, the parent class'
  /// init method is invoked before your changes.
  ///
  virtual auto init() -> void {}

  ///
  /// \brief The identifier of the type
  ///
  /// This method has to be overridden by *every* derived class individually
  /// to return the identifier of the type. This method is used to determine
  /// the documentation object to retrieve for display and other
  /// functionalities. \return the type-id of this type.
  ///
  virtual auto staticTypeId() const -> boost::uuids::uuid = 0;

  /// \{
  /// \name Properties

  ///
  /// \brief name
  ///
  /// This is the getter for the name property.
  /// \return the current value for name.
  ///
  auto name() const -> QString;
  ///
  /// \brief set name
  ///
  /// This is the setter for the name property. Emits the nameChanged signal.
  /// \param value the new name.
  ///
  auto setName(QString const& value) -> void;
  ///
  /// \brief type
  ///
  /// This is the getter for the readonly type property.
  /// \return the value for the type.
  ///
  auto type() const -> QString;
  ///
  /// \brief identifier
  ///
  /// This is the unique identifier of the node object.
  /// \return the object's unique identifier.
  ///
  auto identifier() const -> boost::uuids::uuid;

  /// \}

  /// \{
  /// \name Tree structure

  /// \brief number of children
  ///
  /// The number of direct children of this node.
  /// \return number of children.
  ///
  auto childCount() const -> size_t;
  ///
  /// \brief child at
  ///
  /// Use this method to access the individual children of this node. Please
  /// verify, that the index you provide is not out of scope. \param index the
  /// index of the child to return, \return a specific child of the node.
  ///
  auto childAt(size_t index) const -> std::shared_ptr<node_base>;
  ///
  /// \brief child index
  ///
  /// Calculates and returns the index of the provided node. If the node is
  /// not a child of this node, it will return -1; \param child the node to
  /// calculate the index for, \return the index of the child node or -1 if it
  /// is not a child of this node.
  ///
  auto childIndex(std::shared_ptr<node_base> const& child) const -> int;
  ///
  /// \brief add child
  ///
  /// This method adds a child to the node at the provided index. If the
  /// provided child is already a child of the node, this node or the nullptr,
  /// this method will do nothing. In case the child to add is already a child
  /// of an other node, it will be reparented, calling childAboutToBeRemoved
  /// and childRemoved on the old parent.
  /// The child will be added at the provided index or at the end of the child
  /// list, if the provided index is -1 or equal or larger than the size of
  /// the list. Calls childAboutToBeAdded directly before and childAdded
  /// directly after the child has been added. Emits the signal treeChanged.
  /// \param child the child-node to add,
  /// \param index the index of the child after adding.
  ///
  auto addChild(std::shared_ptr<node_base> const& child, int index = -1) -> void;
  ///
  /// \brief remove child
  ///
  /// This method removes a child from the node. If the child to remove is
  /// either nullptr or not a child of this node, the method returns
  /// immediately. Calls childAboutToBeRemoved directly before and
  /// childRemoved directly after the child has been removed. Emits the signal
  /// treeChanged. \param child the child to be removed.
  ///
  auto removeChild(std::shared_ptr<node_base> const& child) -> void;
  ///
  /// \brief remove child
  ///
  /// This method removes a child from the node. If the provided index is
  /// equal or larger than the number of children in this node, the method
  /// returns directly. Calls childAboutToBeRemoved directly before and
  /// childRemoved directly after the child has been removed. Emits the signal
  /// treeChanged. \param index the index of the child to remove.
  ///
  auto removeChild(size_t index) -> void;
  ///
  /// \brief move child
  ///
  /// This method moves one child inside the internal child list from one
  /// position to a new one. If the provided child is either nullptr or not a
  /// child of this node, the method returns directly. Emits the signal
  /// treeChanged. \param child the child to move, \param targetIndex the
  /// index to where the child should be moved.
  ///
  auto moveChild(std::shared_ptr<node_base> const& child, size_t targetIndex) -> void;
  ///
  /// \brief parent
  ///
  /// This method is a getter for the parent node of this node.
  /// \return the current parent of the node.
  ///
  auto parent() const -> std::weak_ptr<node_base>;

  ///
  /// \brief accepts children
  ///
  /// This method returns true, if the implementation of the node-class is
  /// meant to be an inner node of the tree structure, if it can have
  /// children.
  ///
  /// Being purely virtual, this method has to be implemented by derived
  /// classes. \return true if children can be added to this node.
  ///
  virtual auto acceptsChildren() const -> bool = 0;
  ///
  /// \brief accepts particular child
  ///
  /// This method returns true, if the provided child can be added to this
  /// node as child. For some inner nodes, only specific types of children are
  /// allowed. This is the method, that checks this.
  ///
  /// Being purely virtual, this method has to be implemented by derived
  /// classes. \param child the node for which an add is requested, \return
  /// true, if the child may be added to this node.
  ///
  virtual auto acceptsChild(std::shared_ptr<node_base> const& child) const -> bool = 0;
  ///
  /// \brief accepts particular parent
  ///
  /// This method checks, whether this node can be added to a potential
  /// parent. This makes sense for nodes, which are only allowed for a
  /// particular class of parents.
  ///
  /// Being purely virtual, this method has to be implemented by derived
  /// classes. \param parent the parent to check, \return true, if this node
  /// is allowed as child of the provided parent.
  ///
  virtual auto acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool = 0;

  /// \}

  ///
  /// \brief iterator to the first child in the list of children
  ///
  /// This convenience method provides an iterator to the first child in the
  /// list of children to enable ranges behavior. \return iterator to begin of
  /// child list
  ///
  auto begin() -> iterator;
  ///
  /// \brief iterator to the first child in the list of children
  ///
  /// This convenience method provides a const iterator to the first child in
  /// the list of children to enable ranges behavior. \return const iterator
  /// to begin of child list
  ///
  auto begin() const -> const_iterator;
  ///
  /// \brief iterator beyond the last child in the list of children
  ///
  /// This convenience method provides an iterator beyond the child in the
  /// list of children to enable ranges behavior. \return iterator beyond the
  /// end of child list
  ///
  auto end() -> iterator;
  ///
  /// \brief iterator beyond the last child in the list of children
  ///
  /// This convenience method provides a const iterator beyond the child in
  /// the list of children to enable ranges behavior. \return const iterator
  /// beyond the end of child list
  ///
  auto end() const -> const_iterator;
Q_SIGNALS:
  ///
  /// \brief name changed signal
  ///
  /// This signal is emitted, when the name of the node has changed.
  /// \param name the new name of the node.
  ///
  void nameChanged(QString const& name);
  ///
  /// \brief tree changed signal
  ///
  /// This signal is emitted, when there have been changes in the tree below
  /// this node. The signal is propagated along the tree to the root node.
  /// \param changedNode the parent node, where changes had been made,
  /// \param object the object, which had been moved,
  /// \param index_before_change the index of the object inside the changedNode before the
  ///         structural change. This will be -1 for add. In case of pre-move, this will be the
  ///         target index instead of the actual index before change to have complete information
  ///         about the operation.
  /// \param operationType the type of change, which had been done.
  ///
  void treeChanged(std::weak_ptr<node_base> changedNode, std::weak_ptr<node_base> object,
                   int index_before_change, ChangeOperation operationType);
  ///
  /// \brief exchange requested signal
  ///
  /// This signal is emitted, when in the hierarchy a request for exchanging a
  /// node with a node of an other type. The signal is propagated along the
  /// tree to the root node, since the nodes usually do not have access to the
  /// node factories. \param nodeToExchange the node to exchange, \param
  /// nodeType the type of the node to exchange with.
  ///
  void exchangeRequested(std::shared_ptr<node_base> nodeToExchange, boost::uuids::uuid nodeType);
  ///
  /// \brief add requested signal
  ///
  /// This signal is emitted, when in the hierarchy a request for adding a
  /// node is generated. This signal is propagated along the tree to the root
  /// node, since the nodes usually do not have access to the node factories.
  /// If the index is smaller than zero or larger than the number of children,
  /// the new node will be added at the back. \param parentNode the node to
  /// add a child to, \param nodeType the node type to add, \param index the
  /// target index of the new node.
  ///
  void addRequested(std::shared_ptr<node_base> parentNode, boost::uuids::uuid nodeType, int index);
  ///
  /// \brief request the execution of a script
  ///
  /// This signal is emitted, if the execution of a script is requested. This
  /// signal is propagated along the tree to the root node, since the
  /// scripting engine(s) do not connect to each individual node. \param s the
  /// script to execute.
  ///
  void scriptExecutionRequested(yoyo::properties::script_t s);

private:
  /// \{
  /// \name Purely virtual tree structure

  ///
  /// \brief react on child about to be added
  ///
  /// This method is called directly before a child is added to the child
  /// list. It is meant for derived classes to react on child-adding
  /// operations.
  ///
  /// Being purely virtual, this method has to be implemented by derived
  /// classes. \param child the child which is about to be added to the node,
  /// \return true if the operation was successful.
  ///
  virtual auto childAboutToBeAdded(std::shared_ptr<node_base> const& child) -> bool = 0;
  ///
  /// \brief react on child added
  ///
  /// This method is called directly after a child has been added to the child
  /// list. It is meant for derived classes to react on child-adding
  /// operations.
  ///
  /// Being purely virtual, this method has to be implemented by derived
  /// classes. \param child the child which was added to the node, \return
  /// true if the operation was successful.
  ///
  virtual auto childAdded(std::shared_ptr<node_base> const& child) -> bool = 0;
  ///
  /// \brief react on child about to be removed
  ///
  /// This method is called directly before a child is removed from the child
  /// list. It is meant for derived classes to react on child-removing
  /// operations.
  ///
  /// Being purely virtual, this method has to be implemented by derived
  /// classes. \param child the child which is about to be removed, \return
  /// true if the operation was successful.
  ///
  virtual auto childAboutToBeRemoved(std::shared_ptr<node_base> const& child) -> bool = 0;
  ///
  /// \brief react on child removed
  ///
  /// This method is called directly after a child is removed from the child
  /// list. It is meant for derived classes to react on child-removing
  /// operations.
  ///
  /// Being purely virtual, this method has to be implemented by derived
  /// classes. \param child the child which has been removed, \return true if
  /// the operation was successful.
  ///
  virtual auto childRemoved(std::shared_ptr<node_base> const& child) -> bool = 0;

  ///
  /// \brief react on child move
  ///
  /// This method is called when a child has been moved in the internal list
  /// of children. It is meat for derived classes to react on child move
  /// operations. Being purely virtual, this method has to be implemented by
  /// derived classes. \param child the moved child item, \param from_index
  /// the index, the child had previously, \param to_index the current index
  /// of the child.
  ///
  virtual auto childMoved(std::shared_ptr<node_base> const& child, size_t from_index,
                          size_t to_index) -> void = 0;
  /// \}

private:
  struct impl;
  std::unique_ptr<impl> _p;
};
} // namespace yoyo
