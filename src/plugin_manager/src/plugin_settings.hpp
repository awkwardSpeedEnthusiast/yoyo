#pragma once
#include "pluginmanager_export.h"

#include <boost/filesystem.hpp>

#include <map>
#include <vector>

namespace yoyo::plugin
{
auto YOYO_PLUGIN_MANAGER_SHARED_EXPORT read_settings()
  -> std::vector<std::pair<boost::filesystem::path, bool>>;
auto YOYO_PLUGIN_MANAGER_SHARED_EXPORT
update_settings(std::vector<std::pair<boost::filesystem::path, bool>> paths) -> void;
} // namespace yoyo::plugin
