#include "contextmenu_operator.hpp"
#include "dialog_operator.hpp"
#include "guiFixture.hpp"
#include "gui_simulator.hpp"

#include "plugin_manager.h"
#include "plugin_widget.h"
#include "yoyo/node_factory.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QListView>
#include <QMenu>
#include <QPushButton>
#include <QSettings>

#include <string>

using std::string_literals::operator""s;
using std::chrono_literals::operator""ms;

class PluginWidgetTest : public yoyo::test::guiFixture
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

    QApplication::setOrganizationName("PluginWidgetTest");
    QApplication::setOrganizationDomain("PluginWidgetTest");
    QApplication::setApplicationName("PluginWidgetTest");
    QApplication::setApplicationVersion("0.0.1");
  }

  auto SetUp() -> void override
  {
    QSettings settings { QSettings::IniFormat, QSettings::UserScope,
                         QApplication::organizationName(), QApplication::applicationName() };
    auto setting_file_name = settings.fileName().toStdString();
    if (boost::filesystem::exists(setting_file_name)) {
      boost::filesystem::remove(setting_file_name);
    }

    _factories.push_back(std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject));
    _factories.push_back(std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject));
    _factories.push_back(std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject));
    _factories.push_back(std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject));
    _manager = std::make_shared<yoyo::plugin::plugin_manager>(_factories[0], _factories[1],
                                                              _factories[2], _factories[3]);

    _test_plugin_path = _test_plugin_path.parent_path();
    _test_plugin_path = _test_plugin_path.parent_path();
    _test_plugin_path = _test_plugin_path / "test_plugins"s;
  }

  auto createWidget() -> void
  {
    _widget = std::make_shared<yoyo::gui::plugin_widget>(_manager);
    _widget->resize(200, 200);
    _widget->show();

    ASSERT_EQ(_widget->children().count(), 7);
    ASSERT_EQ(_widget->children()[0]->metaObject()->className(), "QMenu"s);
    ASSERT_EQ(_widget->children()[1]->metaObject()->className(), "QAction"s);
    ASSERT_EQ(_widget->children()[2]->metaObject()->className(), "QAction"s);
    ASSERT_EQ(_widget->children()[3]->metaObject()->className(), "QAction"s);
    ASSERT_EQ(_widget->children()[4]->metaObject()->className(), "QVBoxLayout"s);
    ASSERT_EQ(_widget->children()[5]->metaObject()->className(), "QListView"s);
    ASSERT_EQ(_widget->children()[6]->metaObject()->className(), "QPushButton"s);

    _listView = dynamic_cast<QListView*>(_widget->children()[5]);
    ASSERT_NE(_listView, nullptr);
    _model = _listView->model();
    _menu = dynamic_cast<QMenu*>(_widget->children()[0]);
  }

  auto indexPoint(QWidget* to, QModelIndex index) -> QPoint
  {
    auto r = _listView->visualRect(index);
    return _listView->viewport()->mapTo(to, { r.x() + r.width() / 2, r.y() + r.height() / 2 });
  }

  std::shared_ptr<yoyo::plugin::plugin_manager> _manager;
  std::shared_ptr<yoyo::gui::plugin_widget> _widget;
  std::vector<std::shared_ptr<yoyo::node_factory>> _factories;
  QListView* _listView;
  QAbstractItemModel* _model;
  QMenu* _menu;

  boost::uuids::uuid _plugin_a_id { 0x27, 0x30, 0xbf, 0x79, 0x06, 0x30, 0x4a, 0x0b,
                                    0x80, 0x11, 0x27, 0xd6, 0xde, 0xa0, 0xc3, 0x63 };
  boost::uuids::uuid _plugin_b_id { 0xc3, 0x89, 0x66, 0xac, 0xbd, 0xe6, 0x4c, 0x83,
                                    0x8a, 0x6f, 0xdf, 0x7a, 0xf9, 0x57, 0x96, 0x2b };
  boost::filesystem::path _test_plugin_path { boost::filesystem::current_path() };

  static char* arg0;
  static int argc;
  static std::unique_ptr<QApplication> app;
};

std::unique_ptr<QApplication> PluginWidgetTest::app;
char* PluginWidgetTest::arg0 = new char[7];
int PluginWidgetTest::argc = 1;

TEST_F(PluginWidgetTest, empty)
{
  createWidget();

  EXPECT_EQ(_model->rowCount(), 0);
}

TEST_F(PluginWidgetTest, loadUnloadModel)
{
  {
    auto data = _manager->read_plugin_description(_test_plugin_path / "test_plugin_a"
                                                  / "test_plugin_description.json");
    ASSERT_NE(data, nullptr);
    _manager->add_plugin(data);
  }
  {
    auto data = _manager->read_plugin_description(_test_plugin_path / "test_plugin_b"
                                                  / "test_plugin_description.json");
    ASSERT_NE(data, nullptr);
    _manager->add_plugin(data);
  }
  createWidget();

  EXPECT_EQ(_model->rowCount(), 2);

  EXPECT_EQ(_model->data(_model->index(0, 0), Qt::DisplayRole).toString().toStdString(),
            "test plugin A");
  EXPECT_EQ(_model->data(_model->index(1, 0), Qt::DisplayRole).toString().toStdString(),
            "test plugin B");
  EXPECT_EQ(_model->data(_model->index(0, 0), Qt::ToolTipRole).toString().toStdString(),
            "This is a A plugin for testing purposes.");
  EXPECT_EQ(_model->data(_model->index(1, 0), Qt::ToolTipRole).toString().toStdString(),
            "This is a B plugin for testing purposes.");
  EXPECT_EQ(_model->data(_model->index(0, 0), Qt::CheckStateRole).toBool(), false);
  EXPECT_EQ(_model->data(_model->index(1, 0), Qt::CheckStateRole).toBool(), false);

  // load plugin A with check-box
  {
    EXPECT_THAT(_factories[0]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[1]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[2]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[3]->installed_nodes(), testing::IsEmpty());
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_a_id));
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_b_id));

    EXPECT_FALSE(_model->setData(_model->index(0, 0), "Test", Qt::DisplayRole));
    EXPECT_FALSE(_model->setData(_model->index(0, 0), "Test", Qt::ToolTipRole));
    EXPECT_FALSE(_model->setData(_model->index(0, 0), false, Qt::CheckStateRole));
    EXPECT_TRUE(_model->setData(_model->index(0, 0), true, Qt::CheckStateRole));

    EXPECT_EQ(_model->data(_model->index(0, 0), Qt::CheckStateRole).toBool(), true);
    EXPECT_EQ(_model->data(_model->index(0, 0), Qt::DisplayRole).toString().toStdString(),
              "test plugin A");
    EXPECT_EQ(_model->data(_model->index(0, 0), Qt::ToolTipRole).toString().toStdString(),
              "This is a A plugin for testing purposes.");
    EXPECT_THAT(_factories[0]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[1]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[2]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[3]->installed_nodes(), testing::SizeIs(1));
    EXPECT_TRUE(_manager->is_plugin_loaded(_plugin_a_id));
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_b_id));

    EXPECT_FALSE(_model->setData(_model->index(0, 0), true, Qt::CheckStateRole));
  }
  // load plugin B with check-box
  {
    EXPECT_THAT(_factories[0]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[1]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[2]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[3]->installed_nodes(), testing::SizeIs(1));

    EXPECT_FALSE(_model->setData(_model->index(1, 0), "Test", Qt::DisplayRole));
    EXPECT_FALSE(_model->setData(_model->index(1, 0), "Test", Qt::ToolTipRole));
    EXPECT_FALSE(_model->setData(_model->index(1, 0), false, Qt::CheckStateRole));
    EXPECT_TRUE(_model->setData(_model->index(1, 0), true, Qt::CheckStateRole));

    EXPECT_EQ(_model->data(_model->index(1, 0), Qt::CheckStateRole).toBool(), true);
    EXPECT_EQ(_model->data(_model->index(1, 0), Qt::DisplayRole).toString().toStdString(),
              "test plugin B");
    EXPECT_EQ(_model->data(_model->index(1, 0), Qt::ToolTipRole).toString().toStdString(),
              "This is a B plugin for testing purposes.");
    EXPECT_THAT(_factories[0]->installed_nodes(), testing::SizeIs(2));
    EXPECT_THAT(_factories[1]->installed_nodes(), testing::SizeIs(2));
    EXPECT_THAT(_factories[2]->installed_nodes(), testing::SizeIs(2));
    EXPECT_THAT(_factories[3]->installed_nodes(), testing::SizeIs(2));
    EXPECT_TRUE(_manager->is_plugin_loaded(_plugin_a_id));
    EXPECT_TRUE(_manager->is_plugin_loaded(_plugin_b_id));

    EXPECT_FALSE(_model->setData(_model->index(0, 0), true, Qt::CheckStateRole));
  }
  // unload plugin A with check-box
  {
    EXPECT_TRUE(_model->setData(_model->index(0, 0), false, Qt::CheckStateRole));

    EXPECT_THAT(_factories[0]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[1]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[2]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[3]->installed_nodes(), testing::SizeIs(1));
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_a_id));
    EXPECT_TRUE(_manager->is_plugin_loaded(_plugin_b_id));
  }
  // unload plugin B with check-box
  {
    EXPECT_TRUE(_model->setData(_model->index(1, 0), false, Qt::CheckStateRole));

    EXPECT_THAT(_factories[0]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[1]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[2]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[3]->installed_nodes(), testing::IsEmpty());
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_a_id));
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_b_id));
  }
}

TEST_F(PluginWidgetTest, loadUnloadContext)
{
  {
    auto data = _manager->read_plugin_description(_test_plugin_path / "test_plugin_a"
                                                  / "test_plugin_description.json");
    ASSERT_NE(data, nullptr);
    _manager->add_plugin(data);
  }
  {
    auto data = _manager->read_plugin_description(_test_plugin_path / "test_plugin_b"
                                                  / "test_plugin_description.json");
    ASSERT_NE(data, nullptr);
    _manager->add_plugin(data);
  }
  createWidget();
  // load plugin A with context menu
  {
    EXPECT_THAT(_factories[0]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[1]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[2]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[3]->installed_nodes(), testing::IsEmpty());
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_a_id));
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_b_id));

    yoyo::gui::test::contextmenu_operator { _listView }
      .point(indexPoint(_listView, _model->index(0, 0)))
      .menu(_menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 0, 0, 5ms)
      .operate();

    EXPECT_THAT(_factories[0]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[1]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[2]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[3]->installed_nodes(), testing::SizeIs(1));
    EXPECT_TRUE(_manager->is_plugin_loaded(_plugin_a_id));
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_b_id));
  }
  // unload plugin A with context menu
  {
    yoyo::gui::test::contextmenu_operator { _listView }
      .point(indexPoint(_listView, _model->index(0, 0)))
      .menu(_menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 0, 0, 5ms)
      .operate();
    EXPECT_THAT(_factories[0]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[1]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[2]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[3]->installed_nodes(), testing::IsEmpty());
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_a_id));
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_b_id));
  }

  // load plugin B with context menu
  {
    EXPECT_THAT(_factories[0]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[1]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[2]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[3]->installed_nodes(), testing::IsEmpty());
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_a_id));
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_b_id));

    yoyo::gui::test::contextmenu_operator { _listView }
      .point(indexPoint(_listView, _model->index(1, 0)))
      .menu(_menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 0, 0, 5ms)
      .operate();

    EXPECT_THAT(_factories[0]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[1]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[2]->installed_nodes(), testing::SizeIs(1));
    EXPECT_THAT(_factories[3]->installed_nodes(), testing::SizeIs(1));
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_a_id));
    EXPECT_TRUE(_manager->is_plugin_loaded(_plugin_b_id));
  }
  // unload plugin B with context menu
  {
    yoyo::gui::test::contextmenu_operator { _listView }
      .point(indexPoint(_listView, _model->index(1, 0)))
      .menu(_menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 0, 0, 5ms)
      .operate();
    EXPECT_THAT(_factories[0]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[1]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[2]->installed_nodes(), testing::IsEmpty());
    EXPECT_THAT(_factories[3]->installed_nodes(), testing::IsEmpty());
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_a_id));
    EXPECT_FALSE(_manager->is_plugin_loaded(_plugin_b_id));
  }
}

TEST_F(PluginWidgetTest, remove)
{
  {
    auto data = _manager->read_plugin_description(_test_plugin_path / "test_plugin_a"
                                                  / "test_plugin_description.json");
    ASSERT_NE(data, nullptr);
    _manager->add_plugin(data);
  }
  {
    auto data = _manager->read_plugin_description(_test_plugin_path / "test_plugin_b"
                                                  / "test_plugin_description.json");
    ASSERT_NE(data, nullptr);
    _manager->add_plugin(data);
  }
  createWidget();

  EXPECT_EQ(_model->rowCount(), 2);
  // remove plugin B
  {
    auto datas = _manager->known_plugins();
    EXPECT_NE(datas.find(_plugin_b_id), datas.end());
    yoyo::gui::test::contextmenu_operator { _listView }
      .point(indexPoint(_listView, _model->index(1, 0)))
      .menu(_menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 1, 0, 5ms)
      .operate();

    EXPECT_EQ(_model->rowCount(), 1);
    datas = _manager->known_plugins();
    EXPECT_EQ(datas.find(_plugin_b_id), datas.end());
  }
  // remove plugin A
  {
    auto datas = _manager->known_plugins();
    EXPECT_NE(datas.find(_plugin_a_id), datas.end());
    yoyo::gui::test::contextmenu_operator { _listView }
      .point(indexPoint(_listView, _model->index(0, 0)))
      .menu(_menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 1, 0, 5ms)
      .operate();

    EXPECT_EQ(_model->rowCount(), 0);
    datas = _manager->known_plugins();
    EXPECT_EQ(datas.find(_plugin_a_id), datas.end());
  }
}

TEST_F(PluginWidgetTest, add)
{
  GTEST_SKIP() << "Due to bug https://bugreports.qt.io/browse/QTBUG-47159 in QFileDialog, the test "
                  "can only be executed with manual interaction. Since this is an automated test "
                  "suite, we'll skip the test.";

  createWidget();
  auto button = dynamic_cast<QPushButton*>(_widget->children()[6]);

  yoyo::gui::test::dialog_operator { _widget.get() }
    .openFileDialog(QString::fromStdString(
      (_test_plugin_path / "test_plugin_a" / "test_plugin_description.json").string()))
    .addDialog(
      yoyo::gui::test::single_dialog_interaction { "yoyo::gui::plugin_description_dialog" })
    .operate([button]() { yoyo::gui::test::mouseClick(button, Qt::LeftButton); });
  QTest::qWait(10);
  auto datas = _manager->known_plugins();
  EXPECT_NE(datas.find(_plugin_a_id), datas.end());
}
