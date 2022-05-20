#pragma once
#include "export_on_test.hpp"
#include "plugin_data.h"

namespace yoyo::plugin
{
std::shared_ptr<plugin_data> YOYO_TESTING_EXPORT read(boost::filesystem::path const& path);
} // namespace yoyo::plugin
