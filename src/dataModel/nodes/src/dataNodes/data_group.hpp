#pragma once

#include "export_on_test.hpp"
#include "yoyo/data_container_node.h"

namespace yoyo
{
class YOYO_TESTING_EXPORT data_group final : public data_container_node
{
  Q_OBJECT

public:
  explicit data_group(boost::uuids::uuid identifier = boost::uuids::uuid {});
  virtual ~data_group() = default;
  data_group(data_group const& other) = delete;
  data_group& operator=(data_group const& other) = delete;
  data_group(data_group&& other) = delete;
  data_group& operator=(data_group&& other) = delete;

  auto staticTypeId() const -> boost::uuids::uuid final;
  static auto typeId() -> boost::uuids::uuid;

private:
  auto value() const -> types::variant_t final;

private:
  static boost::uuids::uuid const _typeId;
};
} // namespace yoyo
