#include "TreeWidgetFixture.hpp"
#include "command_handler.h"

#include <QMimeData>

#include <boost/uuid/uuid_io.hpp>

class TreeWidgetDnDTest : public TreeWidgetTest
{
public:
  auto TearDown() -> void override
  {
    if (_widget) {
      _widget->setConfiguration(nullptr);
    }
    if (_configuration) {
      EXPECT_EQ(_configuration.use_count(), 1);
      _configuration.reset();
    }
    TreeWidgetTest::TearDown();
  }
  auto setupDataTree() -> void
  {
    createWidget();
    retrieveListView();
    ASSERT_NE(view, nullptr);

    _configuration = setupConfiguration();
    {
      addChild(_configuration->childAt(0), 0, 0, 1, "group1");
      addChild(_configuration->childAt(0), 1, 0, 1, "group2");
      addChild(_configuration->childAt(0)->childAt(0), 0, 1, 1, "aBit");
      addChild(_configuration->childAt(0)->childAt(0), 1, 2, 1, "aString");
      addChild(_configuration->childAt(0)->childAt(1), 0, 3, 1, "anInteger");
      addChild(_configuration->childAt(0)->childAt(1), 1, 4, 1, "unsignedInteger");
    }
    _widget->setConfiguration(_configuration);
    view->expandAll();
    model = view->model();
    ASSERT_NE(model, nullptr);
  }

  auto setupGuiTree() -> void
  {
    createWidget();
    retrieveListView();
    ASSERT_NE(view, nullptr);

    _configuration = setupConfiguration();
    {
      addChild(_configuration->childAt(1), 0, 0, 2, "group1");
      addChild(_configuration->childAt(1), 1, 0, 2, "group2");
      addChild(_configuration->childAt(1)->childAt(0), 0, 1, 2, "aButton");
      addChild(_configuration->childAt(1)->childAt(0), 1, 2, 2, "aCheckbox");
      addChild(_configuration->childAt(1)->childAt(1), 0, 3, 2, "aComboBox");
      addChild(_configuration->childAt(1)->childAt(1), 1, 4, 2, "aLineEdit");
    }
    _widget->setConfiguration(_configuration);
    view->expandAll();
    model = view->model();
    ASSERT_NE(model, nullptr);
  }

  QAbstractItemModel* model { nullptr };
  std::shared_ptr<yoyo::node_base> _configuration;
};

class TreeWidgetDnDTestActions : public TreeWidgetDnDTest,
                                 public testing::WithParamInterface<Qt::DropAction>
{
};

TEST_F(TreeWidgetDnDTest, dataMimeData)
{
  setupDataTree();

  EXPECT_EQ(model->mimeData({}), nullptr);
  EXPECT_EQ(model->mimeData(
              { model->index(0, 0, model->index(0, 0)), model->index(1, 0, model->index(0, 0)) }),
            nullptr);
  EXPECT_EQ(model->mimeData({ model->index(0, 0) }), nullptr);

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(0, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "aBit");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/data-element"));

  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0, model->index(1, 0)));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0, {}));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::MoveAction, 0, 0, {}));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0, model->index(0, 0)));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::MoveAction, 1, 0, model->index(0, 0)));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0,
                                     model->index(0, 0, model->index(0, 0))));
  EXPECT_TRUE(
    model->canDropMimeData(mimedata, Qt::MoveAction, 1, 0, model->index(0, 0, model->index(0, 0))));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0,
                                     model->index(1, 0, model->index(0, 0, model->index(0, 0)))));

  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, -1, 0, model->index(1, 0)));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, -1, 0, {}));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, 0, 0, {}));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, -1, 0, model->index(0, 0)));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, 1, 0, model->index(0, 0)));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, -1, 0,
                                      model->index(0, 0, model->index(0, 0))));
  EXPECT_FALSE(
    model->canDropMimeData(mimedata, Qt::CopyAction, 1, 0, model->index(0, 0, model->index(0, 0))));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, -1, 0,
                                      model->index(1, 0, model->index(0, 0, model->index(0, 0)))));

  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0, model->index(1, 0)));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0, {}));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::LinkAction, 0, 0, {}));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0, model->index(0, 0)));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::LinkAction, 1, 0, model->index(0, 0)));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0,
                                     model->index(0, 0, model->index(0, 0))));
  EXPECT_TRUE(
    model->canDropMimeData(mimedata, Qt::LinkAction, 1, 0, model->index(0, 0, model->index(0, 0))));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0,
                                     model->index(1, 0, model->index(0, 0, model->index(0, 0)))));
}

///
/// \brief TEST_F
/// Drop on sibling
TEST_P(TreeWidgetDnDTestActions, moveDataSameGroup)
{
  auto action = GetParam();
  setupDataTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(0, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "aBit");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/data-element"));

  EXPECT_TRUE(model->canDropMimeData(mimedata, action, -1, 0,
                                     model->index(1, 0, model->index(0, 0, model->index(0, 0)))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_TRUE(model->dropMimeData(mimedata, action, -1, 0,
                                  model->index(1, 0, model->index(0, 0, model->index(0, 0)))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aBit position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aBit position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
  }
}

///
/// \brief TEST_F
/// Drop below sibling
TEST_P(TreeWidgetDnDTestActions, moveDataGroupSameGroup1)
{
  auto action = GetParam();
  setupDataTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(0, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "aBit");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/data-element"));

  EXPECT_TRUE(
    model->canDropMimeData(mimedata, action, 2, 0, model->index(0, 0, model->index(0, 0))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_TRUE(model->dropMimeData(mimedata, action, 2, 0, model->index(0, 0, model->index(0, 0))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aBit position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aBit position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
  }
}

///
/// \brief TEST_F
/// Drop on parent
TEST_F(TreeWidgetDnDTest, moveDataGroupSameGroup2)
{
  setupDataTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(0, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "aBit");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/data-element"));

  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0,
                                     model->index(0, 0, model->index(0, 0))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_TRUE(
    model->dropMimeData(mimedata, Qt::MoveAction, -1, 0, model->index(0, 0, model->index(0, 0))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aBit position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aBit position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
  }
}

///
/// \brief TEST_F
/// Drop on item
TEST_P(TreeWidgetDnDTestActions, moveDataOtherGroup)
{
  auto action = GetParam();
  setupDataTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(0, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "aBit");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/data-element"));

  EXPECT_TRUE(model->canDropMimeData(mimedata, action, -1, 0,
                                     model->index(0, 0, model->index(1, 0, model->index(0, 0)))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "anInteger");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "unsignedInteger");
  EXPECT_TRUE(model->dropMimeData(mimedata, action, -1, 0,
                                  model->index(0, 0, model->index(1, 0, model->index(0, 0)))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(0, 0))), 3);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "anInteger");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");
  EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "unsignedInteger");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aBit position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
    EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "anInteger");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "unsignedInteger");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aBit position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
    EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(0, 0))), 3);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "anInteger");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
    EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "unsignedInteger");
  }
}

///
/// \brief TEST_F
/// Drop below item
TEST_P(TreeWidgetDnDTestActions, moveDataGroupOtherGroup1)
{
  auto action = GetParam();
  setupDataTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(0, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "aBit");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/data-element"));

  EXPECT_TRUE(
    model->canDropMimeData(mimedata, action, 1, 0, model->index(1, 0, model->index(0, 0))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "anInteger");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "unsignedInteger");
  EXPECT_TRUE(model->dropMimeData(mimedata, action, 1, 0, model->index(1, 0, model->index(0, 0))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(0, 0))), 3);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "anInteger");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");
  EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "unsignedInteger");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aBit position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
    EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "anInteger");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "unsignedInteger");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aBit position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
    EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(0, 0))), 3);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "anInteger");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
    EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "unsignedInteger");
  }
}

///
/// \brief TEST_F
/// Drop on group
TEST_F(TreeWidgetDnDTest, moveDataGroupOtherGroup2)
{
  setupDataTree();

  auto string_index = model->index(1, 0, model->index(0, 0, model->index(0, 0)));
  EXPECT_EQ(model->data(string_index).toString(), "aString");
  auto mimedata = model->mimeData({ string_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/data-element"));

  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0,
                                     model->index(1, 0, model->index(0, 0))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "anInteger");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "unsignedInteger");
  EXPECT_TRUE(
    model->dropMimeData(mimedata, Qt::MoveAction, -1, 0, model->index(1, 0, model->index(0, 0))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");
  EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(0, 0))), 3);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "anInteger");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "unsignedInteger");
  EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aString position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
    EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "anInteger");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "unsignedInteger");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aString position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
    EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(0, 0))), 3);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "anInteger");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "unsignedInteger");
    EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
  }
}

TEST_F(TreeWidgetDnDTest, guiMimeData)
{
  setupGuiTree();

  EXPECT_EQ(model->mimeData({}), nullptr);
  EXPECT_EQ(model->mimeData(
              { model->index(0, 0, model->index(1, 0)), model->index(1, 0, model->index(1, 0)) }),
            nullptr);
  EXPECT_EQ(model->mimeData({ model->index(1, 0) }), nullptr);

  auto button_index = model->index(0, 0, model->index(0, 0, model->index(1, 0)));
  EXPECT_EQ(model->data(button_index).toString(), "button");
  auto mimedata = model->mimeData({ button_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/gui-element"));

  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0, model->index(0, 0)));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0, {}));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::MoveAction, 0, 0, {}));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0, model->index(1, 0)));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::MoveAction, 1, 0, model->index(1, 0)));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0,
                                     model->index(0, 0, model->index(1, 0))));
  EXPECT_TRUE(
    model->canDropMimeData(mimedata, Qt::MoveAction, 1, 0, model->index(0, 0, model->index(1, 0))));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0,
                                     model->index(1, 0, model->index(0, 0, model->index(1, 0)))));

  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, -1, 0, model->index(0, 0)));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, -1, 0, {}));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, 0, 0, {}));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, -1, 0, model->index(1, 0)));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, 1, 0, model->index(1, 0)));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, -1, 0,
                                      model->index(0, 0, model->index(1, 0))));
  EXPECT_FALSE(
    model->canDropMimeData(mimedata, Qt::CopyAction, 1, 0, model->index(0, 0, model->index(1, 0))));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::CopyAction, -1, 0,
                                      model->index(1, 0, model->index(0, 0, model->index(1, 0)))));

  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0, model->index(0, 0)));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0, {}));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::LinkAction, 0, 0, {}));
  EXPECT_FALSE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0, model->index(1, 0)));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::LinkAction, 1, 0, model->index(1, 0)));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0,
                                     model->index(0, 0, model->index(1, 0))));
  EXPECT_TRUE(
    model->canDropMimeData(mimedata, Qt::LinkAction, 1, 0, model->index(0, 0, model->index(1, 0))));
  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0,
                                     model->index(1, 0, model->index(0, 0, model->index(1, 0)))));
}
///
/// \brief TEST_F
/// Drop on sibling
TEST_P(TreeWidgetDnDTestActions, moveGuiSameGroup)
{
  auto action = GetParam();
  setupGuiTree();

  auto button_index = model->index(0, 0, model->index(0, 0, model->index(1, 0)));
  EXPECT_EQ(model->data(button_index).toString(), "button");
  auto mimedata = model->mimeData({ button_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/gui-element"));

  EXPECT_TRUE(model->canDropMimeData(mimedata, action, -1, 0,
                                     model->index(1, 0, model->index(0, 0, model->index(1, 0)))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_TRUE(model->dropMimeData(mimedata, action, -1, 0,
                                  model->index(1, 0, model->index(0, 0, model->index(1, 0)))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aButton position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aButton position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
  }
}

///
/// \brief TEST_F
/// Drop below sibling
TEST_P(TreeWidgetDnDTestActions, moveGuiGroupSameGroup1)
{
  auto action = GetParam();
  setupGuiTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(1, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "button");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/gui-element"));

  EXPECT_TRUE(
    model->canDropMimeData(mimedata, action, 2, 0, model->index(0, 0, model->index(1, 0))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_TRUE(model->dropMimeData(mimedata, action, 2, 0, model->index(0, 0, model->index(1, 0))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aButton position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aButton position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
  }
}

///
/// \brief TEST_F
/// Drop on parent
TEST_F(TreeWidgetDnDTest, moveGuiGroupSameGroup2)
{
  setupGuiTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(1, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "button");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/gui-element"));

  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0,
                                     model->index(0, 0, model->index(1, 0))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_TRUE(
    model->dropMimeData(mimedata, Qt::MoveAction, -1, 0, model->index(0, 0, model->index(1, 0))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aButton position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aButton position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
  }
}

///
/// \brief TEST_F
/// Drop on item
TEST_P(TreeWidgetDnDTestActions, moveGuiOtherGroup)
{
  auto action = GetParam();
  setupGuiTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(1, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "button");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/gui-element"));

  EXPECT_TRUE(model->canDropMimeData(mimedata, action, -1, 0,
                                     model->index(0, 0, model->index(1, 0, model->index(1, 0)))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "combo_box");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "line_edit");
  EXPECT_TRUE(model->dropMimeData(mimedata, action, -1, 0,
                                  model->index(0, 0, model->index(1, 0, model->index(1, 0)))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(1, 0))), 3);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "combo_box");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");
  EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "line_edit");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aButton position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
    EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(1, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "combo_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "line_edit");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aButton position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
    EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(1, 0))), 3);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "combo_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
    EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "line_edit");
  }
}

///
/// \brief TEST_F
/// Drop below item
TEST_P(TreeWidgetDnDTestActions, moveGuiGroupOtherGroup1)
{
  auto action = GetParam();
  setupGuiTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(1, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "button");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/gui-element"));

  EXPECT_TRUE(
    model->canDropMimeData(mimedata, action, 1, 0, model->index(1, 0, model->index(1, 0))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "combo_box");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "line_edit");
  EXPECT_TRUE(model->dropMimeData(mimedata, action, 1, 0, model->index(1, 0, model->index(1, 0))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(1, 0))), 3);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "combo_box");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");
  EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "line_edit");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aButton position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
    EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(1, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "combo_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "line_edit");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aButton position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
    EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(1, 0))), 3);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "combo_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
    EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "line_edit");
  }
}

///
/// \brief TEST_F
/// Drop on group
TEST_F(TreeWidgetDnDTest, moveGuiGroupOtherGroup2)
{
  setupGuiTree();

  auto string_index = model->index(1, 0, model->index(0, 0, model->index(1, 0)));
  EXPECT_EQ(model->data(string_index).toString(), "check_box");
  auto mimedata = model->mimeData({ string_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/gui-element"));

  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::MoveAction, -1, 0,
                                     model->index(1, 0, model->index(1, 0))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "combo_box");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "line_edit");
  EXPECT_TRUE(
    model->dropMimeData(mimedata, Qt::MoveAction, -1, 0, model->index(1, 0, model->index(1, 0))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");
  EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(1, 0))), 3);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "combo_box");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "line_edit");
  EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aCheckbox position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
    EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(1, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "combo_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "line_edit");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aCheckbox position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
    EXPECT_EQ(model->rowCount(model->index(1, 0, model->index(1, 0))), 3);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "combo_box");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "line_edit");
    EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
  }
}

///
/// \brief TEST_F
/// Only relevant for dropping on groups: item will not become child but sibling
TEST_F(TreeWidgetDnDTest, linkDataOnSameGroup)
{
  setupDataTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(0, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "aBit");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/data-element"));

  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0,
                                     model->index(0, 0, model->index(0, 0))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_TRUE(
    model->dropMimeData(mimedata, Qt::LinkAction, -1, 0, model->index(0, 0, model->index(0, 0))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_EQ(model->rowCount(model->index(0, 0)), 3);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0))).toString().toStdString(), "group1");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0))).toString().toStdString(), "aBit");
  EXPECT_EQ(model->data(model->index(2, 0, model->index(0, 0))).toString().toStdString(), "group2");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aBit position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
    EXPECT_EQ(model->rowCount(model->index(0, 0)), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0))).toString().toStdString(),
              "group1");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0))).toString().toStdString(),
              "group2");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aBit position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
    EXPECT_EQ(model->rowCount(model->index(0, 0)), 3);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0))).toString().toStdString(),
              "group1");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0))).toString().toStdString(), "aBit");
    EXPECT_EQ(model->data(model->index(2, 0, model->index(0, 0))).toString().toStdString(),
              "group2");
  }
}

///
/// \brief TEST_F
/// Only relevant for dropping on groups: item will not become child but sibling
TEST_F(TreeWidgetDnDTest, linkDataOnOtherGroup)
{
  setupDataTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(0, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "aBit");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/data-element"));

  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0,
                                     model->index(1, 0, model->index(0, 0))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aBit");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_TRUE(
    model->dropMimeData(mimedata, Qt::LinkAction, -1, 0, model->index(1, 0, model->index(0, 0))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
              .toString()
              .toStdString(),
            "aString");
  EXPECT_EQ(model->rowCount(model->index(0, 0)), 3);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0))).toString().toStdString(), "group1");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0))).toString().toStdString(), "group2");
  EXPECT_EQ(model->data(model->index(2, 0, model->index(0, 0))).toString().toStdString(), "aBit");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aBit position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aBit");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
    EXPECT_EQ(model->rowCount(model->index(0, 0)), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0))).toString().toStdString(),
              "group1");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0))).toString().toStdString(),
              "group2");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aBit position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(0, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(0, 0))))
                .toString()
                .toStdString(),
              "aString");
    EXPECT_EQ(model->rowCount(model->index(0, 0)), 3);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0))).toString().toStdString(),
              "group1");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0))).toString().toStdString(),
              "group2");
    EXPECT_EQ(model->data(model->index(2, 0, model->index(0, 0))).toString().toStdString(), "aBit");
  }
}

///
/// \brief TEST_F
/// Only relevant for dropping on groups: item will not become child but sibling
TEST_F(TreeWidgetDnDTest, linkGuiOnSameGroup)
{
  setupGuiTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(1, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "button");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/gui-element"));

  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0,
                                     model->index(0, 0, model->index(1, 0))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_TRUE(
    model->dropMimeData(mimedata, Qt::LinkAction, -1, 0, model->index(0, 0, model->index(1, 0))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_EQ(model->rowCount(model->index(1, 0)), 3);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0))).toString().toStdString(), "group");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0))).toString().toStdString(), "button");
  EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0))).toString().toStdString(), "group");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aButton position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aButton position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
    EXPECT_EQ(model->rowCount(model->index(1, 0)), 3);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0))).toString().toStdString(),
              "group");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0))).toString().toStdString(),
              "button");
    EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0))).toString().toStdString(),
              "group");
  }
}

///
/// \brief TEST_F
/// Only relevant for dropping on groups: item will not become child but sibling
TEST_F(TreeWidgetDnDTest, linkGuiOnOtherGroup)
{
  setupGuiTree();

  auto bit_index = model->index(0, 0, model->index(0, 0, model->index(1, 0)));
  EXPECT_EQ(model->data(bit_index).toString(), "button");
  auto mimedata = model->mimeData({ bit_index });
  ASSERT_NE(mimedata, nullptr);
  EXPECT_TRUE(mimedata->hasFormat("application/yoyo/gui-element"));

  EXPECT_TRUE(model->canDropMimeData(mimedata, Qt::LinkAction, -1, 0,
                                     model->index(1, 0, model->index(1, 0))));

  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "button");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_TRUE(
    model->dropMimeData(mimedata, Qt::LinkAction, -1, 0, model->index(1, 0, model->index(1, 0))));
  EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
              .toString()
              .toStdString(),
            "check_box");
  EXPECT_EQ(model->rowCount(model->index(1, 0)), 3);
  EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0))).toString().toStdString(), "group");
  EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0))).toString().toStdString(), "group");
  EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0))).toString().toStdString(), "button");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo(), "Move item aButton position");

    handler->undo();

    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 2);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "button");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo(), "Move item aButton position");
    handler->redo();
    EXPECT_EQ(model->rowCount(model->index(0, 0, model->index(1, 0))), 1);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(0, 0, model->index(1, 0))))
                .toString()
                .toStdString(),
              "check_box");
    EXPECT_EQ(model->rowCount(model->index(1, 0)), 3);
    EXPECT_EQ(model->data(model->index(0, 0, model->index(1, 0))).toString().toStdString(),
              "group");
    EXPECT_EQ(model->data(model->index(1, 0, model->index(1, 0))).toString().toStdString(),
              "group");
    EXPECT_EQ(model->data(model->index(2, 0, model->index(1, 0))).toString().toStdString(),
              "button");
  }
}

INSTANTIATE_TEST_SUITE_P(DnD, TreeWidgetDnDTestActions,
                         testing::Values(Qt::MoveAction, Qt::LinkAction));
