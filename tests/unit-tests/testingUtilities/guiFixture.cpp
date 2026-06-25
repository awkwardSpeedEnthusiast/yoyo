#include "guiFixture.hpp"
#include "gui_node_mock.h"

#include <QApplication>

namespace
{
std::unique_ptr<QApplication> app;
char* arg0 = new char[8];
char** args = []() {
  auto c = new char*[1];
  c[0] = arg0;
  return c;
}();
int argc = 1;
} // namespace

namespace yoyo::test
{

auto guiFixture::SetUp() -> void
{
  testing::Test::SetUp();

  if (!qApp) {
    strcpy(arg0, "blabla\0");
    app = std::make_unique<QApplication>(argc, args);
  }
}

auto guiFixture::TearDown() -> void
{
  app.reset();
  testing::Test::TearDown();
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
