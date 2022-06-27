#include "configuration_data.hpp"

#include "data_root.hpp"
#include "factory_provider.hpp"
#include "gui_root.hpp"

#include <QTimer>

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace yoyo
{
boost::uuids::uuid const configuration_data::_typeId = fundamental::configuration_data_id;

configuration_data::configuration_data(boost::uuids::uuid identifier)
  : node_base { "Configuration Data", identifier }
  , _periodicTimer { std::make_unique<QTimer>() }
{
  setName({ "Configuration Data", false });

  _periodicTimer->setSingleShot(false);
  connect(_periodicTimer.get(), &QTimer::timeout, this,
          [this]() { Q_EMIT scriptExecutionRequested(_timerScript); });
}
configuration_data::~configuration_data() = default;

auto configuration_data::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto configuration_data::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}

auto configuration_data::acceptsChildren() const -> bool
{
  return false;
}

auto configuration_data::acceptsChild(std::shared_ptr<node_base> const&) const -> bool
{
  return false;
}

auto configuration_data::acceptsParent(std::shared_ptr<node_base> const& p) const -> bool
{
  return p->staticTypeId() == fundamental::configuration_id;
}

auto configuration_data::afterLoad() const -> yoyo::properties::script_t
{
  return _afterLoad;
}

auto configuration_data::beforeClose() const -> yoyo::properties::script_t
{
  return _beforeClose;
}

auto configuration_data::timerPeriod() const -> int
{
  return _timerPeriod;
}

auto configuration_data::timerScript() const -> yoyo::properties::script_t
{
  return _timerScript;
}

auto configuration_data::setAfterLoad(yoyo::properties::script_t script) -> void
{
  if (_afterLoad == script) {
    return;
  }

  _afterLoad = script;
  Q_EMIT afterLoadChanged(_afterLoad);
}

auto configuration_data::setBeforeClose(yoyo::properties::script_t script) -> void
{
  if (_beforeClose == script) {
    return;
  }

  _beforeClose = script;
  Q_EMIT beforeCloseChanged(_beforeClose);
}

auto configuration_data::setTimerPeriod(int intervalInS) -> void
{
  if (_timerPeriod == intervalInS) {
    return;
  }

  _timerPeriod = intervalInS;
  if (_periodicTimer->isActive()) {
    _periodicTimer->stop();
  }
  Q_EMIT timerPeriodChanged(_timerPeriod);

  if (_timerPeriod > 0 && _timerScript != properties::script_t {}) {
    _periodicTimer->setInterval(_timerPeriod * 1000);
    _periodicTimer->start();
  }
}

auto configuration_data::setTimerScript(yoyo::properties::script_t script) -> void
{
  if (_timerScript == script) {
    return;
  }

  _timerScript = script;
  if (_periodicTimer->isActive()) {
    _periodicTimer->stop();
  }
  Q_EMIT timerScriptChanged(_timerScript);
  if (_timerPeriod > 0 && _timerScript != properties::script_t {}) {
    _periodicTimer->setInterval(_timerPeriod * 1000);
    _periodicTimer->start();
  }
}

auto configuration_data::childAboutToBeAdded(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return false;
}

auto configuration_data::childAdded(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return false;
}

auto configuration_data::childAboutToBeRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return false;
}

auto configuration_data::childRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool
{
  return false;
}

auto configuration_data::childMoved(std::shared_ptr<node_base> const& /*child*/,
                                    size_t /*from_index*/, size_t /*to_index*/) -> void
{
}

} // namespace yoyo
