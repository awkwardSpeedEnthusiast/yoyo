#include "yoyo/node_base.h"
#include "yoyo/property_types.h"

#include <boost/uuid/random_generator.hpp>

#include <QVariant>

namespace yoyo
{
struct node_base::impl {
  impl(QString n, boost::uuids::uuid id)
    : _type { std::move(n) }
    , _identifier { std::move(id) }
  {
  }

  QString _name;
  QString const _type;
  std::vector<std::shared_ptr<node_base>> _children;
  std::weak_ptr<node_base> _parent;
  boost::uuids::uuid const _identifier;
};

node_base::node_base(QString type, boost::uuids::uuid identifier)
  : QObject()
  , _p { std::make_unique<impl>(std::move(type), identifier.is_nil()
                                                   ? boost::uuids::random_generator {}()
                                                   : std::move(identifier)) }
{
}

node_base::~node_base() = default;

auto node_base::name() const -> QString
{
  return _p->_name;
}

auto node_base::setName(QString const& value) -> void
{
  if (_p->_name == value) {
    return;
  }

  _p->_name = value;
  Q_EMIT nameChanged(_p->_name);
}

auto node_base::type() const -> QString
{
  return _p->_type;
}

auto node_base::identifier() const -> boost::uuids::uuid
{
  return _p->_identifier;
}

auto node_base::childCount() const -> size_t
{
  return _p->_children.size();
}

auto node_base::childAt(size_t index) const -> std::shared_ptr<node_base>
{
  return _p->_children.at(index);
}

auto node_base::childIndex(std::shared_ptr<node_base> const& child) const -> int
{
  auto it = std::find(std::begin(_p->_children), std::end(_p->_children), child);

  if (it == std::end(_p->_children)) {
    return -1;
  }

  return std::distance(std::begin(_p->_children), it);
}

auto node_base::addChild(const std::shared_ptr<node_base>& child, int index) -> void
{
  if ((child == nullptr) || (child.get() == this) || !acceptsChild(child)
      || !child->acceptsParent(shared_from_this())) {
    return;
  }

  if (std::find(std::begin(_p->_children), std::end(_p->_children), child)
      != std::end(_p->_children)) {
    return;
  }

  if (auto oldParent = child->parent().lock()) {
    oldParent->removeChild(child);
  }

  childAboutToBeAdded(child);

  child->_p->_parent = weak_from_this();
  connect(child.get(), &node_base::treeChanged, this, &node_base::treeChanged);
  connect(child.get(), &node_base::addRequested, this, &node_base::addRequested);
  connect(child.get(), &node_base::exchangeRequested, this, &node_base::exchangeRequested);
  connect(child.get(), &node_base::scriptExecutionRequested, this,
          &node_base::scriptExecutionRequested);

  if ((index < 0) || (static_cast<size_t>(index) >= _p->_children.size())) {
    _p->_children.push_back(child);
  } else {
    _p->_children.insert(std::begin(_p->_children) + index, child);
  }

  childAdded(child);
  Q_EMIT treeChanged(weak_from_this(), child, ChangeOperation::ADDED);
}

auto node_base::removeChild(std::shared_ptr<node_base> const& child) -> void
{
  auto position = std::find(std::begin(_p->_children), std::end(_p->_children), child);

  if (position == std::end(_p->_children)) {
    return;
  }

  childAboutToBeRemoved(child);
  child->_p->_parent = {};
  _p->_children.erase(position);
  disconnect(child.get(), &node_base::treeChanged, this, &node_base::treeChanged);
  disconnect(child.get(), &node_base::addRequested, this, &node_base::addRequested);
  disconnect(child.get(), &node_base::exchangeRequested, this, &node_base::exchangeRequested);
  disconnect(child.get(), &node_base::scriptExecutionRequested, this,
             &node_base::scriptExecutionRequested);
  childRemoved(child);
  Q_EMIT treeChanged(weak_from_this(), child, ChangeOperation::REMOVED);
}

auto node_base::removeChild(size_t index) -> void
{
  if (index >= _p->_children.size()) {
    return;
  }

  auto child = _p->_children[index];
  removeChild(child);
}

auto node_base::parent() const -> std::weak_ptr<node_base>
{
  return _p->_parent;
}

auto node_base::moveChild(std::shared_ptr<node_base> const& child, size_t targetIndex) -> void
{
  auto position = std::find(std::begin(_p->_children), std::end(_p->_children), child);

  if ((position == std::end(_p->_children))
      || (static_cast<size_t>(std::distance(std::begin(_p->_children), position)) == targetIndex)) {
    return;
  }

  auto index = std::distance(_p->_children.begin(), position);
  // No notifications necessary, because children are neither added nor removed, only reordered.
  _p->_children.erase(position);

  if (static_cast<size_t>(targetIndex) >= _p->_children.size()) {
    _p->_children.push_back(child);
    childMoved(child, index, _p->_children.size() - 1);
  } else {
    _p->_children.insert(std::begin(_p->_children) + targetIndex, child);
    childMoved(child, index, targetIndex);
  }

  Q_EMIT treeChanged(weak_from_this(), child, ChangeOperation::MOVED);
}

auto node_base::begin() -> iterator
{
  return _p->_children.begin();
}

auto node_base::begin() const -> const_iterator
{
  return _p->_children.begin();
}

auto node_base::end() -> iterator
{
  return _p->_children.end();
}

auto node_base::end() const -> const_iterator
{
  return _p->_children.end();
}

auto registerAllPropertyTypes() -> void
{
  qRegisterMetaType<yoyo::types::value_t>();
  qRegisterMetaType<yoyo::types::access_t>();
  qRegisterMetaType<yoyo::types::variant_t>();
  qRegisterMetaType<yoyo::types::layout_direction_t>();
  qRegisterMetaType<yoyo::properties::string_t>();
  qRegisterMetaType<yoyo::properties::invisible_string_t>();
  qRegisterMetaType<yoyo::properties::invisible_layout_direction_t>();
  qRegisterMetaType<yoyo::properties::connected_string_t>();
  qRegisterMetaType<yoyo::properties::connected_boolean_t>();
  qRegisterMetaType<yoyo::properties::connection_t>();
  qRegisterMetaType<yoyo::properties::in_connection_t>();
  qRegisterMetaType<yoyo::properties::out_connection_t>();
  qRegisterMetaType<yoyo::properties::script_t>();
  qRegisterMetaType<yoyo::properties::text_t>();
  qRegisterMetaType<yoyo::properties::enum_t>();
  qRegisterMetaType<yoyo::properties::patterned_string_t>();
  qRegisterMetaType<yoyo::properties::limited_uint8_t>();
  qRegisterMetaType<yoyo::properties::limited_uint16_t>();
  qRegisterMetaType<yoyo::properties::limited_uint32_t>();
  qRegisterMetaType<yoyo::properties::limited_uint64_t>();
  qRegisterMetaType<yoyo::properties::limited_int8_t>();
  qRegisterMetaType<yoyo::properties::limited_int16_t>();
  qRegisterMetaType<yoyo::properties::limited_int32_t>();
  qRegisterMetaType<yoyo::properties::limited_int64_t>();
  qRegisterMetaType<yoyo::properties::limited_float_t>();
  qRegisterMetaType<yoyo::properties::integer_format_t>();
  qRegisterMetaType<int8_t>();
  qRegisterMetaType<int16_t>();
  qRegisterMetaType<int32_t>();
  qRegisterMetaType<int64_t>();
  qRegisterMetaType<uint8_t>();
  qRegisterMetaType<uint16_t>();
  qRegisterMetaType<uint32_t>();
  qRegisterMetaType<uint64_t>();
}

auto value_equals(QVariant const& lhs, QVariant const& rhs) -> bool
{
  if (!lhs.isValid() || !rhs.isValid()) {
    return false;
  }
  if (std::string(lhs.typeName()) != rhs.typeName()) {
    return false;
  }

  static std::map<std::string, std::function<bool(QVariant const&, QVariant const&)>> const
    expecter = {
      { { "yoyo::properties::patterned_string_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::patterned_string_t>()
              == v2.value<yoyo::properties::patterned_string_t>();
        } },
      { { "yoyo::properties::invisible_string_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::invisible_string_t>()
              == v2.value<yoyo::properties::invisible_string_t>();
        } },
      { { "yoyo::properties::text_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::text_t>() == v2.value<yoyo::properties::text_t>();
        } },
      { { "yoyo::properties::connected_string_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::connected_string_t>()
              == v2.value<yoyo::properties::connected_string_t>();
        } },
      { { "yoyo::properties::connected_boolean_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::connected_boolean_t>()
              == v2.value<yoyo::properties::connected_boolean_t>();
        } },
      { { "yoyo::properties::connection_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::connection_t>()
              == v2.value<yoyo::properties::connection_t>();
        } },
      { { "yoyo::properties::in_connection_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::in_connection_t>()
              == v2.value<yoyo::properties::in_connection_t>();
        } },
      { { "yoyo::properties::out_connection_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::out_connection_t>()
              == v2.value<yoyo::properties::out_connection_t>();
        } },
      { { "yoyo::properties::invisible_layout_direction_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::invisible_layout_direction_t>()
              == v2.value<yoyo::properties::invisible_layout_direction_t>();
        } },
      { { "yoyo::properties::limited_uint8_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::limited_uint8_t>()
              == v2.value<yoyo::properties::limited_uint8_t>();
        } },
      { { "yoyo::properties::limited_uint16_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::limited_uint16_t>()
              == v2.value<yoyo::properties::limited_uint16_t>();
        } },
      { { "yoyo::properties::limited_uint32_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::limited_uint32_t>()
              == v2.value<yoyo::properties::limited_uint32_t>();
        } },
      { { "yoyo::properties::limited_uint64_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::limited_uint64_t>()
              == v2.value<yoyo::properties::limited_uint64_t>();
        } },
      { { "yoyo::properties::limited_int8_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::limited_int8_t>()
              == v2.value<yoyo::properties::limited_int8_t>();
        } },
      { { "yoyo::properties::limited_int16_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::limited_int16_t>()
              == v2.value<yoyo::properties::limited_int16_t>();
        } },
      { { "yoyo::properties::limited_int32_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::limited_int32_t>()
              == v2.value<yoyo::properties::limited_int32_t>();
        } },
      { { "yoyo::properties::limited_int64_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::limited_int64_t>()
              == v2.value<yoyo::properties::limited_int64_t>();
        } },
      { { "yoyo::properties::script_t" },
        [](QVariant const& v1, QVariant const& v2) {
          return v1.value<yoyo::properties::script_t>() == v2.value<yoyo::properties::script_t>();
        } },
    };

  auto it = expecter.find(lhs.typeName());

  if (it == expecter.end()) {
    return lhs == rhs;
  }

  return it->second(lhs, rhs);
}
} // namespace yoyo
