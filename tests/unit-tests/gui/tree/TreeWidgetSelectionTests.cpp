#include "TreeWidgetFixture.hpp"

class TreeWidgetSelectionTest : public TreeWidgetTest
{
public:
  auto setupDataTree() -> void
  {
    TreeWidgetTest::SetUp();
    createWidget();
    retrieveListView();
    ASSERT_NE(view, nullptr);

    configuration = setupConfiguration();
    {
      addChild(configuration->childAt(0), 0, 0, 1, "group1");
      addChild(configuration->childAt(0), 1, 0, 1, "group2");
      addChild(configuration->childAt(0)->childAt(0), 0, 1, 1, "aBit");
      addChild(configuration->childAt(0)->childAt(0), 1, 2, 1, "aString");
      addChild(configuration->childAt(0)->childAt(1), 0, 3, 1, "anInteger");
      addChild(configuration->childAt(0)->childAt(1), 1, 4, 1, "unsignedInteger");
    }
    _widget->setConfiguration(configuration);
    view->expandAll();
    selection_model = view->selectionModel();
    ASSERT_NE(selection_model, nullptr);
    model = view->model();
    ASSERT_NE(model, nullptr);
  }

  auto setupGuiTree() -> void
  {
    createWidget();
    retrieveListView();
    ASSERT_NE(view, nullptr);

    configuration = setupConfiguration();
    {
      addChild(configuration->childAt(1), 0, 0, 2, "group1");
      addChild(configuration->childAt(1), 1, 0, 2, "group2");
      addChild(configuration->childAt(1)->childAt(0), 0, 1, 2, "aButton");
      addChild(configuration->childAt(1)->childAt(0), 1, 2, 2, "aCheckbox");
      addChild(configuration->childAt(1)->childAt(1), 0, 3, 2, "aComboBox");
      addChild(configuration->childAt(1)->childAt(1), 1, 4, 2, "aLineEdit");
    }
    _widget->setConfiguration(configuration);
    view->expandAll();
    selection_model = view->selectionModel();
    ASSERT_NE(selection_model, nullptr);
    model = view->model();
    ASSERT_NE(model, nullptr);
  }

  std::shared_ptr<yoyo::node_base> configuration;
  QItemSelectionModel* selection_model { nullptr };
  QAbstractItemModel* model { nullptr };
};

struct SelectionReceiver {
  MOCK_METHOD(void, itemSelected, (std::shared_ptr<yoyo::node_base> item));
};

TEST_F(TreeWidgetSelectionTest, treeSelectData)
{
  setupDataTree();
  testing::StrictMock<SelectionReceiver> receiver;

  QObject::connect(_widget.get(), &yoyo::gui::tree_widget::itemSelected,
                   [&receiver](auto i) { receiver.itemSelected(i); });

  EXPECT_EQ(selection_model->selectedIndexes().count(), 0);

  EXPECT_CALL(receiver, itemSelected(configuration->childAt(0)->childAt(0)));
  selection_model->select(model->index(0, 0, model->index(0, 0)), QItemSelectionModel::Select);

  EXPECT_CALL(receiver, itemSelected(configuration->childAt(0)->childAt(1)));
  selection_model->select(model->index(1, 0, model->index(0, 0)), QItemSelectionModel::Select);

  EXPECT_CALL(receiver, itemSelected(configuration->childAt(0)->childAt(0)->childAt(0)));
  selection_model->select(model->index(0, 0, model->index(0, 0, model->index(0, 0))),
                          QItemSelectionModel::Select);

  EXPECT_CALL(receiver, itemSelected(configuration->childAt(0)->childAt(0)->childAt(1)));
  selection_model->select(model->index(1, 0, model->index(0, 0, model->index(0, 0))),
                          QItemSelectionModel::Select);

  EXPECT_CALL(receiver, itemSelected(configuration->childAt(0)->childAt(1)->childAt(0)));
  selection_model->select(model->index(0, 0, model->index(1, 0, model->index(0, 0))),
                          QItemSelectionModel::Select);
}

TEST_F(TreeWidgetSelectionTest, treeSelectGui)
{
  setupGuiTree();
  testing::StrictMock<SelectionReceiver> receiver;

  QObject::connect(_widget.get(), &yoyo::gui::tree_widget::itemSelected,
                   [&receiver](auto i) { receiver.itemSelected(i); });

  EXPECT_EQ(selection_model->selectedIndexes().count(), 0);

  EXPECT_CALL(receiver, itemSelected(configuration->childAt(1)->childAt(0)));
  selection_model->select(model->index(0, 0, model->index(1, 0)), QItemSelectionModel::Select);

  EXPECT_CALL(receiver, itemSelected(configuration->childAt(1)->childAt(1)));
  selection_model->select(model->index(1, 0, model->index(1, 0)), QItemSelectionModel::Select);

  EXPECT_CALL(receiver, itemSelected(configuration->childAt(1)->childAt(0)->childAt(0)));
  selection_model->select(model->index(0, 0, model->index(0, 0, model->index(1, 0))),
                          QItemSelectionModel::Select);

  EXPECT_CALL(receiver, itemSelected(configuration->childAt(1)->childAt(0)->childAt(1)));
  selection_model->select(model->index(1, 0, model->index(0, 0, model->index(1, 0))),
                          QItemSelectionModel::Select);

  EXPECT_CALL(receiver, itemSelected(configuration->childAt(1)->childAt(1)->childAt(0)));
  selection_model->select(model->index(0, 0, model->index(1, 0, model->index(1, 0))),
                          QItemSelectionModel::Select);
}

TEST_F(TreeWidgetSelectionTest, itemSelectGui)
{
  setupGuiTree();
  testing::StrictMock<SelectionReceiver> receiver;
  QObject lifetimer;

  QObject::connect(_widget.get(), &yoyo::gui::tree_widget::itemSelected, &lifetimer,
                   [&receiver](auto i) { receiver.itemSelected(i); });

  EXPECT_EQ(selection_model->selectedIndexes().count(), 0);
  EXPECT_CALL(receiver, itemSelected(std::shared_ptr<yoyo::node_base> {})).Times(5);

  auto item = configuration->childAt(1)->childAt(0);
  EXPECT_CALL(receiver, itemSelected(item));
  std::dynamic_pointer_cast<yoyo::gui_node>(item)->itemSelected(item);

  item = configuration->childAt(1)->childAt(1);
  EXPECT_CALL(receiver, itemSelected(item));
  std::dynamic_pointer_cast<yoyo::gui_node>(item)->itemSelected(item);

  item = configuration->childAt(1)->childAt(0)->childAt(0);
  EXPECT_CALL(receiver, itemSelected(item));
  std::dynamic_pointer_cast<yoyo::gui_node>(item)->itemSelected(item);

  item = configuration->childAt(1)->childAt(0)->childAt(1);
  EXPECT_CALL(receiver, itemSelected(item));
  std::dynamic_pointer_cast<yoyo::gui_node>(item)->itemSelected(item);

  item = configuration->childAt(1)->childAt(1)->childAt(0);
  EXPECT_CALL(receiver, itemSelected(item));
  std::dynamic_pointer_cast<yoyo::gui_node>(item)->itemSelected(item);

  item = configuration->childAt(1)->childAt(1)->childAt(1);
  EXPECT_CALL(receiver, itemSelected(item));
  std::dynamic_pointer_cast<yoyo::gui_node>(item)->itemSelected(item);
}
