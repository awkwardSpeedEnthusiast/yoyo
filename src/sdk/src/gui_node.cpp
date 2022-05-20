#include "yoyo/gui_node.h"
#include "gui_node_widget.hpp"

namespace yoyo
{
struct gui_node::impl {
  properties::invisible_layout_direction_t _layout_direction {
    properties::invisible_layout_direction_t { types::layout_direction_t::HORIZONTAL, false }
  };
  int _x_position { 0 };
  int _y_position { 0 };
  int _width { 0 };
  int _height { 0 };
  properties::connected_boolean_t _enabled { properties::connected_boolean_t::boolean_t::TRUE,
                                             true };
  bool _visible { true };
  properties::text_t _style_sheet;
  QWidget* _container;
  std::unique_ptr<gui_node_widget> _widget;
};

gui_node::gui_node(QString type_name, QWidget* container, boost::uuids::uuid id)
  : node_base(std::move(type_name), std::move(id))
  , _p(std::make_unique<impl>())
{
  _p->_container = container;
}

gui_node::~gui_node()
{
  std::for_each(begin(), end(), [this](auto& child) {
    removeWidget(std::dynamic_pointer_cast<gui_node>(child)->widget());
  });
}

auto gui_node::init() -> void
{
  _p->_widget = std::make_unique<gui_node_widget>(
    std::dynamic_pointer_cast<gui_node>(shared_from_this()), _p->_container);
  _p->_widget->setLayoutDirection(_p->_layout_direction._s);
  connect(this, &node_base::nameChanged, _p->_widget.get(), &QObject::setObjectName);
}

auto gui_node::setEditMode(bool mode) -> void
{
  _p->_widget->setEditMode(mode);

  std::for_each(begin(), end(), [mode](auto& child) {
    if (auto c = std::dynamic_pointer_cast<gui_node>(child)) {
      c->setEditMode(mode);
    }
  });
}

auto gui_node::widget() -> QWidget*
{
  return _p->_widget.get();
}

auto gui_node::layoutDirection() const -> properties::invisible_layout_direction_t
{
  return _p->_layout_direction;
}

auto gui_node::setLayoutDirection(properties::invisible_layout_direction_t const& ld) -> void
{
  if (_p->_layout_direction == ld) {
    return;
  }

  _p->_layout_direction = ld;
  _p->_widget->setLayoutDirection(_p->_layout_direction._s);
  Q_EMIT layoutDirectionChanged(_p->_layout_direction);
}

auto gui_node::xPosition() const -> int
{
  return _p->_x_position;
}

auto gui_node::setXposition(int x) -> void
{
  if (_p->_x_position == x) {
    return;
  }

  _p->_x_position = x;
  _p->_widget->setGeometry(_p->_x_position, _p->_y_position, _p->_width, _p->_height);
  Q_EMIT xPositionChanged(_p->_x_position);
}

auto gui_node::yPosition() const -> int
{
  return _p->_y_position;
}

auto gui_node::setYposition(int y) -> void
{
  if (_p->_y_position == y) {
    return;
  }

  _p->_y_position = y;
  _p->_widget->setGeometry(_p->_x_position, _p->_y_position, _p->_width, _p->_height);
  Q_EMIT yPositionChanged(_p->_y_position);
}

auto gui_node::width() const -> int
{
  return _p->_width;
}

auto gui_node::setWidth(int w) -> void
{
  if (_p->_width == w) {
    return;
  }

  _p->_width = w;

  if (_p->_width > 0) {
    _p->_widget->setFixedWidth(_p->_width);
  } else {
    _p->_widget->setFixedWidth(QWIDGETSIZE_MAX);
  }

  Q_EMIT widthChanged(_p->_width);
}

auto gui_node::height() const -> int
{
  return _p->_height;
}

auto gui_node::setHeight(int h) -> void
{
  if (_p->_height == h) {
    return;
  }

  _p->_height = h;

  if (_p->_height > 0) {
    _p->_widget->setFixedHeight(_p->_height);
  } else {
    _p->_widget->setFixedHeight(QWIDGETSIZE_MAX);
  }

  Q_EMIT heightChanged(_p->_height);
}

auto gui_node::enabled() const -> properties::connected_boolean_t
{
  return _p->_enabled;
}

auto gui_node::setEnabled(properties::connected_boolean_t e) -> void
{
  if (_p->_enabled == e) {
    return;
  }

  _p->_enabled = e;
  _p->_widget->setEnabled(e.value());
  Q_EMIT enabledChanged(_p->_enabled);
}

auto gui_node::visible() const -> bool
{
  return _p->_visible;
}

auto gui_node::setVisible(bool v) -> void
{
  if (_p->_visible == v) {
    return;
  }

  _p->_visible = v;
  _p->_widget->setVisible(_p->_visible);

  if (_p->_visible) {
    _p->_widget->show();
  } else {
    _p->_widget->hide();
  }

  Q_EMIT visibleChanged(_p->_visible);
}

auto gui_node::styleSheet() const -> properties::text_t
{
  return _p->_style_sheet;
}

auto gui_node::setStyleSheet(properties::text_t const& sheet) -> void
{
  if (_p->_style_sheet == sheet) {
    return;
  }

  _p->_style_sheet = sheet;
  _p->_widget->setStyleSheet(_p->_style_sheet._s);
  Q_EMIT styleSheetChanged(_p->_style_sheet);
}

auto gui_node::addWidget(QWidget* widget, int index) -> void
{
  _p->_widget->addWidget(widget, index);
}

auto gui_node::removeWidget(QWidget* widget) -> void
{
  _p->_widget->removeWidget(widget);
}

auto gui_node::container() const -> QWidget*
{
  return _p->_container;
}

auto gui_node::childAboutToBeAdded(std::shared_ptr<node_base> const& child) -> bool
{
  auto result = beforeChildAdd(child);
  return result;
}

auto gui_node::childAdded(std::shared_ptr<node_base> const& child) -> bool
{
  auto gui_child = std::dynamic_pointer_cast<gui_node>(child);
  addWidget(gui_child->widget(), childIndex(child));
  connect(gui_child.get(), &gui_node::itemSelected, this, &gui_node::itemSelected);
  connect(gui_child.get(), &gui_node::defaultForDataRequested, this,
          &gui_node::defaultForDataRequested);

  return afterChildAdd(child);
}

auto gui_node::childAboutToBeRemoved(std::shared_ptr<node_base> const& child) -> bool
{
  auto result = beforeChildRemove(child);
  auto gui_child = std::dynamic_pointer_cast<gui_node>(child);

  disconnect(gui_child.get(), &gui_node::itemSelected, this, &gui_node::itemSelected);
  disconnect(gui_child.get(), &gui_node::defaultForDataRequested, this,
             &gui_node::defaultForDataRequested);

  if (result) {
    removeWidget(std::dynamic_pointer_cast<gui_node>(child)->widget());
  }

  return result;
}

auto gui_node::childRemoved(std::shared_ptr<node_base> const& child) -> bool
{
  return afterChildRemove(child);
}

void gui_node::childMoved(std::shared_ptr<node_base> const& child, size_t from_index,
                          size_t to_index)
{
  removeWidget(std::dynamic_pointer_cast<gui_node>(child)->widget());
  addWidget(std::dynamic_pointer_cast<gui_node>(child)->widget(), to_index);
  afterChildMoved(child, from_index, to_index);
}
} // namespace yoyo
