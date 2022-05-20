#pragma once
#include "connection_export.h"

#include "yoyo/node_base.h"

namespace yoyo
{
class YOYO_CONNECTION_SHARED_EXPORT connectivity_manager final
{
public:
  explicit connectivity_manager(std::shared_ptr<node_base> root);
  ~connectivity_manager();

private:
  class impl;
  std::unique_ptr<impl> _p;
};
} // namespace yoyo
