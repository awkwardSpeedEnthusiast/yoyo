#include "TreeWidgetFixture.hpp"
#include "contextmenu_operator.hpp"

#include "command_handler.h"

#include <boost/uuid/uuid_io.hpp>
#include <string>

using std::string_literals::operator""s;
using std::chrono_literals::operator""ms;

TEST_F(TreeWidgetTest, noConfig)
{
  createWidget();
  retrieveListView();
  ASSERT_NE(view, nullptr);
  auto model = view->model();
  ASSERT_NE(model, nullptr);
  EXPECT_EQ(model->rowCount(), 0);
  EXPECT_EQ(model->columnCount(), 0);
}

TEST_F(TreeWidgetTest, loadedConfig)
{
  createWidget();
  retrieveListView();
  ASSERT_NE(view, nullptr);
  auto model = view->model();
  ASSERT_NE(model, nullptr);

  auto configuration = setupConfiguration();
  {
    configuration->childAt(0)->addChild(
      _data_factory->createNode(std::get<0>(_data_factory->installed_nodes()[0]), {}));
    configuration->childAt(0)->childAt(0)->setName({ "myGroup", true });
    configuration->childAt(0)->childAt(0)->addChild(
      _data_factory->createNode(std::get<0>(_data_factory->installed_nodes()[1]), {}));
    configuration->childAt(0)->childAt(0)->childAt(0)->setName({ "myData", true });

    configuration->childAt(1)->addChild(
      _gui_factory->createNode(std::get<0>(_gui_factory->installed_nodes()[0]), {}));
    configuration->childAt(1)->childAt(0)->setProperty("title", "aGroup");
    configuration->childAt(1)->childAt(0)->addChild(
      _gui_factory->createNode(std::get<0>(_gui_factory->installed_nodes()[1]), {}));
    configuration->childAt(1)->childAt(0)->childAt(0)->setProperty("title", "myGui");
  }
  _widget->setConfiguration(configuration);

  EXPECT_EQ(model->rowCount(), 2);
  EXPECT_EQ(model->columnCount(), 4);
  EXPECT_EQ(model->rowCount(model->index(0, 0)), 1);
  EXPECT_EQ(model->rowCount(model->index(1, 0)), 1);
  EXPECT_EQ(model->data(model->index(0, 0)).toString(), "Signals");
  EXPECT_EQ(model->data(model->index(1, 0)).toString(), "Layout");

  {
    EXPECT_EQ(model->rowCount(model->index(0, 0)), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0))).toString(), "myGroup");
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0))).toString(), "group");
    EXPECT_EQ(model->data(model->index(0, 2, model->index(0, 0))).toString(), "");

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0)))).toString(),
              "myData");
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "bit");
    EXPECT_EQ(model->data(model->index(0, 2, model->index(0, 0, model->index(0, 0)))).toString(),
              "");

    EXPECT_EQ(model->rowCount(model->index(1, 0)), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0))).toString(), "group");
    EXPECT_EQ(model->data(model->index(0, 1, model->index(1, 0))).toString(), "aGroup");
    EXPECT_EQ(model->data(model->index(0, 2, model->index(1, 0))).toString(), "aGroup");

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "button");
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(1, 0)))).toString(),
              "myGui");
    EXPECT_EQ(model->data(model->index(0, 2, model->index(0, 0, model->index(1, 0)))).toString(),
              "myGui");
  }
}

TEST_F(TreeWidgetTest, addRemove)
{
  createWidget();
  retrieveListView();
  ASSERT_NE(view, nullptr);
  auto model = view->model();
  ASSERT_NE(model, nullptr);

  auto configuration = setupConfiguration();
  _widget->setConfiguration(configuration);

  EXPECT_EQ(model->rowCount(), 2);
  EXPECT_EQ(model->columnCount(), 4);
  EXPECT_EQ(model->rowCount(model->index(0, 0)), 0);
  EXPECT_EQ(model->rowCount(model->index(1, 0)), 0);
  EXPECT_EQ(model->data(model->index(0, 0)).toString(), "Signals");
  EXPECT_EQ(model->data(model->index(1, 0)).toString(), "Layout");

  // add data
  {
    configuration->childAt(0)->addChild(
      _data_factory->createNode(std::get<0>(_data_factory->installed_nodes()[0]), {}));
    configuration->childAt(0)->childAt(0)->setName({ "myGroup", true });

    QApplication::processEvents();
    view->expandAll();
    EXPECT_EQ(model->rowCount(model->index(0, 0)), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0))).toString(), "myGroup");
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0))).toString(), "group");
    EXPECT_EQ(model->data(model->index(0, 2, model->index(0, 0))).toString(), "");

    configuration->childAt(0)->childAt(0)->addChild(
      _data_factory->createNode(std::get<0>(_data_factory->installed_nodes()[1]), {}));
    configuration->childAt(0)->childAt(0)->childAt(0)->setName({ "myData", true });

    QApplication::processEvents();
    view->expandAll();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0)))).toString(),
              "myData");
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "bit");
    EXPECT_EQ(model->data(model->index(0, 2, model->index(0, 0, model->index(0, 0)))).toString(),
              "");
  }

  // add gui
  {
    configuration->childAt(1)->addChild(
      _gui_factory->createNode(std::get<0>(_gui_factory->installed_nodes()[0]), {}));
    configuration->childAt(1)->childAt(0)->setProperty("title", "aGroup");

    QApplication::processEvents();
    view->expandAll();
    EXPECT_EQ(model->rowCount(model->index(1, 0)), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0))).toString(), "group");
    EXPECT_EQ(model->data(model->index(0, 1, model->index(1, 0))).toString(), "aGroup");
    EXPECT_EQ(model->data(model->index(0, 2, model->index(1, 0))).toString(), "aGroup");

    configuration->childAt(1)->childAt(0)->addChild(
      _gui_factory->createNode(std::get<0>(_gui_factory->installed_nodes()[1]), {}));
    configuration->childAt(1)->childAt(0)->childAt(0)->setProperty("title", "myGui");

    QApplication::processEvents();
    view->expandAll();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "button");
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(1, 0)))).toString(),
              "myGui");
    EXPECT_EQ(model->data(model->index(0, 2, model->index(0, 0, model->index(1, 0)))).toString(),
              "myGui");
  }

  // remove data
  {
    configuration->childAt(0)->removeChild(0);
    QApplication::processEvents();
    view->expandAll();
    EXPECT_EQ(model->rowCount(model->index(0, 0)), 0);
  }
  // remove gui
  {
    configuration->childAt(1)->removeChild(0);
    QApplication::processEvents();
    view->expandAll();
    EXPECT_EQ(model->rowCount(model->index(1, 0)), 0);
  }
}

TEST_F(TreeWidgetTest, contextMenuData)
{
  createWidget();
  retrieveListView();
  ASSERT_NE(view, nullptr);
  auto model = view->model();
  ASSERT_NE(model, nullptr);

  auto configuration = setupConfiguration();
  _widget->setConfiguration(configuration);
  view->expandAll();

  EXPECT_EQ(model->rowCount(model->index(0, 0)), 0);
  // add a data-group top level
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(0, 0)))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 0, 3, 5ms)
      .addMenuInteraction(10ms, 0, 0, 5ms)
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(0, 0)), 1);
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0))).toString(), "group");
  }
  // add a bit item to data-group
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(0, 0, model->index(0, 0))))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 0, 3, 5ms)
      .addMenuInteraction(10ms, 1, 0, 5ms)
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "bit");
  }
  // add a string item as sibling after bit item
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(0, 0, model->index(0, 0, model->index(0, 0)))))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 1, 3, 5ms) // items: "sibling before", "sibling after", "remove"
      .addMenuInteraction(10ms, 2, 0, 5ms)
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "bit");
    EXPECT_EQ(model->data(model->index(1, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "string");
  }
  // add a float item as sibling before bit item
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(0, 0, model->index(0, 0, model->index(0, 0)))))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 0, 3, 5ms) // items: "sibling before", "sibling after", "remove"
      .addMenuInteraction(10ms, 3, 0, 5ms)
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 3);
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "int8");
    EXPECT_EQ(model->data(model->index(1, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "bit");
    EXPECT_EQ(model->data(model->index(2, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "string");
  }
  // remove string item
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(2, 1, model->index(0, 0, model->index(0, 0)))))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 2, 0, 5ms) // items: "sibling before", "sibling after", "remove"
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "int8");
    EXPECT_EQ(model->data(model->index(1, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "bit");
  }
  // remove all children of group
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(0, 0, model->index(0, 0))))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(
        10ms, 4, 0,
        5ms) // items: "add child", "sibling before", "sibling after", "remove", "remove children"
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(0, 0)), 1);
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 0);
  }
  // remove group
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(0, 0, model->index(0, 0))))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 3, 0,
                          5ms) // items: "add child", "sibling before", "sibling after", "remove"
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(0, 0)), 0);
  }

  auto handler = yoyo::command::commandhandler();
  { // remove group
    { EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Remove item from Signals");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(0, 0)), 1);
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 0);
}

// remove all children of group
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Remove all children from group");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
            "int8");
  EXPECT_EQ(model->data(model->index(1, 1, model->index(0, 0, model->index(0, 0)))).toString(),
            "bit");
}
// remove string item
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Remove item from group");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 3);
  EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
            "int8");
  EXPECT_EQ(model->data(model->index(1, 1, model->index(0, 0, model->index(0, 0)))).toString(),
            "bit");
  EXPECT_EQ(model->data(model->index(2, 1, model->index(0, 0, model->index(0, 0)))).toString(),
            "string");
}
// add a float item as sibling before bit item
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Add new item to group");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
            "bit");
  EXPECT_EQ(model->data(model->index(1, 1, model->index(0, 0, model->index(0, 0)))).toString(),
            "string");
}
// add a string item as sibling after bit item
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Add new item to group");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
  EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
            "bit");
}
// add a bit item to data-group
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Add new item to group");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(0, 0)), 1);
  EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0))).toString(), "group");
}
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Add new item to Signals");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(0, 0)), 0);
}
}

{
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Add new item to Signals");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(0, 0)), 1);
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0))).toString(), "group");
  }
  // add a bit item to data-group
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Add new item to group");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "bit");
  }
  // add a string item as sibling after bit item
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Add new item to group");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "bit");
    EXPECT_EQ(model->data(model->index(1, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "string");
  }
  // add a float item as sibling before bit item
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Add new item to group");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 3);
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "int8");
    EXPECT_EQ(model->data(model->index(1, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "bit");
    EXPECT_EQ(model->data(model->index(2, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "string");
  }
  // remove string item
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Remove item from group");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "int8");
    EXPECT_EQ(model->data(model->index(1, 1, model->index(0, 0, model->index(0, 0)))).toString(),
              "bit");
  }
  // remove all children of group
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Remove all children from group");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(0, 0)), 1);
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 0);
  }
  // remove group
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Remove item from Signals");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(0, 0)), 0);
  }
}
}

TEST_F(TreeWidgetTest, contextMenuGui)
{
  createWidget();
  retrieveListView();
  ASSERT_NE(view, nullptr);
  auto model = view->model();
  ASSERT_NE(model, nullptr);

  auto configuration = setupConfiguration();
  _widget->setConfiguration(configuration);
  view->expandAll();

  EXPECT_EQ(model->rowCount(model->index(1, 0)), 0);
  // add a gui-group top level
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(1, 0)))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 0, 3, 5ms)
      .addMenuInteraction(10ms, 0, 0, 5ms)
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(1, 0)), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0))).toString(), "group");
  }
  // add a button to group
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(0, 0, model->index(1, 0))))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 0, 3, 5ms)
      .addMenuInteraction(10ms, 1, 0, 5ms)
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "button");
  }
  // add a checkbox as sibling after button
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(0, 0, model->index(0, 0, model->index(1, 0)))))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 1, 3, 5ms) // items: "sibling before", "sibling after", "remove"
      .addMenuInteraction(10ms, 2, 0, 5ms)
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "button");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "check_box");
  }
  // add a combobox as sibling before button
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(0, 0, model->index(0, 0, model->index(1, 0)))))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 0, 3, 5ms) // items: "sibling before", "sibling after", "remove"
      .addMenuInteraction(10ms, 3, 0, 5ms)
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 3);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "combo_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "button");
    EXPECT_EQ(model->data(model->index(2, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "check_box");
  }
  // remove button
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(1, 1, model->index(0, 0, model->index(1, 0)))))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(10ms, 2, 0, 5ms) // items: "sibling before", "sibling after", "remove"
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "combo_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "check_box");
  }
  // remove all children of group
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(0, 0, model->index(1, 0))))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(
        10ms, 4, 0,
        5ms) // items: "add child", "sibling before", "sibling after", "remove", "remove children"
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(1, 0)), 1);
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 0);
  }
  // remove group
  {
    yoyo::gui::test::contextmenu_operator { _widget.get() }
      .point(indexPoint(_widget.get(), model->index(0, 0, model->index(1, 0))))
      .menu(menu)
      .timeoutForMenu(10ms)
      .addMenuInteraction(
        10ms, 3, 0,
        5ms) // items: "add child", "sibling before", "sibling after", "remove", "remove children"
      .operate();
    view->expandAll();

    EXPECT_EQ(model->rowCount(model->index(1, 0)), 0);
  }

  auto handler = yoyo::command::commandhandler();
  { // remove group
    { EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Remove item from Layout");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(1, 0)), 1);
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 0);
}

// remove all children of group
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Remove all children from group");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
            "combo_box");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0)))).toString(),
            "check_box");
}
// remove button
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Remove item from group");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 3);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
            "combo_box");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0)))).toString(),
            "button");
  EXPECT_EQ(model->data(model->index(2, 0, model->index(0, 0, model->index(1, 0)))).toString(),
            "check_box");
}
// add a combobox as sibling before button
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Add new item to group");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
            "button");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0)))).toString(),
            "check_box");
}
// add a checkbox as sibling after button
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Add new item to group");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
            "button");
}
// add a button to group
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Add new item to group");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(1, 0)), 1);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0))).toString(), "group");
}
// add a gui-group top level
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Add new item to Layout");
  handler->undo();

  EXPECT_EQ(model->rowCount(model->index(1, 0)), 0);
}
}

{
  // add a gui-group top level
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Add new item to Layout");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(1, 0)), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0))).toString(), "group");
  }
  // add a button to group
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Add new item to group");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "button");
  }
  // add a checkbox as sibling after button
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Add new item to group");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "button");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "check_box");
  }
  // add a combobox as sibling before button
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Add new item to group");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 3);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "combo_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "button");
    EXPECT_EQ(model->data(model->index(2, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "check_box");
  }
  // remove button
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Remove item from group");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "combo_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0)))).toString(),
              "check_box");
  }
  // remove all children of group
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Remove all children from group");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(1, 0)), 1);
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 0);
  }
  // remove group
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Remove item from Layout");
    handler->redo();

    EXPECT_EQ(model->rowCount(model->index(1, 0)), 0);
  }
}
}
