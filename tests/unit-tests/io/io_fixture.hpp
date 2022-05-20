#include "yoyo/node_base.h"
#include "yoyo/node_factory.h"

#include <gtest/gtest.h>

/// To test IO fully, we ned to define a set of classes and install them into node factories.
/// We need different classes with different sets of properties and their respective documentations
/// to allow for omitting non-default values when writing.
///
/// To get a bit of randomness inside the tests, the trees will be created with a randomization
/// algorithm and the properties will not only be filled using random values, but also randomly
/// whether they get values differing from the default value.
/// We cannot test everything, but with this, we can make sure, that at least for the defined
/// property types, we touch all possible combinations over time.
///
/// \todo when adding new property types, make sure, that they are added to at least one of the
/// node types defined here or create a new node type.

class IoFixture : public testing::Test
{
public:
  using testing::Test::Test;
  auto SetUp() -> void override;
  auto TearDown() -> void override;
  auto installNodes() -> void;
  auto compareNodes(std::shared_ptr<yoyo::node_base> actual,
                    std::shared_ptr<yoyo::node_base> expected) -> void;
  auto expectValuesEqual(QVariant const& v1, QVariant const& v2) -> void;
  auto setNodeValues(std::shared_ptr<yoyo::node_base> node) -> void;
  auto createRandomNode() -> std::shared_ptr<yoyo::node_base>;
  auto roll(int min, int max) -> int;

  std::vector<std::shared_ptr<yoyo::node_factory>> _factories;
  std::vector<std::pair<boost::uuids::uuid, int>> _ids;
};
