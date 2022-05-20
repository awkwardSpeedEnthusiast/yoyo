#include "leaf_node.hpp"

#include "guiFixture.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QApplication>

#include <memory>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;
using testing::WithArg;

class leaf_node_instance : public yoyo::leaf_node
{
public:
  using leaf_node::leaf_node;
  auto supportsValueType(yoyo::types::value_t) const -> bool
  {
    return false;
  }
};

class leafNodeTest : public yoyo::test::guiFixture
{
};

TEST_F(leafNodeTest, hierarchy)
{
  auto object = std::make_shared<leaf_node_instance>("mock", nullptr);
  object->init();

  // Accept no children
  EXPECT_FALSE(object->acceptsChildren());
  EXPECT_FALSE(object->acceptsChild(nullptr));

  // Accepts all parents, even nullptrs (which doesn't really make sense, but whatever)
  EXPECT_TRUE(object->acceptsParent(nullptr));

  // It is never meant to be instantiated directly, so there is no type-id
  EXPECT_TRUE(object->staticTypeId().is_nil());

  // The before/After Child Add/Remove/Mode methods cannot be tested, since they will
  // never be called. So, bad news for coverage here.
}
