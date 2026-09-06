#pragma once

#include <boost/uuid/uuid.hpp>
#include <gtest/gtest.h>

namespace yoyo::test
{
class qtCoreFixture : public testing::Test
{
public:
  auto SetUp() -> void override;
  auto TearDown() -> void override;
};
} // namespace yoyo::test
