#pragma once
#include "yoyo/node_base.h"
#include "yoyo/yoyo_sdk_global.h"

namespace yoyo
{
///
/// \brief Base class for all widget based nodes.
///
/// This is the base class for all nodes, which have a representation in the main display of the
/// application.
/// Note, that the gui node itself is no QWidget. The Has-A relationship was used instead of an Is-A
/// relationship. This helps avoid multiple inheritance and virtual inheritance, which is not really
/// supported with Qt. The actual QWidget object can be retrieved with the widget method, but the
/// type is not exposed to the SDK, so only QWidget methods are available. It should not be
/// necessary to access it directly, the node provides functionality to add the widgets of children
/// and to add other widgets as well.
///
/// The node provides the possibility to pass a widget to the constructor which is then used as a
/// container for all added widgets or child nodes. An example would be, if a QGroupBox or a
/// QRadioButtonGroup should be used to manage child widgets. The container is optional, it will
/// work as well, if nullptr is provided.
///
class YOYO_SDK_SHARED_EXPORT gui_node : public node_base
{
  Q_OBJECT

  ///
  /// \brief layout direction
  ///
  /// The layout direction specifies the underlying layout for the widget, which is represented by
  /// this node. If this property is not to be modified by the user during edit process, set the
  /// visibility flag to false.
  ///
  Q_PROPERTY(yoyo::properties::invisible_layout_direction_t layoutDirection READ layoutDirection
               WRITE setLayoutDirection NOTIFY layoutDirectionChanged)

  ///
  /// \brief x-position
  ///
  /// The x position of the widget inside its parent widget. This will only have effect, if a
  /// free layout is used.
  ///
  Q_PROPERTY(int xPosition READ xPosition WRITE setXposition NOTIFY xPositionChanged)

  ///
  /// \brief y-position
  ///
  /// The y position of the widget inside its parent widget. This will onyl have effect, if a
  /// free layout is used.
  ///
  Q_PROPERTY(int yPosition READ yPosition WRITE setYposition NOTIFY yPositionChanged)

  ///
  /// \brief width
  ///
  /// The width of the widget inside its parent widget. This will set the fixed width of the
  /// widget, so it won't be affected by the layouting if one is applied. Use a negative value
  /// to reset.
  ///
  Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)

  ///
  /// \brief height
  ///
  /// The height of the widget inside its parent widget. This will set the fixed height of the
  /// widget, so it won't be affected by the layouting if one is applied. Use a negative value
  /// to reset.
  ///
  Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)

  ///
  /// \brief enabled
  ///
  /// The enabled property: depending on the actual boolean value, the widget is marked as enabled
  /// or disabled.
  ///
  Q_PROPERTY(yoyo::properties::connected_boolean_t enabled READ enabled WRITE setEnabled NOTIFY
               enabledChanged)

  ///
  /// \brief visible
  ///
  /// Manages the visibility of the widget. Invisible widgets are ignored by layouts.
  ///
  Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)

  ///
  /// \brief style sheet
  ///
  /// This is the style sheet to be used by the widget.
  /// \see the Qt documentation on style sheets: https://doc.qt.io/qt-5/stylesheet.html
  ///
  Q_PROPERTY(yoyo::properties::text_t styleSheet READ styleSheet WRITE setStyleSheet NOTIFY
               styleSheetChanged)

public:
  ///
  /// \brief Constructor
  /// \param type_name the name of the type to be displayed for editing,
  /// \param container a possible widget to be used as a container for all child widgets, can be
  ///     left empty (nullptr),
  /// \param id the unique identifier for the individual node.
  gui_node(QString type_name, QWidget* container, boost::uuids::uuid id = {});
  ~gui_node() override;
  gui_node(gui_node const& other) = delete;
  gui_node& operator=(gui_node const& other) = delete;
  gui_node(gui_node&& other) = delete;
  gui_node& operator=(gui_node&& other) = delete;

  ///
  /// \brief init
  ///
  /// This method is called by the factory directly after creation, before it is passed to any
  /// other object to initialize internals, which need the memory management to be initialized
  /// already (if shared_from_this is to be called). When overriding, please make sure, that you
  /// call this implementation here.
  ///
  auto init() -> void override;

  ///
  /// \brief widget
  ///
  /// Returns a pointer to the widget, which is to be used in the main display. This method is
  /// necessary to pass the widget to the main display, but it is not meant to be used manage the
  /// children introduced by derived classes. Use the protected methods addWidget and removeWidget
  /// for that purpose.
  /// \return the widget represented by this gui node.
  ///
  auto widget() -> QWidget*;

  ///
  /// \{
  /// \name Properties
  ///

  ///
  /// \brief layout direction
  /// \return the layout direction property
  ///
  auto layoutDirection() const -> yoyo::properties::invisible_layout_direction_t;

  ///
  /// \brief set layout direction
  ///
  /// This is the setter for the layout direction property. It will emit a layoutDirectionChanged
  /// signal on actual changes.
  /// \param ld the new layout direction value.
  ///
  auto setLayoutDirection(yoyo::properties::invisible_layout_direction_t const& ld) -> void;

  ///
  /// \brief x-position
  /// \return the x position of the widget inside its parent widget.
  ///
  auto xPosition() const -> int;

  ///
  /// \brief set x-position
  ///
  /// This is the setter for the x position of the widget inside its parent widget. It will emit
  /// an xPositionChanged signal on actual changes.
  /// \param x the new x position of the widget.
  ///
  auto setXposition(int x) -> void;

  ///
  /// \brief y-position
  /// \return the y position of the widget inside its parent widget.
  ///
  auto yPosition() const -> int;

  ///
  /// \brief set y-position
  ///
  /// This is the setter for the y position of the widget inside its parent widget. It will emit
  /// a yPositionChanged signal on actual changes.
  /// \param y the new y position of the widget.
  ///
  auto setYposition(int y) -> void;

  ///
  /// \brief width
  /// \return the width of the widget inside its parent widget.
  ///
  auto width() const -> int;

  ///
  /// \brief set width
  ///
  /// This is the setter for the width of the widget inside its parent widget. It will emit
  /// a widthChanged signal on actual changes.
  /// \param w the new width of the widget.
  ///
  auto setWidth(int w) -> void;

  ///
  /// \brief height
  /// \return the height of the widget inside its parent widget.
  ///
  auto height() const -> int;

  ///
  /// \brief set height
  ///
  /// This is the setter for the heigh of the widget inside its parent widget. It will emit
  /// a heightChanged signal on actual changes.
  /// \param h the new height of the widget.
  ///
  auto setHeight(int h) -> void;

  ///
  /// \brief enabled
  /// \return true if the widget is in enabled state.
  ///
  auto enabled() const -> yoyo::properties::connected_boolean_t;

  ///
  /// \brief set enabled
  ///
  /// This is the setter for the enabled property of the widget. It will emit the enabledChanged
  /// signal on change.
  /// \param e the new enabled value.
  ///
  auto setEnabled(yoyo::properties::connected_boolean_t e) -> void;

  ///
  /// \brief visible
  /// \return true if the widget is visible.
  ///
  auto visible() const -> bool;

  ///
  /// \brief set visible
  ///
  /// Sets the visibility of the widget. It will emit the visibilityChanged signal on change.
  /// \param v the new visibility.
  ///
  auto setVisible(bool v) -> void;

  ///
  /// \brief style sheet
  /// \return the style sheet of the widget and its children.
  ///
  auto styleSheet() const -> yoyo::properties::text_t;

  ///
  /// \brief set style sheet
  ///
  /// This is the setter for the style sheet property. It will emit the styleSheetChanged signal
  /// on change.
  /// Please consult the Qt documentation (https://doc.qt.io/qt-5/stylesheet.html) on this topic
  /// for further information.
  /// \param sheet the new style sheet.
  ///
  auto setStyleSheet(yoyo::properties::text_t const& sheet) -> void;

  ///
  /// \}
  ///

  ///
  /// \brief enables edit mode for node
  ///
  /// Use this method to enable some additional features on the gui node, like drag&drop of the
  /// nodes in the scene. This information is passed on along the tree, so it is only necessary to
  /// do on the root node.
  /// \param mode true if the edit features are to be enabled.
  ///
  auto setEditMode(bool mode) -> void;

  ///
  /// \brief does the node support a specific value-type
  ///
  /// Returns whether the node supports a specific value type. This is used to determine whether a
  /// connection may be made between this gui node and a data node of a certain value type.
  /// \param valueType the type in question,
  /// \return true if the value type is supported by this node.
  ///
  virtual auto supportsValueType(types::value_t valueType) const -> bool = 0;
Q_SIGNALS:
  ///
  /// \{
  /// \name Property signals
  ///

  ///
  /// \brief layout direction changed signal
  ///
  /// This signal is emitted, when the layout direction property changed.
  /// \param ld the new layout direction.
  ///
  void layoutDirectionChanged(yoyo::properties::invisible_layout_direction_t ld);

  ///
  /// \brief style sheet changed signal
  ///
  /// This signal is emitted, when the style sheet property of this node changed. It doesn't take
  /// into account, if changed further up the hierarchy, which could have effects on the apperance
  /// of this node's widget.
  /// \param sheet the new style sheet.
  ///
  void styleSheetChanged(yoyo::properties::text_t sheet);

  ///
  /// \brief x-position changed signal
  ///
  /// This signal is emitted, when the x position property of the node changed. It doesn't take
  /// into account changes, which are due to layouting inside the parent widget.
  /// \param x the new x position value.
  ///
  void xPositionChanged(int x);

  ///
  /// \brief y-position changed signal
  ///
  /// This signal is emitted, when the y position property of the node changed. It doesn't take
  /// into account changes, which are due to layouting inside the parent widget.
  /// \param y the new y position value.
  ///
  void yPositionChanged(int y);

  ///
  /// \brief width changed signal
  ///
  /// This signal is emitted, when the width property of the node changed. It doesn't take
  /// into account changes, which are due to layouting inside the parent widget.
  /// \param w the new width of the widget.
  ///
  void widthChanged(int w);

  ///
  /// \brief height changed signal
  ///
  /// This signal is emitted, when the height property of the node changed. It doesn't take
  /// into account changes, which are due to layouting inside the parent widget.
  /// \param h the new height of the widget.
  ///
  void heightChanged(int h);

  ///
  /// \brief enabled changed signal
  ///
  /// This signal is emitted, when the enabled property of the node changed.
  /// \param e the new enablement value.
  ///
  void enabledChanged(yoyo::properties::connected_boolean_t e);

  ///
  /// \brief visible changed signal
  ///
  /// This signal is emitted, when the visibility property of the node changed. Note, that the
  /// node's widget can be invisible, even if this property is true, when a node up the hierarchy
  /// has visibility false.
  /// \param v the new visibility of this node.
  ///
  void visibleChanged(bool v);

  ///
  /// \}
  ///

  ///
  /// \brief item selected signal
  ///
  /// This signal is emitted, if the widget has been clicked. The signal is propagated up along
  /// the hierarchy.
  /// \param selected the node which has been clicked.
  ///
  void itemSelected(std::shared_ptr<node_base> selected);

  ///
  /// \brief default for data requested
  ///
  /// This signal is emitted, when a data element has been dropped on this node's widget. Since
  /// the node doesn't have access to a factory, the request is passed upstream along the
  /// hierarchy to a point, where the it can be processed.
  /// \param path the id-path to the data node, which had been dropped,
  /// \param parent the gui node, which should act as parent for the new node,
  /// \param index the index to add the new node at,
  /// \param connectDefaultInput true if the new node shall connect the input,
  /// \param connectDefaultOutput true if the new node shall connect the output,
  /// \param connectDefaultAutoConnect true if the new node shall connect automatically.
  ///
  void defaultForDataRequested(QString path, std::shared_ptr<node_base> parent, int index,
                               bool connectDefaultInput, bool connectDefaultOutput,
                               bool connectDefaultAutoConnect);

protected:
  ///
  /// \brief add widget
  ///
  /// Add a widget to this node's widget as a child.
  /// Please use this method to add widgets and create new widgets with no parent (nullptr)
  /// instead. This allows to manage the order of the widgets and possible layouting.
  /// Do not use this method for adding widgets of child nodes; this is done already automatically.
  /// \param widget the widget to add to this node's widget,
  /// \param index the index, where to add it (-1 adds it at the end).
  ///
  auto addWidget(QWidget* widget, int index = -1) -> void;

  ///
  /// \brief remove widget
  ///
  /// Remove a widget from this node's widget.
  /// Don't use this method for cleanup purposes at destruction of the node (Qt takes care of
  /// that), but to dynamically modify the widget's appearance.
  /// Do not use this method for removing widgets of child nodes; this is done already
  /// automatically.
  /// \param widget the widget to remove.
  ///
  auto removeWidget(QWidget* widget) -> void;

  ///
  /// \brief container
  ///
  /// This method returns the container widget, if one is defined for this widget type.
  /// \return the container widget if defined
  ///
  auto container() const -> QWidget*;

private:
  ///
  /// \brief before child add
  ///
  /// Managing widgets of child nodes is implemented already in the overridden methods from the
  /// base class node_base. Please use this method to add any functionality necessary directly
  /// before adding a child node.
  /// \param child the child which is to be added to this node,
  /// \return true if operation is successful.
  /// \see node_base::childAboutToBeAdded
  ///
  virtual auto beforeChildAdd(std::shared_ptr<node_base> const& child) -> bool = 0;
  ///
  /// \brief after child add
  ///
  /// Managing widgets of child nodes is implemented already in the overridden methods from the
  /// base class node_base. Please use this method to add any functionality necessary directly
  /// after adding a child node.
  /// \param child the child which has been added to this node,
  /// \return true if operation is successful.
  /// \see node_base::childAdded
  ///
  virtual auto afterChildAdd(std::shared_ptr<node_base> const& child) -> bool = 0;
  ///
  /// \brief before child remove
  ///
  /// Managing widgets of child nodes is implemented already in the overridden methods from the
  /// base class node_base. Please use this method to add any functionality necessary directly
  /// before removing a child node.
  /// \param child the child which has to be removed from this node,
  /// \return true if operation is successful.
  /// \see node_base::childAboutToBeRemoved
  ///
  virtual auto beforeChildRemove(std::shared_ptr<node_base> const& child) -> bool = 0;
  ///
  /// \brief after child remove
  ///
  /// Managing widgets of child nodes is implemented already in the overridden methods from the
  /// base class node_base. Please use this method to add any functionality necessary directly
  /// after removing a child node.
  /// \param child the child which has been removed from this node,
  /// \return true if operation is successful.
  /// \see node_base::childRemoved
  ///
  virtual auto afterChildRemove(std::shared_ptr<node_base> const& child) -> bool = 0;

  ///
  /// \brief react on child move
  ///
  /// This method is called when a child has been moved in the internal list of children. It is
  /// meat for derived classes to react on child move operations.
  /// Being purely virtual, this method has to be implemented by derived classes.
  /// \param child the moved child item,
  /// \param from_index the index, the child had previously,
  /// \param to_index the current index of the child.
  ///
  virtual auto afterChildMoved(std::shared_ptr<node_base> const& child, size_t from_index,
                               size_t to_index) -> void = 0;

  auto childAboutToBeAdded(std::shared_ptr<node_base> const& child) -> bool override final;
  auto childAdded(std::shared_ptr<node_base> const& child) -> bool override final;
  auto childAboutToBeRemoved(std::shared_ptr<node_base> const& child) -> bool override final;
  auto childRemoved(std::shared_ptr<node_base> const& child) -> bool override final;
  auto childMoved(std::shared_ptr<node_base> const& child, size_t from_index, size_t to_index)
    -> void final;

private:
  struct impl;
  std::unique_ptr<impl> _p;
};
} // namespace yoyo
