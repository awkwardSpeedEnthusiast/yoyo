#include "group.hpp"

#include <QGroupBox>
#include <QLayout>

#include <boost/uuid/string_generator.hpp>

namespace yoyo
{
struct group::Impl {
  QString _title;
  QGroupBox* _box;
};

boost::uuids::uuid const group::_typeId =
  boost::uuids::string_generator {}("b5c09162-9eb8-4ed2-95b8-3096c8dcb356");
group::group(boost::uuids::uuid identifier)
  : gui_node { "group", new QGroupBox, identifier }
  , _p { std::make_unique<Impl>() }
{
  _p->_box = dynamic_cast<QGroupBox*>(container());
}

group::~group() = default;
auto group::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto group::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto group::init() -> void
{
  gui_node::init();
  setLayoutDirection({ types::layout_direction_t::VERTICAL, true });
}

auto group::acceptsChildren() const -> bool
{
  return true;
}

auto group::acceptsChild(std::shared_ptr<node_base> const& /*child*/) const -> bool
{
  return true;
}

auto group::acceptsParent(std::shared_ptr<node_base> const& /*parent*/) const -> bool
{
  return true;
}

auto group::supportsValueType(types::value_t /*valueType*/) const -> bool
{
  return false;
}

auto group::title() const -> QString
{
  return _p->_title;
}

auto group::setTitle(QString const& t) -> void
{
  if (_p->_title == t) {
    return;
  }

  _p->_title = t;
  _p->_box->setTitle(_p->_title);
  Q_EMIT titleChanged(_p->_title);
}

auto group::beforeChildAdd(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto group::afterChildAdd(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto group::beforeChildRemove(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto group::afterChildRemove(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto group::afterChildMoved(std::shared_ptr<node_base> const& /*child*/, size_t /*from_index*/,
                            size_t /*to_index*/) -> void
{
}
} // namespace yoyo
