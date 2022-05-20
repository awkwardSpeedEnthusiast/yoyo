#include "plugin_settings.hpp"

#include <QCoreApplication>
#include <QSettings>

namespace
{
constexpr auto const key_group = "plugin_descriptions";
constexpr auto const key_file = "file";
constexpr auto const key_loaded = "loaded";
} // namespace
namespace yoyo::plugin
{
auto read_settings() -> std::vector<std::pair<boost::filesystem::path, bool>>
{
  std::vector<std::pair<boost::filesystem::path, bool>> result;
  QSettings settings { QSettings::IniFormat, QSettings::UserScope,
                       QCoreApplication::organizationName(), QCoreApplication::applicationName() };

  int size = settings.beginReadArray(key_group);

  for (int i = 0; i < size; ++i) {
    settings.setArrayIndex(i);
    auto file = settings.value(key_file).toString().toStdString();
    auto loaded = settings.value(key_loaded).toBool();
    result.push_back(std::make_pair(boost::filesystem::path { file }, loaded));
  }

  settings.endArray();
  return result;
}

auto update_settings(std::vector<std::pair<boost::filesystem::path, bool>> paths) -> void
{
  QSettings settings { QSettings::IniFormat, QSettings::UserScope,
                       QCoreApplication::organizationName(), QCoreApplication::applicationName() };
  settings.beginWriteArray(key_group);

  for (size_t i = 0; i < paths.size(); ++i) {
    settings.setArrayIndex(i);
    settings.setValue(key_file, QString::fromStdString(paths[i].first.string<std::string>()));
    settings.setValue(key_loaded, paths[i].second);
  }

  settings.endArray();
}
} // namespace yoyo::plugin
