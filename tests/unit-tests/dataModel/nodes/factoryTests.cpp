#include "guiFixture.hpp"

#include "factory_provider.hpp"

#include "yoyo/data_node.h"
#include "yoyo/gui_node.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <QMetaProperty>

class FactoryProviderTests : public yoyo::test::guiFixture
{
};

TEST_F(FactoryProviderTests, dataNodes)
{
  yoyo::node_factory factory(&yoyo::data_node::staticMetaObject);

  EXPECT_THAT(factory.installed_nodes(), testing::SizeIs(0));
  yoyo::install_data_nodes(factory);

  EXPECT_THAT(factory.installed_nodes(), testing::SizeIs(5));

  for (auto node_id : factory.installed_nodes()) {
    auto doc = factory.node_documentation(std::get<0>(node_id));
    ASSERT_NE(doc, nullptr) << "no documentation for " << std::get<2>(node_id).toStdString();
    EXPECT_EQ(doc->type(), std::get<2>(node_id));

    auto node = factory.createNode(std::get<0>(node_id), {});
    ASSERT_NE(node, nullptr) << "no node created for " << std::get<2>(node_id).toStdString();

    auto meta = node->metaObject();

    for (int i = yoyo::node_base::staticMetaObject.propertyOffset(); i < meta->propertyCount();
         i++) {
      auto prop = doc->property(meta->property(i).name());
      EXPECT_NE(std::get<0>(prop), "") << "title empty for property " << meta->property(i).name()
                                       << " of " << std::get<2>(node_id).toStdString();
      EXPECT_NE(std::get<1>(prop), "")
        << "description empty for property " << meta->property(i).name() << " of "
        << std::get<2>(node_id).toStdString();
      EXPECT_NE(std::get<2>(prop), "") << "toolTip empty for property " << meta->property(i).name()
                                       << " of " << std::get<2>(node_id).toStdString();
    }
  }
}

TEST_F(FactoryProviderTests, guiNodes)
{
  yoyo::node_factory factory(&yoyo::gui_node::staticMetaObject);

  EXPECT_THAT(factory.installed_nodes(), testing::SizeIs(0));
  yoyo::install_gui_nodes(factory);

  EXPECT_THAT(factory.installed_nodes(), testing::SizeIs(5));

  for (auto node_id : factory.installed_nodes()) {
    auto doc = factory.node_documentation(std::get<0>(node_id));
    ASSERT_NE(doc, nullptr) << "no documentation for " << std::get<2>(node_id).toStdString();
    EXPECT_EQ(doc->type(), std::get<2>(node_id));

    auto node = factory.createNode(std::get<0>(node_id), {});
    ASSERT_NE(node, nullptr) << "no node created for " << std::get<2>(node_id).toStdString();

    auto meta = node->metaObject();

    for (int i = yoyo::node_base::staticMetaObject.propertyOffset(); i < meta->propertyCount();
         i++) {
      auto prop = doc->property(meta->property(i).name());
      EXPECT_NE(std::get<0>(prop), "") << "title empty for property " << meta->property(i).name()
                                       << " of " << std::get<2>(node_id).toStdString();
      EXPECT_NE(std::get<1>(prop), "")
        << "description empty for property " << meta->property(i).name() << " of "
        << std::get<2>(node_id).toStdString();
      EXPECT_NE(std::get<2>(prop), "") << "toolTip empty for property " << meta->property(i).name()
                                       << " of " << std::get<2>(node_id).toStdString();
    }
  }
}

TEST_F(FactoryProviderTests, fundamentalNodes)
{
  yoyo::node_factory factory(&yoyo::node_base::staticMetaObject);

  EXPECT_THAT(factory.installed_nodes(), testing::SizeIs(0));
  yoyo::install_fundamental_nodes(factory);

  EXPECT_THAT(factory.installed_nodes(), testing::SizeIs(8));

  for (auto node_id : factory.installed_nodes()) {
    auto doc = factory.node_documentation(std::get<0>(node_id));
    ASSERT_NE(doc, nullptr) << "no documentation for " << std::get<2>(node_id).toStdString();
    EXPECT_EQ(doc->type(), std::get<2>(node_id));

    auto node = factory.createNode(std::get<0>(node_id), {});
    ASSERT_NE(node, nullptr) << "no node created for " << std::get<2>(node_id).toStdString();

    auto meta = node->metaObject();
    if (std::get<boost::uuids::uuid>(node_id) == yoyo::fundamental::configuration_id) {
      // No property documentation needed for configuration.
      continue;
    }

    for (int i = yoyo::node_base::staticMetaObject.propertyOffset(); i < meta->propertyCount();
         i++) {
      auto prop = doc->property(meta->property(i).name());
      EXPECT_NE(std::get<0>(prop), "") << "title empty for property " << meta->property(i).name()
                                       << " of " << std::get<2>(node_id).toStdString();
      EXPECT_NE(std::get<1>(prop), "")
        << "description empty for property " << meta->property(i).name() << " of "
        << std::get<2>(node_id).toStdString();
      EXPECT_NE(std::get<2>(prop), "") << "toolTip empty for property " << meta->property(i).name()
                                       << " of " << std::get<2>(node_id).toStdString();
    }
  }
}
