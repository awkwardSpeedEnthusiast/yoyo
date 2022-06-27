#pragma once

#include "nodes_export.h"
#include "yoyo/node_base.h"

namespace yoyo
{
class YOYO_NODES_SHARED_EXPORT configuration final : public node_base
{
  Q_OBJECT

  Q_PROPERTY(yoyo::properties::invisible_string_t version READ version)

public:
  explicit configuration(boost::uuids::uuid identifier = boost::uuids::uuid {});
  virtual ~configuration() = default;
  configuration(configuration const& other) = delete;
  configuration& operator=(configuration const& other) = delete;
  configuration(configuration&& other) = delete;
  configuration& operator=(configuration&& other) = delete;

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto acceptsChildren() const -> bool final;
  auto acceptsChild(std::shared_ptr<node_base> const& child) const -> bool final;
  auto acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool final;

  auto version() const -> yoyo::properties::invisible_string_t;

  static auto typeId() -> boost::uuids::uuid;

private:
  auto childAboutToBeAdded(std::shared_ptr<node_base> const& child) -> bool final;
  auto childAdded(std::shared_ptr<node_base> const& child) -> bool final;
  auto childAboutToBeRemoved(std::shared_ptr<node_base> const& child) -> bool final;
  auto childRemoved(std::shared_ptr<node_base> const& child) -> bool final;
  auto childMoved(std::shared_ptr<node_base> const& child, size_t from_index, size_t to_index)
    -> void final;

private:
  static boost::uuids::uuid const _typeId;

  yoyo::properties::invisible_string_t _version;
  yoyo::properties::script_t _afterLoad;
  yoyo::properties::script_t _beforeClose;
  int _timerPeriod;
  yoyo::properties::script_t _timerScript;
};
} // namespace yoyo
