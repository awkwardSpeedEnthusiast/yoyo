#include "TreeWidgetFixture.hpp"
#include "tree_utils.hpp"

#include "command_handler.h"

#include <string>
using std::string_literals::operator""s;
using testing::_;
using CO = yoyo::node_base::ChangeOperation;

class TreeWidgetRequestTest : public TreeWidgetTest
{
public:
  auto setupTree() -> void
  {
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
    model = view->model();
    ASSERT_NE(model, nullptr);
  }

  std::shared_ptr<yoyo::node_base> configuration;
  QAbstractItemModel* model { nullptr };
};

auto operator==(std::weak_ptr<yoyo::node_base> const& lhs,
                std::weak_ptr<yoyo::node_base> const& rhs) -> bool
{
  return lhs.lock() == rhs.lock();
}

struct TreeChangeReceiver {
  MOCK_METHOD(void, treeChanged,
              (std::weak_ptr<yoyo::node_base> parent, std::weak_ptr<yoyo::node_base>, CO));
};

TEST_F(TreeWidgetRequestTest, exchange)
{
  setupTree();
  testing::StrictMock<TreeChangeReceiver> receiver;
  QObject lifetimer;
  QObject::connect(configuration.get(), &yoyo::node_base::treeChanged, &lifetimer,
                   [&receiver](auto a, auto b, auto c) { receiver.treeChanged(a, b, c); });
  auto requestingItem = configuration->childAt(1)->childAt(0);
  auto sameId = requestingItem->staticTypeId();
  auto buttonId = std::get<boost::uuids::uuid>(_gui_factory->installed_nodes()[1]);
  // exchange for same id is caught
  EXPECT_CALL(receiver, treeChanged(_, _, _)).Times(0);
  requestingItem->exchangeRequested(requestingItem, sameId);
  // objects with children may not be exchanged for items, that cannot take children
  EXPECT_CALL(receiver, treeChanged(_, _, _)).Times(0);
  requestingItem->exchangeRequested(requestingItem, buttonId);

  requestingItem = configuration->childAt(1)->childAt(1)->childAt(1);
  sameId = requestingItem->staticTypeId();
  // exchange for same id is caught
  EXPECT_CALL(receiver, treeChanged(_, _, _)).Times(0);
  requestingItem->exchangeRequested(requestingItem, sameId);

  requestingItem = configuration->childAt(0)->childAt(0)->childAt(0);
  {
    EXPECT_FALSE(requestingItem->acceptsChildren());
    EXPECT_CALL(receiver, treeChanged(requestingItem->parent(), requestingItem->weak_from_this(),
                                      CO::REMOVED))
      .Times(0);
    EXPECT_EQ(requestingItem->type().toStdString(), "bit");
    requestingItem->exchangeRequested(
      requestingItem, std::get<boost::uuids::uuid>(_data_factory->installed_nodes()[3]));
    EXPECT_EQ(requestingItem->type().toStdString(), "bit");
  }

  testing::Mock::VerifyAndClearExpectations(&receiver);
  testing::Sequence seq;
  requestingItem = configuration->childAt(1)->childAt(1)->childAt(1);
  EXPECT_EQ(configuration->childAt(1)->childAt(1)->childCount(), 2);
  EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(0)->type().toStdString(), "combo_box");
  EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(1)->type().toStdString(), "line_edit");

  EXPECT_CALL(receiver,
              treeChanged(configuration->childAt(1)->childAt(1)->weak_from_this(), _, CO::ADDED))
    .InSequence(seq);
  EXPECT_CALL(receiver, treeChanged(configuration->childAt(1)->childAt(1)->weak_from_this(),
                                    requestingItem->weak_from_this(), CO::REMOVED))
    .InSequence(seq);
  requestingItem->exchangeRequested(requestingItem, buttonId);

  EXPECT_EQ(configuration->childAt(1)->childAt(1)->childCount(), 2);
  EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(0)->type().toStdString(), "combo_box");
  EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(1)->type().toStdString(), "button");

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo().toStdString(), "Change item type from line_edit to button");

    EXPECT_CALL(receiver, treeChanged(configuration->childAt(1)->childAt(1)->weak_from_this(),
                                      requestingItem->weak_from_this(), CO::ADDED))
      .InSequence(seq);
    EXPECT_CALL(receiver, treeChanged(configuration->childAt(1)->childAt(1)->weak_from_this(), _,
                                      CO::REMOVED))
      .InSequence(seq);
    handler->undo();

    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childCount(), 2);
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(0)->type().toStdString(), "combo_box");
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(1)->type().toStdString(), "line_edit");

    EXPECT_FALSE(handler->hasCommandToUndo());
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Change item type from line_edit to button");

    EXPECT_CALL(receiver,
                treeChanged(configuration->childAt(1)->childAt(1)->weak_from_this(), _, CO::ADDED))
      .InSequence(seq);
    EXPECT_CALL(receiver, treeChanged(configuration->childAt(1)->childAt(1)->weak_from_this(),
                                      requestingItem->weak_from_this(), CO::REMOVED))
      .InSequence(seq);
    handler->redo();

    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childCount(), 2);
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(0)->type().toStdString(), "combo_box");
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(1)->type().toStdString(), "button");

    EXPECT_FALSE(handler->hasCommandToRedo());
  }
}

TEST_F(TreeWidgetRequestTest, add)
{
  setupTree();

  testing::StrictMock<TreeChangeReceiver> receiver;
  QObject lifetimer;
  QObject::connect(configuration.get(), &yoyo::node_base::treeChanged, &lifetimer,
                   [&receiver](auto a, auto b, auto c) { receiver.treeChanged(a, b, c); });
  auto buttonId = std::get<boost::uuids::uuid>(_gui_factory->installed_nodes()[1]);
  // doesn't work for data-nodes
  {
    auto requestingItem = configuration->childAt(0)->childAt(1);
    EXPECT_TRUE(requestingItem->acceptsChildren());

    EXPECT_CALL(receiver, treeChanged(_, _, CO::ADDED)).Times(0);
    requestingItem->addRequested(
      requestingItem, std::get<boost::uuids::uuid>(_data_factory->installed_nodes()[1]), 1);
  }
  // doesn't work on items that don't accept children
  {
    auto requestingItem = configuration->childAt(1)->childAt(1)->childAt(0);
    EXPECT_FALSE(requestingItem->acceptsChildren());

    EXPECT_CALL(receiver, treeChanged(_, _, CO::ADDED)).Times(0);
    requestingItem->addRequested(requestingItem, buttonId, 1);
  }
  {
    auto requestingItem = configuration->childAt(1)->childAt(1);

    EXPECT_EQ(requestingItem->childCount(), 2);

    EXPECT_CALL(receiver, treeChanged(requestingItem->weak_from_this(), _, CO::ADDED));
    requestingItem->addRequested(requestingItem, buttonId, 1);

    EXPECT_EQ(requestingItem->childCount(), 3);
    EXPECT_EQ(requestingItem->childAt(0)->type().toStdString(), "combo_box");
    EXPECT_EQ(requestingItem->childAt(1)->type().toStdString(), "button");
    EXPECT_EQ(requestingItem->childAt(2)->type().toStdString(), "line_edit");
  }

  auto handler = yoyo::command::commandhandler();
  {
    EXPECT_TRUE(handler->hasCommandToUndo());
    EXPECT_EQ(handler->nextUndo().toStdString(), "Add new item to group");

    EXPECT_CALL(receiver, treeChanged(configuration->childAt(1)->childAt(1)->weak_from_this(), _,
                                      CO::REMOVED));
    handler->undo();

    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childCount(), 2);
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(0)->type().toStdString(), "combo_box");
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(1)->type().toStdString(), "line_edit");

    EXPECT_FALSE(handler->hasCommandToUndo());
  }

  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Add new item to group");

    EXPECT_CALL(receiver,
                treeChanged(configuration->childAt(1)->childAt(1)->weak_from_this(), _, CO::ADDED));
    handler->redo();

    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childCount(), 3);
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(0)->type().toStdString(), "combo_box");
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(1)->type().toStdString(), "button");
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(2)->type().toStdString(), "line_edit");

    EXPECT_FALSE(handler->hasCommandToRedo());
  }
}

TEST_F(TreeWidgetRequestTest, addForData)
{
  setupTree();

  auto path = yoyo::utilities::calculatePath<yoyo::utilities::path_strategy_t::INDEX>(
    configuration->childAt(0)->childAt(1)->childAt(0));
  testing::StrictMock<TreeChangeReceiver> receiver;
  QObject lifetimer;
  QObject::connect(configuration.get(), &yoyo::node_base::treeChanged, &lifetimer,
                   [&receiver](auto a, auto b, auto c) { receiver.treeChanged(a, b, c); });
  testing::Sequence seq;
  // doesn't work for items that don't accept children
  {
    auto requestingItem =
      std::dynamic_pointer_cast<yoyo::gui_node>(configuration->childAt(1)->childAt(0)->childAt(0));
    EXPECT_CALL(receiver, treeChanged(_, _, _)).Times(0).InSequence(seq);
    ASSERT_NE(requestingItem, nullptr);
    requestingItem->defaultForDataRequested(path, requestingItem, 0, false, false, true);
  }

  // integer
  {
    auto requestingItem =
      std::dynamic_pointer_cast<yoyo::gui_node>(configuration->childAt(1)->childAt(0));
    ASSERT_NE(requestingItem, nullptr);
    EXPECT_EQ(requestingItem->childCount(), 2);
    EXPECT_CALL(receiver, treeChanged(requestingItem->weak_from_this(), _, CO::ADDED))
      .InSequence(seq);
    requestingItem->defaultForDataRequested(path, requestingItem, 0, false, false, true);
    ASSERT_EQ(requestingItem->childCount(), 3);
    EXPECT_EQ(requestingItem->childAt(0)->type().toStdString(), "line_edit");
    EXPECT_EQ(requestingItem->childAt(1)->type().toStdString(), "button");
    EXPECT_EQ(requestingItem->childAt(2)->type().toStdString(), "check_box");
    EXPECT_EQ(requestingItem->childAt(0)->property("connection").typeName(),
              "yoyo::properties::connection_t"s);
    auto c =
      requestingItem->childAt(0)->property("connection").value<yoyo::properties::connection_t>();
    EXPECT_EQ(c._in, "");
    EXPECT_EQ(c._out, "");
    EXPECT_EQ(c._auto, path);
  }
  path = yoyo::utilities::calculatePath<yoyo::utilities::path_strategy_t::INDEX>(
    configuration->childAt(0)->childAt(0)->childAt(0));
  // bit
  {
    auto requestingItem =
      std::dynamic_pointer_cast<yoyo::gui_node>(configuration->childAt(1)->childAt(1));
    ASSERT_NE(requestingItem, nullptr);
    EXPECT_CALL(receiver, treeChanged(requestingItem->weak_from_this(), _, CO::ADDED))
      .InSequence(seq);
    requestingItem->defaultForDataRequested(path, requestingItem, -1, false, true, false);
    EXPECT_EQ(requestingItem->childCount(), 3);
    EXPECT_EQ(requestingItem->childAt(0)->type().toStdString(), "combo_box");
    EXPECT_EQ(requestingItem->childAt(1)->type().toStdString(), "line_edit");
    EXPECT_EQ(requestingItem->childAt(2)->type().toStdString(), "check_box");

    EXPECT_EQ(requestingItem->childAt(2)->property("connection").typeName(),
              "yoyo::properties::connection_t"s);
    auto c =
      requestingItem->childAt(2)->property("connection").value<yoyo::properties::connection_t>();
    EXPECT_EQ(c._in.toStdString(), "");
    EXPECT_EQ(c._out.toStdString(), path.toStdString());
    EXPECT_EQ(c._auto.toStdString(), "");
  }
  path = yoyo::utilities::calculatePath<yoyo::utilities::path_strategy_t::INDEX>(
    configuration->childAt(0)->childAt(1)->childAt(1));
  // uint
  {
    auto requestingItem =
      std::dynamic_pointer_cast<yoyo::gui_node>(configuration->childAt(1)->childAt(1));
    ASSERT_NE(requestingItem, nullptr);
    EXPECT_CALL(receiver, treeChanged(requestingItem->weak_from_this(), _, CO::ADDED))
      .InSequence(seq);
    requestingItem->defaultForDataRequested(path, requestingItem, -1, true, false, false);
    EXPECT_EQ(requestingItem->childCount(), 4);
    EXPECT_EQ(requestingItem->childAt(0)->type().toStdString(), "combo_box");
    EXPECT_EQ(requestingItem->childAt(1)->type().toStdString(), "line_edit");
    EXPECT_EQ(requestingItem->childAt(2)->type().toStdString(), "check_box");
    EXPECT_EQ(requestingItem->childAt(3)->type().toStdString(), "combo_box");

    EXPECT_EQ(requestingItem->childAt(3)->property("connection").typeName(),
              "yoyo::properties::connection_t"s);
    auto c =
      requestingItem->childAt(3)->property("connection").value<yoyo::properties::connection_t>();
    EXPECT_EQ(c._in.toStdString(), path.toStdString());
    EXPECT_EQ(c._out.toStdString(), "");
    EXPECT_EQ(c._auto.toStdString(), "");
  }

  auto handler = yoyo::command::commandhandler();
  { { EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Add new item to group for signal");

  EXPECT_CALL(receiver,
              treeChanged(configuration->childAt(1)->childAt(1)->weak_from_this(), _, CO::REMOVED))
    .InSequence(seq);
  handler->undo();

  EXPECT_EQ(configuration->childAt(1)->childAt(1)->childCount(), 3);
  EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(0)->type().toStdString(), "combo_box");
  EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(1)->type().toStdString(), "line_edit");
  EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(2)->type().toStdString(), "check_box");
}
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Add new item to group for signal");

  EXPECT_CALL(receiver,
              treeChanged(configuration->childAt(1)->childAt(1)->weak_from_this(), _, CO::REMOVED))
    .InSequence(seq);
  handler->undo();

  ASSERT_EQ(configuration->childAt(1)->childAt(1)->childCount(), 2);
  EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(0)->type().toStdString(), "combo_box");
  EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(1)->type().toStdString(), "line_edit");
}
{
  EXPECT_TRUE(handler->hasCommandToUndo());
  EXPECT_EQ(handler->nextUndo().toStdString(), "Add new item to group for signal");

  EXPECT_CALL(receiver,
              treeChanged(configuration->childAt(1)->childAt(0)->weak_from_this(), _, CO::REMOVED))
    .InSequence(seq);
  handler->undo();

  EXPECT_EQ(configuration->childAt(1)->childAt(0)->childCount(), 2);
  EXPECT_EQ(configuration->childAt(1)->childAt(0)->childAt(0)->type().toStdString(), "button");
  EXPECT_EQ(configuration->childAt(1)->childAt(0)->childAt(1)->type().toStdString(), "check_box");
}
}

{
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Add new item to group for signal");

    EXPECT_CALL(receiver,
                treeChanged(configuration->childAt(1)->childAt(0)->weak_from_this(), _, CO::ADDED))
      .InSequence(seq);
    handler->redo();

    path = yoyo::utilities::calculatePath<yoyo::utilities::path_strategy_t::INDEX>(
      configuration->childAt(0)->childAt(1)->childAt(0));
    ASSERT_EQ(configuration->childAt(1)->childAt(0)->childCount(), 3);
    EXPECT_EQ(configuration->childAt(1)->childAt(0)->childAt(0)->type().toStdString(), "line_edit");
    EXPECT_EQ(configuration->childAt(1)->childAt(0)->childAt(1)->type().toStdString(), "button");
    EXPECT_EQ(configuration->childAt(1)->childAt(0)->childAt(2)->type().toStdString(), "check_box");
    EXPECT_EQ(configuration->childAt(1)->childAt(0)->childAt(0)->property("connection").typeName(),
              "yoyo::properties::connection_t"s);
    auto c = configuration->childAt(1)
               ->childAt(0)
               ->childAt(0)
               ->property("connection")
               .value<yoyo::properties::connection_t>();
    EXPECT_EQ(c._in, "");
    EXPECT_EQ(c._out, "");
    EXPECT_EQ(c._auto, path);
  }
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Add new item to group for signal");

    EXPECT_CALL(receiver,
                treeChanged(configuration->childAt(1)->childAt(1)->weak_from_this(), _, CO::ADDED))
      .InSequence(seq);
    handler->redo();

    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childCount(), 3);
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(0)->type().toStdString(), "combo_box");
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(1)->type().toStdString(), "line_edit");
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(2)->type().toStdString(), "check_box");

    path = yoyo::utilities::calculatePath<yoyo::utilities::path_strategy_t::INDEX>(
      configuration->childAt(0)->childAt(0)->childAt(0));
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(2)->property("connection").typeName(),
              "yoyo::properties::connection_t"s);
    auto c = configuration->childAt(1)
               ->childAt(1)
               ->childAt(2)
               ->property("connection")
               .value<yoyo::properties::connection_t>();
    EXPECT_EQ(c._in.toStdString(), "");
    EXPECT_EQ(c._out.toStdString(), path.toStdString());
    EXPECT_EQ(c._auto.toStdString(), "");
  }
  {
    EXPECT_TRUE(handler->hasCommandToRedo());
    EXPECT_EQ(handler->nextRedo().toStdString(), "Add new item to group for signal");

    EXPECT_CALL(receiver,
                treeChanged(configuration->childAt(1)->childAt(1)->weak_from_this(), _, CO::ADDED))
      .InSequence(seq);
    handler->redo();

    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childCount(), 4);
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(0)->type().toStdString(), "combo_box");
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(1)->type().toStdString(), "line_edit");
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(2)->type().toStdString(), "check_box");
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(3)->type().toStdString(), "combo_box");

    path = yoyo::utilities::calculatePath<yoyo::utilities::path_strategy_t::INDEX>(
      configuration->childAt(0)->childAt(1)->childAt(1));
    EXPECT_EQ(configuration->childAt(1)->childAt(1)->childAt(3)->property("connection").typeName(),
              "yoyo::properties::connection_t"s);
    auto c = configuration->childAt(1)
               ->childAt(1)
               ->childAt(3)
               ->property("connection")
               .value<yoyo::properties::connection_t>();
    EXPECT_EQ(c._in.toStdString(), path.toStdString());
    EXPECT_EQ(c._out.toStdString(), "");
    EXPECT_EQ(c._auto.toStdString(), "");
  }
}
}
