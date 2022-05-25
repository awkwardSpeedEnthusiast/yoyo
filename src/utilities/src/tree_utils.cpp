#include "tree_utils.hpp"

namespace yoyo::utilities
{
template <path_strategy_t T>
auto calculatePath(std::shared_ptr<node_base> node) -> QString
{
  if (!node || node->parent().expired()) {
    return {};
  }

  if constexpr (T == path_strategy_t::NAME) {
    if ((node->type() == "Signals") || (node->type() == "Layout")) {
      return {};
    }
    if (auto parent = node->parent().lock()) {
      if (parent->type() == "Signals" || (parent->type() == "Layout")) {
        return node->name();
      }

      return calculatePath<T>(parent) + "." + node->name();
    }
  } else {
    if (auto parent = node->parent().lock()) {
      if (parent->type() == "Configuration") {
        return QString { "item_%1" }.arg(parent->childIndex(node));
      }

      return calculatePath<T>(parent) + QString { ".item_%1" }.arg(parent->childIndex(node));
    }
  }

  return {};
}

template <path_strategy_t T>
std::shared_ptr<yoyo::node_base> findChildOfName(QString const& path,
                                                 std::shared_ptr<yoyo::node_base> parent)
{
  if (!parent) {
    return {};
  }

  yoyo::node_base::const_iterator it = parent->end();
  if constexpr (T == path_strategy_t::NAME) {
    it = std::find_if(parent->begin(), parent->end(),
                      [&path](auto child) { return child->name() == path; });
  } else if constexpr (T == path_strategy_t::INDEX) {
    it = parent->begin() + path.rightRef(path.length() - 5).toUInt();
  }
  if (it != parent->end()) {
    return *it;
  }

  return {};
}

template <path_strategy_t T>
auto retrieveFromPath(QString const& path, std::shared_ptr<node_base> root)
  -> std::shared_ptr<node_base>
{
  if (!root) {
    return {};
  }

  if (!path.contains(".")) {
    return findChildOfName<T>(path, root);
  }

  QString childname = path.section('.', 0, 0);
  QString rest = path.section('.', 1);
  return retrieveFromPath<T>(rest, findChildOfName<T>(childname, root));
}

template auto calculatePath<path_strategy_t::NAME>(std::shared_ptr<node_base> node) -> QString;
template auto calculatePath<path_strategy_t::INDEX>(std::shared_ptr<node_base> node) -> QString;

template auto retrieveFromPath<path_strategy_t::NAME>(QString const& path,
                                                      std::shared_ptr<node_base> root)
  -> std::shared_ptr<node_base>;
template auto retrieveFromPath<path_strategy_t::INDEX>(QString const& path,
                                                       std::shared_ptr<node_base> root)
  -> std::shared_ptr<node_base>;

} // namespace yoyo::utilities
