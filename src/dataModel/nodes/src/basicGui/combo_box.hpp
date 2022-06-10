#pragma once

#include "export_on_test.hpp"
#include "leaf_node.hpp"

#include <memory>

namespace yoyo
{
class YOYO_TESTING_EXPORT combo_box final : public leaf_node
{
  Q_OBJECT

  Q_PROPERTY(
    yoyo::properties::connected_string_t title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(yoyo::properties::connected_string_t unit READ unit WRITE setUnit NOTIFY unitChanged)
  Q_PROPERTY(yoyo::properties::connection_t connection READ connection WRITE setConnection NOTIFY
               connectionChanged)
  Q_PROPERTY(yoyo::properties::enum_t enumValues READ enumValues WRITE setEnumValues NOTIFY
               enumValuesChanged)

public:
  explicit combo_box(boost::uuids::uuid identifier = boost::uuids::uuid {});
  ~combo_box() override;
  combo_box(combo_box const& other) = delete;
  combo_box& operator=(combo_box const& other) = delete;
  combo_box(combo_box&& other) = delete;
  combo_box& operator=(combo_box&& other) = delete;
  auto init() -> void final;

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto supportsValueType(types::value_t valueType) const -> bool final;

  auto title() const -> yoyo::properties::connected_string_t;
  auto setTitle(yoyo::properties::connected_string_t const& t) -> void;

  auto unit() const -> yoyo::properties::connected_string_t;
  auto setUnit(yoyo::properties::connected_string_t const& t) -> void;

  auto connection() const -> properties::connection_t;
  auto setConnection(properties::connection_t s) -> void;

  auto enumValues() const -> yoyo::properties::enum_t;
  auto setEnumValues(yoyo::properties::enum_t const& values) -> void;

  static auto typeId() -> boost::uuids::uuid;

Q_SIGNALS:
  void titleChanged(yoyo::properties::connected_string_t t);
  void unitChanged(yoyo::properties::connected_string_t t);
  void connectionChanged(yoyo::properties::connection_t s);
  void enumValuesChanged(yoyo::properties::enum_t v);

private:
  static boost::uuids::uuid const _typeId;
  struct Impl;
  std::unique_ptr<Impl> _p;
};
} // namespace yoyo
