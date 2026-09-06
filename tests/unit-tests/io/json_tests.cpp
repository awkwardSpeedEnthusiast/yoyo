#include "io_fixture.hpp"
#include "json/node_deserializer_json.hpp"
#include "json/node_serializer_json.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sstream>

class JsonIoTests : public IoFixture
{
public:
  auto SetUp() -> void override
  {
    IoFixture::SetUp();
    _serializer = std::make_shared<yoyo::io::node_serializer_json>(_factories);
    _deserializer = std::make_shared<yoyo::io::node_deserializer_json>(_factories);
  }

  std::shared_ptr<yoyo::io::node_serializer_json> _serializer;
  std::shared_ptr<yoyo::io::node_deserializer_json> _deserializer;
};

TEST_F(JsonIoTests, singleNode)
{
  for (auto item : _ids) {
    ASSERT_THAT(_factories, testing::SizeIs(4));
    auto node = _factories[item.second]->createNode(item.first, {});
    ASSERT_NE(node, nullptr);
    setNodeValues(node);

    std::stringstream inStream;

    _serializer->serialize(inStream, node);

    std::stringstream outStream(inStream.str());

    auto readNode = _deserializer->deserialize(outStream);
    ASSERT_NE(readNode, nullptr);
    compareNodes(readNode, node);
  }
}

TEST_F(JsonIoTests, defaultValues)
{
  for (auto item : _ids) {
    ASSERT_THAT(_factories, testing::SizeIs(4));
    auto node = _factories[item.second]->createNode(item.first, {});
    ASSERT_NE(node, nullptr);

    std::stringstream inStream_default;
    _serializer->serialize(inStream_default, node);

    std::stringstream outStream(inStream_default.str());

    auto readNode = _deserializer->deserialize(outStream);
    ASSERT_NE(readNode, nullptr);
    compareNodes(readNode, node);

    setNodeValues(node);
    std::stringstream inStream_set;
    _serializer->serialize(inStream_set, node);

    EXPECT_GE(inStream_set.str().length(), inStream_default.str().length());
  }
}

TEST_F(JsonIoTests, hierarchy)
{
  auto node = _factories[_ids[0].second]->createNode(_ids[0].first, {});
  setNodeValues(node);
  ASSERT_EQ(node->type(), "dummy group");

  auto childCount = roll(0, 10);

  for (auto i = 0; i < childCount; i++) {
    node->addChild(createRandomNode());
  }

  EXPECT_EQ(node->childCount(), childCount);

  std::stringstream inStream;

  _serializer->serialize(inStream, node);

  std::stringstream outStream(inStream.str());

  auto readNode = _deserializer->deserialize(outStream);
  ASSERT_NE(readNode, nullptr);
  compareNodes(readNode, node);
}

TEST_F(JsonIoTests, deepHierarchy)
{
  auto node = _factories[_ids[0].second]->createNode(_ids[0].first, {});
  setNodeValues(node);
  ASSERT_EQ(node->type(), "dummy group");

  auto n = node;

  for (auto j = 0; j < 5; j++) {
    auto childCount = roll(0, 10);

    for (auto i = 0; i < childCount; i++) {
      n->addChild(createRandomNode());
    }

    auto group = _factories[_ids[0].second]->createNode(_ids[0].first, {});
    n->addChild(group);
  }

  std::stringstream inStream;

  _serializer->serialize(inStream, node);

  std::stringstream outStream(inStream.str());

  auto readNode = _deserializer->deserialize(outStream);
  ASSERT_NE(readNode, nullptr);
  compareNodes(readNode, node);
}
