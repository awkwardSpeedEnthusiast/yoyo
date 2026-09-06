#pragma once

#include "nodes_export.h"
#include "yoyo/node_base.h"

class QTimer;
namespace yoyo
{
class YOYO_NODES_SHARED_EXPORT configuration_data final : public node_base
{
  Q_OBJECT

  Q_PROPERTY(
    yoyo::properties::script_t afterLoad READ afterLoad WRITE setAfterLoad NOTIFY afterLoadChanged)
  Q_PROPERTY(yoyo::properties::script_t beforeClose READ beforeClose WRITE setBeforeClose NOTIFY
               beforeCloseChanged)
  Q_PROPERTY(int timerPeriod READ timerPeriod WRITE setTimerPeriod NOTIFY timerPeriodChanged)
  Q_PROPERTY(yoyo::properties::script_t timerScript READ timerScript WRITE setTimerScript NOTIFY
               timerScriptChanged)

public:
  explicit configuration_data(boost::uuids::uuid identifier = boost::uuids::uuid {});
  ~configuration_data() override;
  configuration_data(configuration_data const& other) = delete;
  configuration_data& operator=(configuration_data const& other) = delete;
  configuration_data(configuration_data&& other) = delete;
  configuration_data& operator=(configuration_data&& other) = delete;

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto acceptsChildren() const -> bool final;
  auto acceptsChild(std::shared_ptr<node_base> const& child) const -> bool final;
  auto acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool final;

  auto afterLoad() const -> yoyo::properties::script_t;
  auto beforeClose() const -> yoyo::properties::script_t;
  auto timerPeriod() const -> int;
  auto timerScript() const -> yoyo::properties::script_t;

  auto setAfterLoad(yoyo::properties::script_t script) -> void;
  auto setBeforeClose(yoyo::properties::script_t script) -> void;
  auto setTimerPeriod(int intervalInS) -> void;
  auto setTimerScript(yoyo::properties::script_t script) -> void;
  static auto typeId() -> boost::uuids::uuid;

Q_SIGNALS:
  void afterLoadChanged(yoyo::properties::script_t script);
  void beforeCloseChanged(yoyo::properties::script_t script);
  void timerPeriodChanged(int intervalInS);
  void timerScriptChanged(yoyo::properties::script_t script);

private:
  auto childAboutToBeAdded(std::shared_ptr<node_base> const& child) -> bool final;
  auto childAdded(std::shared_ptr<node_base> const& child) -> bool final;
  auto childAboutToBeRemoved(std::shared_ptr<node_base> const& child) -> bool final;
  auto childRemoved(std::shared_ptr<node_base> const& child) -> bool final;
  auto childMoved(std::shared_ptr<node_base> const& child, size_t from_index, size_t to_index)
    -> void final;

private:
  static boost::uuids::uuid const _typeId;

  yoyo::properties::script_t _afterLoad;
  yoyo::properties::script_t _beforeClose;
  int _timerPeriod;
  yoyo::properties::script_t _timerScript;
  std::unique_ptr<QTimer> _periodicTimer;
};
} // namespace yoyo
