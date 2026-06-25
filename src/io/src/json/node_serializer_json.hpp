#pragma once
#include "node_serializer.h"

namespace yoyo::io
{
class YOYO_IO_SHARED_EXPORT node_serializer_json final : public node_serializer
{
public:
  using node_serializer::node_serializer;
  virtual ~node_serializer_json() = default;
  node_serializer_json(node_serializer_json const&) = default;
  node_serializer_json(node_serializer_json&&) = default;
  node_serializer_json& operator=(node_serializer_json const&) = default;
  node_serializer_json& operator=(node_serializer_json&&) = default;

  auto serialize(std::ostream& output, std::shared_ptr<node_base> root_node) const -> void override;
};
} // namespace yoyo::io
