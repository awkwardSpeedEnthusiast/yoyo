#include "guiFixture.hpp"
#include "gui_node_mock.h"

namespace yoyo::test
{
char* guiFixture::arg0 = new char[7];
int guiFixture::argc = 1;
std::unique_ptr<QApplication> guiFixture::app;

auto guiFixture::SetUpTestSuite() -> void
{
  testing::Test::SetUpTestSuite();

  if (!qApp) {
    strcpy(arg0, "blabla\0");
    char* args[1] = { arg0 };
    app = std::make_unique<QApplication>(argc, args);
  }
}

auto guiFixture::TearDownTestSuite() -> void
{
  testing::Test::TearDownTestSuite();
}

auto guiFixture::createGuiNode(QString const& type_name, QWidget* container,
                               boost::uuids::uuid const& id) -> std::shared_ptr<yoyo::gui_node>
{
  auto object = std::make_shared<testing::StrictMock<gui_node_mock>>(type_name, container, id);
  object->init();
  return object;
}

auto guiFixture::tearDownNodes(std::vector<std::shared_ptr<yoyo::gui_node>> nodes) -> void
{
  for (auto n : nodes) {
    testing::Mock::VerifyAndClear(n.get());
    EXPECT_CALL(*std::dynamic_pointer_cast<test::gui_node_mock>(n), destructorCalled());
  }

  nodes.clear();
  QApplication::processEvents();
}

boost::uuids::uuid gui_node_mock::_id;
} // namespace yoyo::test
