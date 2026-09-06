#include "plugin_data_reader.hpp"
#include "plugin_manager.h"
#include "plugin_settings.hpp"

#include "qt_core_fixture.hpp"

#include "yoyo/node_factory.h"

#include <QCoreApplication>
#include <QSettings>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/filesystem.hpp>
#include <boost/uuid/string_generator.hpp>

#include <filesystem>

namespace
{
constexpr auto const test_plugin_id =
  boost::uuids::string_generator {}("2730bf79-0630-4a0b-8011-27d6dea0c363");
constexpr auto const plugin_extension =
#ifdef __linux
  ".so"
#else
  ".dll"
#endif
  ;
constexpr auto const plugin_prefix =
#ifdef __linux
  "lib"
#else
  ""
#endif
  ;

} // namespace

class PluginManagerTests : public yoyo::test::qtCoreFixture
{
public:
  auto SetUp() -> void override
  {
    yoyo::test::qtCoreFixture::SetUp();

    QSettings settings { QSettings::IniFormat, QSettings::UserScope,
                         QCoreApplication::organizationName(),
                         QCoreApplication::applicationName() };
    setting_file_name = settings.fileName().toStdString();
  }

  std::string setting_file_name;
};

using namespace std::string_literals;

TEST_F(PluginManagerTests, settingsTest)
{
  // empty settings
  if (std::filesystem::exists(setting_file_name)) {
    std::filesystem::remove(setting_file_name);
  }

  auto result = yoyo::plugin::read_settings();
  EXPECT_THAT(result, testing::IsEmpty());

  // write some
  result.push_back(std::make_pair(
    boost::filesystem::path { "C:\\Users\\SomeUser\\Documents\\plugins\\plugin1.json" }, true));
  result.push_back(std::make_pair(
    boost::filesystem::path { "/c/Users/SomeUser/Documents/plugins/plugin2.json" }, false));
  result.push_back(std::make_pair(
    boost::filesystem::path { "/home/SomeUser/Documents/plugins/plugin3.json" }, false));
  yoyo::plugin::update_settings(result);

  // read again
  auto actual = yoyo::plugin::read_settings();
  EXPECT_EQ(actual, result);
}

TEST_F(PluginManagerTests, pluginDescription)
{
  auto data = yoyo::plugin::read("/this/is/a/bad/path.json");
  EXPECT_EQ(data, nullptr);

  auto test_plugin_path =
    boost::filesystem::current_path().parent_path() / "test_plugins" / "test_plugin_a";
  EXPECT_TRUE(boost::filesystem::exists(test_plugin_path / "test_plugin_description.json"));
  EXPECT_TRUE(boost::filesystem::exists(test_plugin_path
                                        / (plugin_prefix + "test-plugin-a"s + plugin_extension)));
  data = yoyo::plugin::read(test_plugin_path / "test_plugin_description.json");
  ASSERT_NE(data, nullptr);

  EXPECT_EQ(data->id(), test_plugin_id);
  EXPECT_EQ(data->name(), "test plugin A");
  EXPECT_EQ(data->description(), "This is a A plugin for testing purposes.");
  EXPECT_EQ(data->description_location(), test_plugin_path / "test_plugin_description.json");
  EXPECT_EQ(data->location(),
            test_plugin_path / (plugin_prefix + "test-plugin-a"s + plugin_extension));
  EXPECT_THAT(data->depends_on(), testing::IsEmpty());
}

TEST_F(PluginManagerTests, testPlugin)
{
  if (std::filesystem::exists(setting_file_name)) {
    std::filesystem::remove(setting_file_name);
  }

  auto gui = std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject);
  auto data_f = std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject);
  auto protocol = std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject);
  auto communication = std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject);

  auto test_plugin_path =
    boost::filesystem::current_path().parent_path() / "test_plugins" / "test_plugin_a";
  {
    yoyo::plugin::plugin_manager manager { data_f, gui, protocol, communication };

    auto data = manager.read_plugin_description(test_plugin_path / "test_plugin_description.json");
    EXPECT_EQ(data->id(), test_plugin_id);
    EXPECT_EQ(data->name(), "test plugin A");
    EXPECT_EQ(data->description(), "This is a A plugin for testing purposes.");
    EXPECT_EQ(data->description_location(), test_plugin_path / "test_plugin_description.json");
    EXPECT_EQ(data->location(),
              test_plugin_path / (plugin_prefix + "test-plugin-a"s + plugin_extension));
    EXPECT_THAT(data->depends_on(), testing::IsEmpty());

    EXPECT_THAT(manager.known_plugins(), testing::IsEmpty());

    EXPECT_TRUE(manager.add_plugin(data));
    EXPECT_THAT(manager.known_plugins(), testing::Contains(std::make_pair(test_plugin_id, data)));
    EXPECT_FALSE(manager.is_plugin_loaded(test_plugin_id));
    EXPECT_THAT(gui->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(data_f->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(protocol->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(communication->installed_nodes(), testing::IsEmpty());
    // cannot be added twice
    EXPECT_FALSE(manager.add_plugin(data));
  }
  {
    yoyo::plugin::plugin_manager manager { data_f, gui, protocol, communication };
    EXPECT_THAT(manager.known_plugins(), testing::SizeIs(1));
    EXPECT_FALSE(manager.is_plugin_loaded(test_plugin_id));
    EXPECT_THAT(gui->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(data_f->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(protocol->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(communication->installed_nodes(), testing::IsEmpty());

    EXPECT_TRUE(manager.load_plugin(test_plugin_id));
    EXPECT_THAT(manager.known_plugins(), testing::SizeIs(1));
    EXPECT_TRUE(manager.is_plugin_loaded(test_plugin_id));
    EXPECT_THAT(gui->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(data_f->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(protocol->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(communication->installed_nodes(), testing::SizeIs(1));

    // Cannot be loaded twice
    EXPECT_FALSE(manager.load_plugin(test_plugin_id));
  }
  EXPECT_THAT(gui->installed_nodes(), testing::IsEmpty());
  EXPECT_THAT(data_f->installed_nodes(), testing::IsEmpty());
  EXPECT_THAT(protocol->installed_nodes(), testing::IsEmpty());
  EXPECT_THAT(communication->installed_nodes(), testing::IsEmpty());
  {
    yoyo::plugin::plugin_manager manager { data_f, gui, protocol, communication };
    EXPECT_THAT(manager.known_plugins(), testing::SizeIs(1));
    EXPECT_TRUE(manager.is_plugin_loaded(test_plugin_id));
    EXPECT_THAT(gui->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(data_f->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(protocol->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(communication->installed_nodes(), testing::SizeIs(1));

    EXPECT_TRUE(manager.unload_plugin(test_plugin_id));
    EXPECT_THAT(manager.known_plugins(), testing::SizeIs(1));
    EXPECT_FALSE(manager.is_plugin_loaded(test_plugin_id));
    EXPECT_THAT(gui->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(data_f->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(protocol->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(communication->installed_nodes(), testing::IsEmpty());

    // Cannot be unloaded if not loaded
    EXPECT_FALSE(manager.unload_plugin(test_plugin_id));
  }
  {
    yoyo::plugin::plugin_manager manager { data_f, gui, protocol, communication };
    EXPECT_THAT(manager.known_plugins(), testing::SizeIs(1));
    EXPECT_FALSE(manager.is_plugin_loaded(test_plugin_id));

    EXPECT_TRUE(manager.remove_plugin(test_plugin_id));
    EXPECT_THAT(manager.known_plugins(), testing::IsEmpty());

    EXPECT_FALSE(manager.is_plugin_loaded(test_plugin_id));
    // Cannot be removed twice
    EXPECT_FALSE(manager.remove_plugin(test_plugin_id));
  }
  {
    yoyo::plugin::plugin_manager manager { data_f, gui, protocol, communication };
    EXPECT_THAT(manager.known_plugins(), testing::IsEmpty());
    EXPECT_TRUE(manager.add_plugin(
      manager.read_plugin_description(test_plugin_path / "test_plugin_description.json")));

    EXPECT_THAT(manager.known_plugins(), testing::SizeIs(1));
    EXPECT_FALSE(manager.is_plugin_loaded(test_plugin_id));
    EXPECT_THAT(gui->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(data_f->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(protocol->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(communication->installed_nodes(), testing::IsEmpty());

    EXPECT_TRUE(manager.load_plugin(test_plugin_id));
    EXPECT_TRUE(manager.is_plugin_loaded(test_plugin_id));

    EXPECT_TRUE(manager.remove_plugin(test_plugin_id));
    EXPECT_THAT(gui->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(data_f->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(protocol->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(communication->installed_nodes(), testing::IsEmpty());
  }
}
