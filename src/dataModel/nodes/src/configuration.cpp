#include "configuration.hpp"

#include "data_root.hpp"
#include "gui_root.hpp"

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace yoyo
{
boost::uuids::uuid const configuration::_typeId =
  boost::uuids::string_generator {}("ced35bdc-c7f6-4468-9b61-57fefe022e9d");

configuration::configuration(boost::uuids::uuid identifier)
  : node_base("Configuration", identifier)
{
}
auto configuration::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto configuration::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto configuration::acceptsChildren() const -> bool
{
  return true;
}

auto configuration::acceptsChild(std::shared_ptr<node_base> const& child) const -> bool
{
  return child->staticTypeId() == data_root::typeId()
      || child->staticTypeId() == gui_root::typeId();
}

auto configuration::acceptsParent(std::shared_ptr<node_base> const&) const -> bool
{
  return false;
}

auto configuration::version() const -> yoyo::properties::invisible_string_t
{
  return _version;
}

auto configuration::afterLoad() const -> yoyo::properties::script_t
{
  return _afterLoad;
}

auto configuration::beforeClose() const -> yoyo::properties::script_t
{
  return _beforeClose;
}

auto configuration::timerPeriod() const -> int
{
  return _timerPeriod;
}

auto configuration::timerScript() const -> yoyo::properties::script_t
{
  return _timerScript;
}

auto configuration::setAfterLoad(yoyo::properties::script_t script) -> void
{
  if (_afterLoad == script) {
    return;
  }

  _afterLoad = script;
  Q_EMIT afterLoadChanged(_afterLoad);
}

auto configuration::setBeforeClose(yoyo::properties::script_t script) -> void
{
  if (_beforeClose == script) {
    return;
  }

  _beforeClose = script;
  Q_EMIT beforeCloseChanged(_beforeClose);
}

auto configuration::setTimerPeriod(int intervalInS) -> void
{
  if (_timerPeriod == intervalInS) {
    return;
  }

  _timerPeriod = intervalInS;
  Q_EMIT timerPeriodChanged(_timerPeriod);
}

auto configuration::setTimerScript(yoyo::properties::script_t script) -> void
{
  if (_timerScript == script) {
    return;
  }

  _timerScript = script;
  Q_EMIT timerScriptChanged(_timerScript);
}

auto configuration::childAboutToBeAdded(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto configuration::childAdded(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto configuration::childAboutToBeRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto configuration::childRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return true;
}

auto configuration::childMoved(std::shared_ptr<node_base> const& /*child*/, size_t /*from_index*/,
                               size_t /*to_index*/) -> void
{
}
} // namespace yoyo
