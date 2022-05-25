#pragma once
#include "tree_widget.h"

#include "guiFixture.hpp"
#include "yoyo/communication_node.h"
#include "yoyo/data_node.h"
#include "yoyo/gui_node.h"
#include "yoyo/node_factory.h"
#include "yoyo/protocol_node.h"

#include <QMenu>
#include <QTreeView>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class TreeWidgetTest : public yoyo::test::guiFixture
{
public:
  auto SetUp() -> void override;
  auto createWidget() -> void;
  auto setupConfiguration() -> std::shared_ptr<yoyo::node_base>;
  auto retrieveListView() -> void;
  auto indexPoint(QWidget* to, QModelIndex index) -> QPoint;

  auto addChild(std::shared_ptr<yoyo::node_base> parent, size_t index, size_t typeIndex,
                size_t factoryIndex, QString const& name) -> void;

  std::shared_ptr<yoyo::node_factory> _fundamental_factory { std::make_shared<yoyo::node_factory>(
    &yoyo::node_base::staticMetaObject) };
  std::shared_ptr<yoyo::node_factory> _data_factory { std::make_shared<yoyo::node_factory>(
    &yoyo::data_node::staticMetaObject) };
  std::shared_ptr<yoyo::node_factory> _gui_factory { std::make_shared<yoyo::node_factory>(
    &yoyo::gui_node::staticMetaObject) };
  std::shared_ptr<yoyo::node_factory> _protocol_factory { std::make_shared<yoyo::node_factory>(
    &yoyo::protocol_node::staticMetaObject) };
  std::shared_ptr<yoyo::node_factory> _communication_factory { std::make_shared<yoyo::node_factory>(
    &yoyo::communication_node::staticMetaObject) };
  std::shared_ptr<yoyo::gui::tree_widget> _widget;

  QTreeView* view;
  QMenu* menu;
};
