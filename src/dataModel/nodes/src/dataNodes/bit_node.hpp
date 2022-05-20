#pragma once

#include "export_on_test.hpp"
#include "yoyo/data_value_node.h"

namespace yoyo
{
class YOYO_TESTING_EXPORT bit_node final : public data_value_node
{
  Q_OBJECT

public:
  explicit bit_node(boost::uuids::uuid identifier = boost::uuids::uuid {});
  virtual ~bit_node() = default;
  bit_node(bit_node const& other) = delete;
  bit_node& operator=(bit_node const& other) = delete;
  bit_node(bit_node&& other) = delete;
  bit_node& operator=(bit_node&& other) = delete;

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto value() const -> types::variant_t final;
  auto setValue(QVariant const& value) -> void final;
  static auto typeId() -> boost::uuids::uuid;

private:
  static boost::uuids::uuid const _typeId;
  bool _bufferedValue { false };
};
} // namespace yoyo
