#pragma once

#include "export_on_test.hpp"
#include "leaf_node.hpp"

#include <memory>

namespace yoyo
{
class YOYO_TESTING_EXPORT button final : public leaf_node
{
  Q_OBJECT

  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(yoyo::properties::script_t onClick READ onClick WRITE setOnClick NOTIFY onClickChanged)
  // image property
  // checkable property?
public:
  explicit button(boost::uuids::uuid identifier = boost::uuids::uuid {});
  ~button() override;
  button(button const& other) = delete;
  button& operator=(button const& other) = delete;
  button(button&& other) = delete;
  button& operator=(button&& other) = delete;
  auto init() -> void final;

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto supportsValueType(types::value_t valueType) const -> bool final;

  auto title() const -> QString;
  auto setTitle(QString const& t) -> void;

  auto onClick() const -> yoyo::properties::script_t;
  auto setOnClick(yoyo::properties::script_t s) -> void;

  static auto typeId() -> boost::uuids::uuid;

Q_SIGNALS:
  void titleChanged(QString t);
  void onClickChanged(yoyo::properties::script_t s);

private:
  static boost::uuids::uuid const _typeId;
  struct Impl;
  std::unique_ptr<Impl> _p;
};
} // namespace yoyo
