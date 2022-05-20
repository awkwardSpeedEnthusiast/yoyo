#pragma once
#include "yoyo/node_base.h"

namespace yoyo
{
namespace utilities
{
enum class path_strategy_t { NAME, INDEX };
template <path_strategy_t T>
auto calculatePath(std::shared_ptr<node_base> node) -> QString;

template <path_strategy_t T>
auto retrieveFromPath(QString const& path, std::shared_ptr<node_base> root)
  -> std::shared_ptr<node_base>;
} // namespace utilities
} // namespace yoyo
