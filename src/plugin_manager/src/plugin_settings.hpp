#pragma once
#include "export_on_test.hpp"

#include <boost/filesystem.hpp>

#include <map>
#include <vector>

namespace yoyo::plugin
{
auto YOYO_TESTING_EXPORT read_settings() -> std::vector<std::pair<boost::filesystem::path, bool>>;
auto YOYO_TESTING_EXPORT
update_settings(std::vector<std::pair<boost::filesystem::path, bool>> paths) -> void;
} // namespace yoyo::plugin
