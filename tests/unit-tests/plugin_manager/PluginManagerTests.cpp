#include "plugin_data_reader.hpp"
#include "plugin_manager.h"
#include "plugin_settings.hpp"

#include "yoyo/node_factory.h"

#include <QApplication>
#include <QSettings>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

namespace
{
constexpr auto const test_plugin_id =
  boost::uuids::uuid { 0x27, 0x30, 0xbf, 0x79, 0x06, 0x30, 0x4a, 0x0b,
                       0x80, 0x11, 0x27, 0xd6, 0xde, 0xa0, 0xc3, 0x63 };
}

class PluginManagerTests : public testing::Test
{
public:
  static auto SetUpTestSuite() -> void
  {
    testing::Test::SetUpTestSuite();

    if (!qApp) {
      strcpy(arg0, "blabla\0");
      char* args[1] = { arg0 };
      app = std::make_unique<QApplication>(argc, args);
    }

    QApplication::setOrganizationName("PluginManagerTest");
    QApplication::setOrganizationDomain("PluginManagerTest");
    QApplication::setApplicationName("PluginManagerTest");
    QApplication::setApplicationVersion("0.0.1");
  }

  auto SetUp() -> void override
  {
    QSettings settings { QSettings::IniFormat, QSettings::UserScope,
                         QApplication::organizationName(), QApplication::applicationName() };
    setting_file_name = settings.fileName().toStdString();
  }

  std::string setting_file_name;

  static char* arg0;
  static int argc;
  static std::unique_ptr<QApplication> app;
};
std::unique_ptr<QApplication> PluginManagerTests::app;
char* PluginManagerTests::arg0 = new char[7];
int PluginManagerTests::argc = 1;

TEST_F(PluginManagerTests, settingsTest)
{
  // empty settings
  if (boost::filesystem::exists(setting_file_name)) {
    boost::filesystem::remove(setting_file_name);
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

  auto test_plugin_path = boost::filesystem::current_path() / "test_plugin";
  EXPECT_TRUE(boost::filesystem::exists(test_plugin_path / "test_plugin_description.json"));
  EXPECT_TRUE(boost::filesystem::exists(test_plugin_path / "test-plugin.dll"));
  data = yoyo::plugin::read(test_plugin_path / "test_plugin_description.json");
  ASSERT_NE(data, nullptr);

  EXPECT_EQ(data->id(), test_plugin_id);
  EXPECT_EQ(data->name(), "test plugin");
  EXPECT_EQ(data->description(), "This is a plugin for testing purposes.");
  EXPECT_EQ(data->description_location(), test_plugin_path / "test_plugin_description.json");
  EXPECT_EQ(data->location(), test_plugin_path / "test-plugin.dll");
  EXPECT_THAT(data->depends_on(), testing::IsEmpty());
}

TEST_F(PluginManagerTests, testPlugin)
{
  if (boost::filesystem::exists(setting_file_name)) {
    boost::filesystem::remove(setting_file_name);
  }

  auto gui = std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject);
  auto data_f = std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject);
  auto protocol = std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject);
  auto communication = std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject);

  auto test_plugin_path = boost::filesystem::current_path() / "test_plugin";
  {
    yoyo::plugin::plugin_manager manager { data_f, gui, protocol, communication };

    auto data = manager.read_plugin_description(test_plugin_path / "test_plugin_description.json");
    EXPECT_EQ(data->id(), test_plugin_id);
    EXPECT_EQ(data->name(), "test plugin");
    EXPECT_EQ(data->description(), "This is a plugin for testing purposes.");
    EXPECT_EQ(data->description_location(), test_plugin_path / "test_plugin_description.json");
    EXPECT_EQ(data->location(), test_plugin_path / "test-plugin.dll");
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
