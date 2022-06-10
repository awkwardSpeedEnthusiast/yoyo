#pragma once

#include "export_on_test.hpp"
#include "leaf_node.hpp"

#include <memory>

namespace yoyo
{
class YOYO_TESTING_EXPORT check_box final : public leaf_node
{
  Q_OBJECT

  Q_PROPERTY(
    yoyo::properties::connected_string_t title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(yoyo::properties::connection_t connection READ connection WRITE setConnection NOTIFY
               connectionChanged)

public:
  explicit check_box(boost::uuids::uuid identifier = boost::uuids::uuid {});
  ~check_box() override;
  check_box(check_box const& other) = delete;
  check_box& operator=(check_box const& other) = delete;
  check_box(check_box&& other) = delete;
  check_box& operator=(check_box&& other) = delete;
  auto init() -> void final;

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto supportsValueType(types::value_t valueType) const -> bool final;

  auto title() const -> yoyo::properties::connected_string_t;
  auto setTitle(yoyo::properties::connected_string_t const& t) -> void;

  auto connection() const -> yoyo::properties::connection_t;
  auto setConnection(yoyo::properties::connection_t s) -> void;

  static auto typeId() -> boost::uuids::uuid;

Q_SIGNALS:
  void titleChanged(yoyo::properties::connected_string_t t);
  void connectionChanged(yoyo::properties::connection_t s);

private:
  static boost::uuids::uuid const _typeId;
  struct Impl;
  std::unique_ptr<Impl> _p;
};
} // namespace yoyo
