#pragma once

#include "export_on_test.hpp"
#include "yoyo/data_value_node.h"

namespace yoyo
{
class YOYO_TESTING_EXPORT string_node final : public data_value_node
{
  Q_OBJECT

public:
  explicit string_node(boost::uuids::uuid identifier = boost::uuids::uuid {});
  virtual ~string_node() = default;
  string_node(string_node const& other) = delete;
  string_node& operator=(string_node const& other) = delete;
  string_node(string_node&& other) = delete;
  string_node& operator=(string_node&& other) = delete;

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto value() const -> types::variant_t final;
  auto setValue(QVariant const& value) -> void final;
  static auto typeId() -> boost::uuids::uuid;

private:
  static boost::uuids::uuid const _typeId;
  QString _bufferedValue;
};
} // namespace yoyo
