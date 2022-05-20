#pragma once
#include "export_on_test.hpp"
#include "node_deserializer.h"

namespace yoyo::io
{
class YOYO_TESTING_EXPORT node_deserializer_json final : public node_deserializer
{
public:
  using node_deserializer::node_deserializer;
  ~node_deserializer_json() override;
  node_deserializer_json(node_deserializer_json const&) = default;
  node_deserializer_json(node_deserializer_json&&) = default;
  node_deserializer_json& operator=(node_deserializer_json const&) = default;
  node_deserializer_json& operator=(node_deserializer_json&&) = default;

  auto deserialize(std::istream& input) const -> std::shared_ptr<node_base> override;
};
} // namespace yoyo::io
