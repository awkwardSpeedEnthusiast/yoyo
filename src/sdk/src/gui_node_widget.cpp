#include "gui_node_widget.hpp"
#include "yoyo/gui_node.h"

#include <QApplication>
#include <QBoxLayout>
#include <QDrag>
#include <QEvent>
#include <QLayout>
#include <QMetaProperty>
#include <QMimeData>
#include <QMouseEvent>

#include <boost/uuid/string_generator.hpp>

#include <functional>

namespace
{
auto fromWidget(QWidget* w) -> yoyo::gui_node_widget*
{
  if (auto gui = dynamic_cast<yoyo::gui_node_widget*>(w)) {
    return gui;
  }

  while (w) {
    w = w->parentWidget();

    if (auto gui = dynamic_cast<yoyo::gui_node_widget*>(w)) {
      return gui;
    }
  }

  return nullptr;
}

auto cleanout_children(std::vector<QWidget*>& children, QWidget* reference) -> void
{
  children.erase(
    std::remove_if(std::begin(children), std::end(children),
                   [reference](auto* element) { return element->parent() != reference; }),
    std::end(children));
}

const constexpr auto mimetype_gui_element = "application/yoyo/gui-element";
const constexpr auto mimetype_gui_new = "application/yoyo/gui-new";
const constexpr auto mimetype_data_element = "application/yoyo/data-element";
} // namespace

namespace yoyo
{
gui_node_widget::gui_node_widget(std::weak_ptr<gui_node> parent, QWidget* container_widget)
  : _parent(parent)
  , _container(container_widget)
{
  installEventFilter(this);
  setAcceptDrops(true);

  if (_container) {
    _container->setParent(this);
    setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->setSpacing(0);
    layout()->addWidget(_container.get());
  }
}

gui_node_widget::~gui_node_widget() = default;

auto gui_node_widget::containerWidget() -> QWidget*
{
  return _container.get();
}

auto gui_node_widget::setLayoutDirection(types::layout_direction_t direction) -> void
{
  auto* w = _container ? _container.get() : this;

  if ((w->layout() == nullptr) && (direction == types::layout_direction_t::FREE)) {
    return;
  }

  if (w->layout()) {
    switch (direction) {
    case types::layout_direction_t::FREE: {
      auto l = w->layout();
      delete l;
    } break;
    case types::layout_direction_t::HORIZONTAL:
      dynamic_cast<QBoxLayout*>(w->layout())->setDirection(QBoxLayout::LeftToRight);
      break;
    case types::layout_direction_t::VERTICAL:
      dynamic_cast<QBoxLayout*>(w->layout())->setDirection(QBoxLayout::TopToBottom);
      break;
    }
  } else {
    auto* l =
      new QBoxLayout((direction == types::layout_direction_t::HORIZONTAL ? QBoxLayout::LeftToRight
                                                                         : QBoxLayout::TopToBottom),
                     w);
    l->setSpacing(1);
    l->setContentsMargins(1, 1, 1, 1);
    w->setLayout(l);

    for (auto child : _children) {
      l->addWidget(dynamic_cast<QWidget*>(child));
    }
  }
}

auto gui_node_widget::addWidget(QWidget* widget, int index) -> void
{
  cleanout_children(_children, _container ? _container.get() : this);

  if ((index < 0) || (static_cast<size_t>(index) >= _children.size())) {
    index = _children.size();
  }

  _children.insert(_children.begin() + index, widget);

  if (_container) {
    widget->setParent(_container.get());

    if (_container->layout()) {
      dynamic_cast<QBoxLayout*>(_container->layout())->insertWidget(index, widget);
    }
  } else {
    widget->setParent(this);

    if (layout()) {
      dynamic_cast<QBoxLayout*>(layout())->insertWidget(index, widget);
    }
  }
}

auto gui_node_widget::removeWidget(QWidget* widget) -> void
{
  if (_container) {
    if (_container->layout()) {
      _container->layout()->removeWidget(widget);
    }
  } else {
    if (layout()) {
      layout()->removeWidget(widget);
    }
  }

  widget->setParent(nullptr);
  cleanout_children(_children, this);
}

auto gui_node_widget::setEditMode(bool editModeOn) -> void
{
  _active = editModeOn;
  setAcceptDrops(editModeOn);
}

auto gui_node_widget::eventFilter(QObject* obj, QEvent* event) -> bool
{
  if (event->type() != QEvent::MouseButtonPress) {
    return QWidget::eventFilter(obj, event);
  }

  return QWidget::eventFilter(obj, event);
}

auto gui_node_widget::mousePressEvent(QMouseEvent* event) -> void
{
  if (event->button() == Qt::LeftButton) {
    _startDrag = event->pos();
    auto parent = _parent.lock();

    if (parent) {
      Q_EMIT parent->itemSelected(parent);
    }
  }
}

auto gui_node_widget::mouseMoveEvent(QMouseEvent* event) -> void
{
  if (!((event->buttons() & Qt::LeftButton) || (event->buttons() & Qt::RightButton))) {
    return;
  }

  if ((event->pos() - _startDrag).manhattanLength() < QApplication::startDragDistance()) {
    return;
  }

  // Check for valid node and whether drag&drop is currently available
  auto parent = _parent.lock();

  if (!_active || !parent) {
    event->ignore();
    return;
  }

  // Only items should start drag&drop, which are actual gui-elements, not the root-object nor
  // the gui-element container below that. It could make sense to check on type instead of
  // hierarchy.
  auto node_parent = parent->parent().lock();

  if (!node_parent) {
    event->ignore();
    return;
  }

  auto node_parent2 = node_parent->parent().lock();

  if (!node_parent2) {
    event->ignore();
    return;
  }

  auto* drag = new QDrag(parent.get());
  auto* mimeData = new QMimeData;
  mimeData->setData(mimetype_gui_element, {});
  drag->setMimeData(mimeData);

  if (event != nullptr) {
    drag->setHotSpot(event->pos() - this->pos());
  }

  drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::MoveAction);

  event->accept();
}

auto gui_node_widget::checkDragEvent_new(QMimeData const* /*data*/, Qt::MouseButtons buttons)
  -> bool
{
  auto parent = _parent.lock();

  if (!parent) {
    return false;
  }

  if (((buttons & Qt::RightButton) == 0) && (!parent || !parent->acceptsChildren())) {
    auto p_parent = parent->parent().lock();

    if (!p_parent || !p_parent->acceptsChildren()) {
      return false;
    }
  }

  return true;
}

auto gui_node_widget::checkDragEvent_data(QMimeData const* data, Qt::MouseButtons buttons) -> bool
{
  auto parent = _parent.lock();

  if (!parent) {
    return false;
  }

  if (buttons == Qt::RightButton) {
    auto d = data->data(mimetype_data_element);
    QDataStream dataStream(&d, QIODevice::ReadOnly);
    int type;
    dataStream >> type;

    return parent->supportsValueType(static_cast<types::value_t>(type));
  } else if (buttons == Qt::LeftButton) {
    if (!parent->acceptsChildren()) {
      auto p_parent = parent->parent().lock();

      if (!p_parent || !p_parent->acceptsChildren()) {
        return false;
      }
    }

    return true;
  }

  return false;
}

auto gui_node_widget::checkDragEvent_other(QMimeData const* /*data*/, Qt::MouseButtons /*buttons*/,
                                           node_base* source) -> bool
{
  auto parent = _parent.lock();

  if (!parent || !source) {
    return false;
  }

  if (!parent || !parent->acceptsChildren() || !parent->acceptsChild(source->shared_from_this())) {
    auto p_parent = parent->parent().lock();

    if (!p_parent || !p_parent->acceptsChildren()
        || !p_parent->acceptsChild(source->shared_from_this())) {
      return false;
    }
  }

  return true;
}

auto gui_node_widget::dragEnterEvent(QDragEnterEvent* event) -> void
{
  if (!((fromWidget(childAt(event->position())) == this)
        || ((childAt(event->position()) == nullptr)))) {
    return;
  }

  if (event->mimeData()->hasFormat(mimetype_gui_new)
      && checkDragEvent_new(event->mimeData(), event->buttons())) {
    event->acceptProposedAction();
    event->accept();
  } else if (event->mimeData()->hasFormat(mimetype_data_element)
             && checkDragEvent_data(event->mimeData(), event->buttons())) {
    event->acceptProposedAction();
    event->accept();
  } else if (event->mimeData()->hasFormat(mimetype_gui_element)) {
    auto* src = dynamic_cast<node_base*>(event->source());

    if (checkDragEvent_other(event->mimeData(), event->buttons(), src)) {
      event->acceptProposedAction();
      event->accept();
    }
  } else {
    event->ignore();
  }
}

auto gui_node_widget::dragMoveEvent(QDragMoveEvent* event) -> void
{
  if (!((fromWidget(childAt(event->position())) == this)
        || ((childAt(event->position()) == nullptr)))) {
    return;
  }

  if (event->mimeData()->hasFormat(mimetype_gui_new)
      && checkDragEvent_new(event->mimeData(), event->buttons())) {
    event->acceptProposedAction();
    event->accept();
  } else if (event->mimeData()->hasFormat(mimetype_data_element)
             && checkDragEvent_data(event->mimeData(), event->buttons())) {
    event->acceptProposedAction();
    event->accept();
  } else if (event->mimeData()->hasFormat(mimetype_gui_element)) {
    auto* src = dynamic_cast<node_base*>(event->source());

    if (checkDragEvent_other(event->mimeData(), event->buttons(), src)) {
      event->acceptProposedAction();
      event->accept();
    }
  } else {
    event->ignore();
  }
}

auto gui_node_widget::dropEvent(QDropEvent* event) -> void
{
  if (!((fromWidget(childAt(event->position())) == this)
        || ((childAt(event->position()) == nullptr)))) {
    return;
  }

  bool b { false };
  auto guard = std::unique_ptr<bool, std::function<void(bool)>>(&b, [event](bool b) {
    if (b) {
      event->acceptProposedAction();
      event->accept();
    } else {
      event->ignore();
    }
  });
  auto parent = _parent.lock();

  if (!parent) {
    return;
  }

  auto pp = parent->parent().lock();

  if (!pp) {
    return;
  }

  auto index = pp->childIndex(parent) + 1;

  if (event->mimeData()->hasFormat(mimetype_gui_new)
      && checkDragEvent_new(event->mimeData(), event->buttons())) {
    auto data = event->mimeData()->data(mimetype_gui_new);
    auto id = boost::uuids::string_generator {}(data.toStdString());

    if (event->buttons() == Qt::RightButton) {
      Q_EMIT pp->exchangeRequested(parent, id);
    } else if (parent->acceptsChildren()) {
      Q_EMIT pp->addRequested(parent, id, -1);
    } else {
      Q_EMIT pp->addRequested(pp, id, index);
    }

    b = true;
  } else if (event->mimeData()->hasFormat(mimetype_data_element)
             && checkDragEvent_data(event->mimeData(), event->buttons())) {
    QByteArray itemData = event->mimeData()->data(mimetype_data_element);
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    QString data_element_path;
    int type;
    dataStream >> type >> data_element_path;

    bool doInput = (event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier)) > 0;
    bool doOutput = (event->modifiers() & (Qt::AltModifier | Qt::ShiftModifier)) > 0;
    bool doAuto = event->modifiers() == Qt::NoModifier;

    if (event->buttons() == Qt::RightButton) {
      auto meta = parent->metaObject();

      for (auto i = 0; i < meta->propertyCount(); i++) {
        if (meta->property(i).typeName() == QString("yoyo::properties::connection_t")) {
          auto c = meta->property(i).read(parent.get()).value<properties::connection_t>();
          c._in = doInput ? data_element_path : "";
          c._out = doOutput ? data_element_path : "";
          c._auto = doAuto ? data_element_path : "";
          meta->property(i).write(parent.get(), QVariant::fromValue(c));
        }
      }
    } else {
      if (parent->acceptsChildren()) {
        Q_EMIT parent->defaultForDataRequested(data_element_path, parent, -1, doInput, doOutput,
                                               doAuto);
      } else if (auto gpp = std::dynamic_pointer_cast<gui_node>(pp)) {
        Q_EMIT gpp->defaultForDataRequested(data_element_path, pp, index, doInput, doOutput,
                                            doAuto);
      }
    }

    b = true;
  } else if (event->mimeData()->hasFormat(mimetype_gui_element)) {
    auto* source = dynamic_cast<node_base*>(event->source());

    if (!checkDragEvent_other(event->mimeData(), event->buttons(), source)) {
      return;
    }

    if (parent->acceptsChild(source->shared_from_this())) {
      parent->addChild(source->shared_from_this());
    } else if (pp->acceptsChild(source->shared_from_this())) {
      pp->addChild(source->shared_from_this(), index);
    }

    b = true;
  }
}

auto gui_node_widget::contextMenuEvent(QContextMenuEvent* /*event*/) -> void {}
} // namespace yoyo
