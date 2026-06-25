#pragma once
#include "plugin_data.h"

namespace yoyo::plugin
{
std::shared_ptr<plugin_data>
  YOYO_PLUGIN_MANAGER_SHARED_EXPORT read(boost::filesystem::path const& path);
} // namespace yoyo::plugin
