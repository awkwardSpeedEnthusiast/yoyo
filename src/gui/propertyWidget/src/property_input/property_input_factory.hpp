#pragma once
#include "property_input.hpp"

#include <string>

namespace yoyo
{
class node_base;
} // namespace yoyo

namespace yoyo::gui
{
struct property {
  using creator = std::function<std::shared_ptr<property_input>(std::shared_ptr<node_base>,
                                                                std::string, QWidget*)>;
  static std::map<std::string, creator> const factory;
};
} // namespace yoyo::gui
