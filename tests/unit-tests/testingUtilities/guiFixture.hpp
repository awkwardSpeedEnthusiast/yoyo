#pragma once
#include <QTest>

#include <boost/uuid/uuid.hpp>
#include <gtest/gtest.h>

namespace yoyo
{
class gui_node;
} // namespace yoyo
namespace yoyo::test
{
class guiFixture : public testing::Test
{
public:
  auto SetUp() -> void override;
  auto TearDown() -> void override;

  auto createGuiNode(QString const& type_name, QWidget* container = nullptr,
                     boost::uuids::uuid const& id = boost::uuids::uuid {})
    -> std::shared_ptr<yoyo::gui_node>;
  auto tearDownNodes(std::vector<std::shared_ptr<yoyo::gui_node>> nodes) -> void;

private:
};
} // namespace yoyo::test
