#include "tool_widget.h"

#include "factory_provider.hpp"
#include "guiFixture.hpp"
#include "yoyo/gui_node.h"

#include <QListView>
#include <QMimeData>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/uuid/uuid_io.hpp>

class ToolWidgetTest : public yoyo::test::guiFixture
{
public:
  auto createWidget() -> void
  {
    _widget = std::make_shared<yoyo::gui::tool_widget>(_gui_factory);
    _widget->resize(50, 200);
    _widget->show();
  }

  auto retrieveListView(QListView** view) -> void
  {
    // dock widget stuff
    ASSERT_EQ(_widget->children().count(), 5);
    ASSERT_FALSE(_widget->children()[0]->isWidgetType()); // layout
    ASSERT_TRUE(_widget->children()[1]->isWidgetType());  // button
    ASSERT_TRUE(_widget->children()[2]->isWidgetType());  // button
    ASSERT_FALSE(_widget->children()[3]->isWidgetType());
    ASSERT_TRUE(_widget->children()[4]->isWidgetType()); // content

    // one layout and the list view
    ASSERT_EQ(_widget->children()[4]->children().count(), 2);
    ASSERT_FALSE(_widget->children()[4]->children()[0]->isWidgetType()); // layout
    ASSERT_TRUE(_widget->children()[4]->children()[1]->isWidgetType());  // listview

    (*view) = dynamic_cast<QListView*>(_widget->children()[4]->children()[1]);
  }

  std::shared_ptr<yoyo::node_factory> _gui_factory { std::make_shared<yoyo::node_factory>(
    &yoyo::gui_node::staticMetaObject) };
  std::shared_ptr<yoyo::gui::tool_widget> _widget;
};

TEST_F(ToolWidgetTest, nodesListed)
{
  yoyo::install_gui_nodes(*_gui_factory);
  createWidget();
  QListView* view = nullptr;
  retrieveListView(&view);
  ASSERT_NE(view, nullptr);

  auto m = view->model();
  ASSERT_EQ(m->rowCount(), 5);

  EXPECT_EQ(m->data(m->index(0, 0)).toString().toStdString(), "Group");
  EXPECT_EQ(m->data(m->index(1, 0)).toString().toStdString(), "Button");
  EXPECT_EQ(m->data(m->index(2, 0)).toString().toStdString(), "CheckBox");
  EXPECT_EQ(m->data(m->index(3, 0)).toString().toStdString(), "ComboBox");
  EXPECT_EQ(m->data(m->index(4, 0)).toString().toStdString(), "LineEdit");

  EXPECT_EQ(m->data(m->index(0, 0), Qt::DecorationRole).metaType(), QMetaType::fromType<QIcon>());
  EXPECT_EQ(m->data(m->index(1, 0), Qt::DecorationRole).metaType(), QMetaType::fromType<QIcon>());
  EXPECT_EQ(m->data(m->index(2, 0), Qt::DecorationRole).metaType(), QMetaType::fromType<QIcon>());
  EXPECT_EQ(m->data(m->index(3, 0), Qt::DecorationRole).metaType(), QMetaType::fromType<QIcon>());
  EXPECT_EQ(m->data(m->index(4, 0), Qt::DecorationRole).metaType(), QMetaType::fromType<QIcon>());

  EXPECT_FALSE(m->data(m->index(5, 0)).isValid());
  EXPECT_FALSE(m->data(m->index(-1, 0)).isValid());
}

TEST_F(ToolWidgetTest, update)
{
  createWidget();
  QListView* view = nullptr;
  retrieveListView(&view);
  ASSERT_NE(view, nullptr);

  auto m = view->model();
  ASSERT_EQ(m->rowCount(), 0);
  yoyo::install_gui_nodes(*_gui_factory);
  ASSERT_EQ(m->rowCount(), 0);
  _widget->update();
  ASSERT_EQ(m->rowCount(), 5);

  EXPECT_EQ(m->data(m->index(0, 0)).toString().toStdString(), "Group");
  EXPECT_EQ(m->data(m->index(1, 0)).toString().toStdString(), "Button");
  EXPECT_EQ(m->data(m->index(2, 0)).toString().toStdString(), "CheckBox");
  EXPECT_EQ(m->data(m->index(3, 0)).toString().toStdString(), "ComboBox");
  EXPECT_EQ(m->data(m->index(4, 0)).toString().toStdString(), "LineEdit");

  EXPECT_EQ(m->data(m->index(0, 0), Qt::DecorationRole).metaType(), QMetaType::fromType<QIcon>());
  EXPECT_EQ(m->data(m->index(1, 0), Qt::DecorationRole).metaType(), QMetaType::fromType<QIcon>());
  EXPECT_EQ(m->data(m->index(2, 0), Qt::DecorationRole).metaType(), QMetaType::fromType<QIcon>());
  EXPECT_EQ(m->data(m->index(3, 0), Qt::DecorationRole).metaType(), QMetaType::fromType<QIcon>());
  EXPECT_EQ(m->data(m->index(4, 0), Qt::DecorationRole).metaType(), QMetaType::fromType<QIcon>());
}

TEST_F(ToolWidgetTest, drag)
{
  yoyo::install_gui_nodes(*_gui_factory);
  createWidget();
  QListView* view = nullptr;
  retrieveListView(&view);
  ASSERT_NE(view, nullptr);

  EXPECT_FALSE(view->acceptDrops());
  EXPECT_TRUE(view->dragEnabled());
  auto m = view->model();
  ASSERT_EQ(m->rowCount(), 5);
  EXPECT_EQ(m->mimeTypes(), QStringList {} << "application/yoyo/gui-new");

  auto nodelist = _gui_factory->installed_nodes();

  for (size_t i = 0; i < nodelist.size(); i++) {
    auto mimedata = m->mimeData({ m->index(i, 0) });
    ASSERT_NE(mimedata, nullptr);
    EXPECT_TRUE(mimedata->hasFormat("application/yoyo/gui-new"));
    EXPECT_EQ(mimedata->data("application/yoyo/gui-new").toStdString(),
              boost::uuids::to_string(std::get<0>(nodelist[i])));
  }

  EXPECT_EQ(m->mimeData({ m->index(m->rowCount(), 0) }), nullptr);
  EXPECT_EQ(m->mimeData({}), nullptr);
  EXPECT_EQ(m->mimeData({ m->index(0, 0), m->index(1, 0) }), nullptr);
}
