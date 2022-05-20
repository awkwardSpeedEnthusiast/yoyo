#pragma once
#include "pluginmanager_export.h"

#include <QString>

#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>

#include <memory>
#include <set>
#include <string>

namespace yoyo::plugin
{
class YOYO_PLUGIN_MANAGER_SHARED_EXPORT plugin_data
{
public:
  auto id() const -> boost::uuids::uuid;
  auto name() const -> QString;
  auto description() const -> QString;
  auto location() const -> boost::filesystem::path;
  auto description_location() const -> boost::filesystem::path;
  auto depends_on() const -> std::set<boost::uuids::uuid>;

public:
  struct YOYO_PLUGIN_MANAGER_SHARED_EXPORT builder {
    builder(boost::uuids::uuid id);
    auto name(QString const& n) -> builder&;
    auto description(QString const& n) -> builder&;
    auto location(boost::filesystem::path const& p) -> builder&;
    auto description_location(boost::filesystem::path const& p) -> builder&;
    auto depend_on(boost::uuids::uuid const& id) -> builder&;
    auto build() -> std::shared_ptr<plugin_data>;

  private:
    boost::uuids::uuid _id;
    QString _name;
    QString _description;
    boost::filesystem::path _location;
    boost::filesystem::path _description_location;
    std::set<boost::uuids::uuid> _depends_on;
  };

private:
  plugin_data(boost::uuids::uuid id, QString name, QString description,
              boost::filesystem::path location, boost::filesystem::path description_location,
              std::set<boost::uuids::uuid> depends_on);

private:
  boost::uuids::uuid const _id;
  QString const _name;
  QString const _description;
  boost::filesystem::path const _location;
  boost::filesystem::path const _description_location;
  std::set<boost::uuids::uuid> const _depends_on;
  friend struct builder;
};
} // namespace yoyo::plugin
