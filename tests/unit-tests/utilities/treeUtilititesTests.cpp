#include "guiFixture.hpp"

#include "tree_utils.hpp"

#include "configuration.hpp"
#include "factory_provider.hpp"

#include "yoyo/data_container_node.h"
#include "yoyo/data_node.h"
#include "yoyo/data_value_node.h"
#include "yoyo/gui_node.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <QMetaProperty>

#include <boost/uuid/random_generator.hpp>

class leaf_gui : public yoyo::gui_node
{
  Q_OBJECT

public:
  using yoyo::gui_node::gui_node;
  auto acceptsChildren() const -> bool final
  {
    return false;
  }
  auto acceptsChild(std::shared_ptr<node_base> const&) const -> bool final
  {
    return false;
  }
  auto acceptsParent(std::shared_ptr<node_base> const&) const -> bool override
  {
    return true;
  }

private:
  auto beforeChildAdd(std::shared_ptr<node_base> const&) -> bool final
  {
    return false;
  }
  auto afterChildAdd(std::shared_ptr<node_base> const&) -> bool final
  {
    return false;
  }
  auto beforeChildRemove(std::shared_ptr<node_base> const&) -> bool final
  {
    return false;
  }
  auto afterChildRemove(std::shared_ptr<node_base> const&) -> bool final
  {
    return false;
  }
  auto afterChildMoved(std::shared_ptr<node_base> const&, size_t, size_t) -> void final {}
};
class inner_gui : public yoyo::gui_node
{
  Q_OBJECT

public:
  using yoyo::gui_node::gui_node;
  auto acceptsChildren() const -> bool final
  {
    return true;
  }
  auto acceptsChild(std::shared_ptr<node_base> const&) const -> bool final
  {
    return true;
  }
  auto acceptsParent(std::shared_ptr<node_base> const&) const -> bool override
  {
    return true;
  }

private:
  auto beforeChildAdd(std::shared_ptr<node_base> const&) -> bool final
  {
    return true;
  }
  auto afterChildAdd(std::shared_ptr<node_base> const&) -> bool final
  {
    return true;
  }
  auto beforeChildRemove(std::shared_ptr<node_base> const&) -> bool final
  {
    return true;
  }
  auto afterChildRemove(std::shared_ptr<node_base> const&) -> bool final
  {
    return true;
  }
  auto afterChildMoved(std::shared_ptr<node_base> const&, size_t, size_t) -> void final {}
};

class gui_mock : public leaf_gui
{
  Q_OBJECT
public:
  gui_mock()
    : leaf_gui { "gui_mock", nullptr, boost::uuids::uuid {} }
  {
  }
  MOCK_METHOD(bool, supportsValueType, (yoyo::types::value_t valueType), (const, override));
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _typeId;
  }

private:
  static boost::uuids::uuid const _typeId;
};

class gui_container_mock : public inner_gui
{
  Q_OBJECT
public:
  gui_container_mock()
    : inner_gui { "gui_container_mock", nullptr, boost::uuids::uuid {} }
  {
  }
  MOCK_METHOD(bool, supportsValueType, (yoyo::types::value_t valueType), (const, override));
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _typeId;
  }

private:
  static boost::uuids::uuid const _typeId;
};

class data_container_mock : public yoyo::data_container_node
{
  Q_OBJECT
public:
  data_container_mock()
    : yoyo::data_container_node { "group_mock", {}, yoyo::types::value_t::CUSTOM }
  {
  }

  MOCK_METHOD(yoyo::types::variant_t, value, (), (const, override));
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid const _id;
};

class data_mock : public yoyo::data_value_node
{
  Q_OBJECT
public:
  data_mock()
    : yoyo::data_value_node { "value_mock", {}, yoyo::types::value_t::INT32 }
  {
  }

  MOCK_METHOD(yoyo::types::variant_t, value, (), (const, override));
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid const _id;
};

boost::uuids::uuid const data_container_mock::_id = boost::uuids::random_generator {}();
boost::uuids::uuid const data_mock::_id = boost::uuids::random_generator {}();
boost::uuids::uuid const gui_container_mock::_typeId = boost::uuids::random_generator {}();
boost::uuids::uuid const gui_mock::_typeId = boost::uuids::random_generator {}();

class UtilitiesTests : public yoyo::test::guiFixture
{
public:
  auto SetUp() -> void override
  {
    yoyo::test::guiFixture::SetUp();

    auto fundamentalFactory =
      std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject);
    yoyo::install_fundamental_nodes(*fundamentalFactory);

    _factories.push_back(fundamentalFactory);
  }

  auto setupConfiguration() -> std::shared_ptr<yoyo::node_base>
  {
    auto configuration =
      _factories[0]->createNode(std::get<0>(_factories[0]->installed_nodes()[0]), {});

    auto data_root =
      _factories[0]->createNode(std::get<0>(_factories[0]->installed_nodes()[1]), {});
    EXPECT_TRUE(configuration->acceptsChild(data_root));
    configuration->addChild(data_root);

    auto gui_root = _factories[0]->createNode(std::get<0>(_factories[0]->installed_nodes()[2]), {});
    EXPECT_TRUE(configuration->acceptsChild(gui_root));
    configuration->addChild(gui_root);

    return configuration;
  }

  template <typename T>
  auto createData(std::shared_ptr<yoyo::node_base> parent, QString const& name)
    -> std::shared_ptr<testing::StrictMock<T>>
  {
    auto some_data = std::make_shared<testing::StrictMock<T>>();
    parent->addChild(some_data);
    some_data->setName(name);
    return some_data;
  }

  template <typename T>
  auto createGui(std::shared_ptr<yoyo::node_base> parent) -> std::shared_ptr<testing::StrictMock<T>>
  {
    auto some_gui = std::make_shared<testing::StrictMock<T>>();
    some_gui->init();
    parent->addChild(some_gui);
    return some_gui;
  }

  std::vector<std::shared_ptr<yoyo::node_factory>> _factories;
};

TEST_F(UtilitiesTests, treeCheck_name)
{
  using S = yoyo::utilities::path_strategy_t;
  using yoyo::utilities::calculatePath;
  using yoyo::utilities::retrieveFromPath;

  auto configuration = setupConfiguration();

  auto d_group1 = createData<data_container_mock>(configuration->childAt(0), "group1");
  auto d_group2 = createData<data_container_mock>(d_group1, "group2");
  auto d_group3 = createData<data_container_mock>(d_group2, "group3");
  auto d_group4 = createData<data_container_mock>(d_group3, "group4");
  auto data1 = createData<data_mock>(configuration->childAt(0), "data1");
  auto data2 = createData<data_mock>(d_group1, "data2");
  auto data3 = createData<data_mock>(d_group2, "data3");
  auto data4 = createData<data_mock>(d_group3, "data4");
  auto data5 = createData<data_mock>(d_group4, "data5");

  EXPECT_EQ(calculatePath<S::NAME>(data1).toStdString(), "data1");
  EXPECT_EQ(calculatePath<S::NAME>(data2).toStdString(), "group1.data2");
  EXPECT_EQ(calculatePath<S::NAME>(data3).toStdString(), "group1.group2.data3");
  EXPECT_EQ(calculatePath<S::NAME>(data4).toStdString(), "group1.group2.group3.data4");
  EXPECT_EQ(calculatePath<S::NAME>(data5).toStdString(), "group1.group2.group3.group4.data5");
  EXPECT_EQ(calculatePath<S::NAME>(d_group1).toStdString(), "group1");
  EXPECT_EQ(calculatePath<S::NAME>(d_group2).toStdString(), "group1.group2");
  EXPECT_EQ(calculatePath<S::NAME>(d_group3).toStdString(), "group1.group2.group3");
  EXPECT_EQ(calculatePath<S::NAME>(d_group4).toStdString(), "group1.group2.group3.group4");

  EXPECT_EQ(retrieveFromPath<S::NAME>(calculatePath<S::NAME>(data1), configuration->childAt(0)),
            data1);
  EXPECT_EQ(retrieveFromPath<S::NAME>(calculatePath<S::NAME>(data2), configuration->childAt(0)),
            data2);
  EXPECT_EQ(retrieveFromPath<S::NAME>(calculatePath<S::NAME>(data3), configuration->childAt(0)),
            data3);
  EXPECT_EQ(retrieveFromPath<S::NAME>(calculatePath<S::NAME>(data4), configuration->childAt(0)),
            data4);
  EXPECT_EQ(retrieveFromPath<S::NAME>(calculatePath<S::NAME>(data5), configuration->childAt(0)),
            data5);
  EXPECT_EQ(retrieveFromPath<S::NAME>(calculatePath<S::NAME>(d_group1), configuration->childAt(0)),
            d_group1);
  EXPECT_EQ(retrieveFromPath<S::NAME>(calculatePath<S::NAME>(d_group2), configuration->childAt(0)),
            d_group2);
  EXPECT_EQ(retrieveFromPath<S::NAME>(calculatePath<S::NAME>(d_group3), configuration->childAt(0)),
            d_group3);
  EXPECT_EQ(retrieveFromPath<S::NAME>(calculatePath<S::NAME>(d_group4), configuration->childAt(0)),
            d_group4);
}

TEST_F(UtilitiesTests, treeCheckDataIndex)
{
  using S = yoyo::utilities::path_strategy_t;
  using yoyo::utilities::calculatePath;
  using yoyo::utilities::get_root;
  using yoyo::utilities::retrieveFromPath;

  auto configuration = setupConfiguration();

  auto d_group1 = createData<data_container_mock>(configuration->childAt(0), "group1");
  auto d_group2 = createData<data_container_mock>(d_group1, "group2");
  auto d_group3 = createData<data_container_mock>(d_group2, "group3");
  auto d_group4 = createData<data_container_mock>(d_group3, "group4");
  auto data1 = createData<data_mock>(configuration->childAt(0), "data1");
  auto data2 = createData<data_mock>(d_group1, "data2");
  auto data3 = createData<data_mock>(d_group2, "data3");
  auto data4 = createData<data_mock>(d_group3, "data4");
  auto data5 = createData<data_mock>(d_group4, "data5");

  EXPECT_EQ(get_root(d_group1), configuration);
  EXPECT_EQ(get_root(d_group2), configuration);
  EXPECT_EQ(get_root(d_group3), configuration);
  EXPECT_EQ(get_root(d_group4), configuration);
  EXPECT_EQ(get_root(data1), configuration);
  EXPECT_EQ(get_root(data2), configuration);
  EXPECT_EQ(get_root(data3), configuration);
  EXPECT_EQ(get_root(data4), configuration);
  EXPECT_EQ(get_root(data5), configuration);

  EXPECT_EQ(calculatePath<S::INDEX>(configuration).toStdString(), "");
  EXPECT_EQ(calculatePath<S::INDEX>(data1).toStdString(), "item_0.item_1");
  EXPECT_EQ(calculatePath<S::INDEX>(data2).toStdString(), "item_0.item_0.item_1");
  EXPECT_EQ(calculatePath<S::INDEX>(data3).toStdString(), "item_0.item_0.item_0.item_1");
  EXPECT_EQ(calculatePath<S::INDEX>(data4).toStdString(), "item_0.item_0.item_0.item_0.item_1");
  EXPECT_EQ(calculatePath<S::INDEX>(data5).toStdString(),
            "item_0.item_0.item_0.item_0.item_0.item_0");
  EXPECT_EQ(calculatePath<S::INDEX>(d_group1).toStdString(), "item_0.item_0");
  EXPECT_EQ(calculatePath<S::INDEX>(d_group2).toStdString(), "item_0.item_0.item_0");
  EXPECT_EQ(calculatePath<S::INDEX>(d_group3).toStdString(), "item_0.item_0.item_0.item_0");
  EXPECT_EQ(calculatePath<S::INDEX>(d_group4).toStdString(), "item_0.item_0.item_0.item_0.item_0");

  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(configuration), configuration),
            configuration);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(data1), configuration), data1);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(data2), configuration), data2);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(data3), configuration), data3);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(data4), configuration), data4);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(data5), configuration), data5);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(d_group1), configuration), d_group1);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(d_group2), configuration), d_group2);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(d_group3), configuration), d_group3);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(d_group4), configuration), d_group4);
}

TEST_F(UtilitiesTests, treeCheckGuiIndex)
{
  using S = yoyo::utilities::path_strategy_t;
  using yoyo::utilities::calculatePath;
  using yoyo::utilities::get_root;
  using yoyo::utilities::retrieveFromPath;

  auto configuration = setupConfiguration();

  auto g_group1 = createGui<gui_container_mock>(configuration->childAt(1));
  auto g_group2 = createGui<gui_container_mock>(g_group1);
  auto g_group3 = createGui<gui_container_mock>(g_group2);
  auto g_group4 = createGui<gui_container_mock>(g_group3);
  auto gui1 = createGui<gui_mock>(configuration->childAt(1));
  auto gui2 = createGui<gui_mock>(g_group1);
  auto gui3 = createGui<gui_mock>(g_group2);
  auto gui4 = createGui<gui_mock>(g_group3);
  auto gui5 = createGui<gui_mock>(g_group4);

  EXPECT_EQ(get_root(g_group1), configuration);
  EXPECT_EQ(get_root(g_group2), configuration);
  EXPECT_EQ(get_root(g_group3), configuration);
  EXPECT_EQ(get_root(g_group4), configuration);
  EXPECT_EQ(get_root(gui1), configuration);
  EXPECT_EQ(get_root(gui2), configuration);
  EXPECT_EQ(get_root(gui3), configuration);
  EXPECT_EQ(get_root(gui4), configuration);
  EXPECT_EQ(get_root(gui5), configuration);

  EXPECT_EQ(calculatePath<S::INDEX>(gui1).toStdString(), "item_1.item_1");
  EXPECT_EQ(calculatePath<S::INDEX>(gui2).toStdString(), "item_1.item_0.item_1");
  EXPECT_EQ(calculatePath<S::INDEX>(gui3).toStdString(), "item_1.item_0.item_0.item_1");
  EXPECT_EQ(calculatePath<S::INDEX>(gui4).toStdString(), "item_1.item_0.item_0.item_0.item_1");
  EXPECT_EQ(calculatePath<S::INDEX>(gui5).toStdString(),
            "item_1.item_0.item_0.item_0.item_0.item_0");
  EXPECT_EQ(calculatePath<S::INDEX>(g_group1).toStdString(), "item_1.item_0");
  EXPECT_EQ(calculatePath<S::INDEX>(g_group2).toStdString(), "item_1.item_0.item_0");
  EXPECT_EQ(calculatePath<S::INDEX>(g_group3).toStdString(), "item_1.item_0.item_0.item_0");
  EXPECT_EQ(calculatePath<S::INDEX>(g_group4).toStdString(), "item_1.item_0.item_0.item_0.item_0");

  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(gui1), configuration), gui1);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(gui2), configuration), gui2);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(gui3), configuration), gui3);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(gui4), configuration), gui4);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(gui5), configuration), gui5);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(g_group1), configuration), g_group1);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(g_group2), configuration), g_group2);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(g_group3), configuration), g_group3);
  EXPECT_EQ(retrieveFromPath<S::INDEX>(calculatePath<S::INDEX>(g_group4), configuration), g_group4);
}

#include "treeUtilititesTests.moc"
