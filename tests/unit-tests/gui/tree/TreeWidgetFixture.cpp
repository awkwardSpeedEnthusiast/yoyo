#include "TreeWidgetFixture.hpp"

#include "command_handler.h"
#include "factory_provider.hpp"

#include <string>

using std::string_literals::operator""s;

auto TreeWidgetTest::SetUp() -> void
{
  yoyo::test::guiFixture::SetUp();
  if (!yoyo::command::commandhandler()) {
    yoyo::command::initialize_commandhandler(new yoyo::command::command_handler(100));
  }
  yoyo::command::commandhandler()->setEditMode(true);

  if (!_fundamental_factory) {
    _fundamental_factory = std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject);
    yoyo::install_fundamental_nodes(*_fundamental_factory);
  }
  if (!_data_factory) {
    _data_factory = std::make_shared<yoyo::node_factory>(&yoyo::data_node::staticMetaObject);
    yoyo::install_data_nodes(*_data_factory);
  }
  if (!_gui_factory) {
    _gui_factory = std::make_shared<yoyo::node_factory>(&yoyo::gui_node::staticMetaObject);
    yoyo::install_gui_nodes(*_gui_factory);
  }
  if (!_protocol_factory) {
    _protocol_factory =
      std::make_shared<yoyo::node_factory>(&yoyo::protocol_node::staticMetaObject);
    yoyo::install_protocol_nodes(*_protocol_factory);
  }
  if (!_communication_factory) {
    _communication_factory =
      std::make_shared<yoyo::node_factory>(&yoyo::communication_node::staticMetaObject);
    yoyo::install_communication_nodes(*_communication_factory);
  }
}

auto TreeWidgetTest::TearDown() -> void
{
  yoyo::command::commandhandler()->clearStack();
  if (_widget) {
    _widget->setConfiguration(nullptr);
  }
  _widget.reset();
  EXPECT_EQ(_fundamental_factory.use_count(), 1);
  EXPECT_EQ(_data_factory.use_count(), 1);
  EXPECT_EQ(_gui_factory.use_count(), 1);
  EXPECT_EQ(_protocol_factory.use_count(), 1);
  EXPECT_EQ(_communication_factory.use_count(), 1);
  yoyo::test::guiFixture::TearDown();
}

auto TreeWidgetTest::createWidget() -> void
{
  _widget = std::make_shared<yoyo::gui::tree_widget>(std::vector {
    _fundamental_factory, _data_factory, _gui_factory, _protocol_factory, _communication_factory });
  _widget->resize(50, 200);
  _widget->show();
}

auto TreeWidgetTest::setupConfiguration() -> std::shared_ptr<yoyo::node_base>
{
  auto configuration =
    _fundamental_factory->createNode(std::get<0>(_fundamental_factory->installed_nodes()[0]), {});

  auto data_root =
    _fundamental_factory->createNode(std::get<0>(_fundamental_factory->installed_nodes()[1]), {});
  EXPECT_TRUE(configuration->acceptsChild(data_root));
  configuration->addChild(data_root);

  auto gui_root =
    _fundamental_factory->createNode(std::get<0>(_fundamental_factory->installed_nodes()[2]), {});
  EXPECT_TRUE(configuration->acceptsChild(gui_root));
  configuration->addChild(gui_root);

  return configuration;
}

auto TreeWidgetTest::retrieveListView() -> void
{
  // dock widget stuff
  ASSERT_EQ(_widget->children().count(), 6);
  ASSERT_EQ(_widget->children()[0]->metaObject()->className(), "QDockWidgetLayout"s);
  ASSERT_EQ(_widget->children()[1]->metaObject()->className(), "QDockWidgetTitleButton"s);
  ASSERT_EQ(_widget->children()[2]->metaObject()->className(), "QDockWidgetTitleButton"s);
  ASSERT_EQ(_widget->children()[3]->metaObject()->className(), "QAction"s);
  ASSERT_EQ(_widget->children()[4]->metaObject()->className(), "QMenu"s);
  ASSERT_EQ(_widget->children()[5]->metaObject()->className(), "QWidget"s);

  // one layout, then a combobox and the list view
  ASSERT_EQ(_widget->children()[5]->children().count(), 3);
  ASSERT_EQ(_widget->children()[5]->children()[0]->metaObject()->className(), "QVBoxLayout"s);
  ASSERT_EQ(_widget->children()[5]->children()[1]->metaObject()->className(), "QComboBox"s);
  ASSERT_EQ(_widget->children()[5]->children()[2]->metaObject()->className(), "QTreeView"s);

  view = dynamic_cast<QTreeView*>(_widget->children()[5]->children()[2]);
  menu = dynamic_cast<QMenu*>(_widget->children()[4]);
}

auto TreeWidgetTest::indexPoint(QWidget* to, QModelIndex index) -> QPoint
{
  auto r = view->visualRect(index);
  return view->viewport()->mapTo(to, QPoint { r.x() + r.width() / 2, r.y() + r.height() / 2 });
}

auto TreeWidgetTest::addChild(std::shared_ptr<yoyo::node_base> parent, size_t index,
                              size_t typeIndex, size_t factoryIndex, QString const& name) -> void
{
  std::shared_ptr<yoyo::node_base> new_node;

  switch (factoryIndex) {
  case 1:
    new_node =
      _data_factory->createNode(std::get<0>(_data_factory->installed_nodes()[typeIndex]), {});
    break;
  case 2:
    new_node =
      _gui_factory->createNode(std::get<0>(_gui_factory->installed_nodes()[typeIndex]), {});
    break;
  }

  parent->addChild(new_node, index);
  new_node->setName({ name, true });
}
