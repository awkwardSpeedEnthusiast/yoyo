#include "qt_core_fixture.hpp"

#include <QCoreApplication>

namespace
{
std::unique_ptr<QCoreApplication> app;
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
auto qtCoreFixture::SetUp() -> void
{
  testing::Test::SetUp();

  if (!qApp) {
    strcpy(arg0, "blabla\0");
    app = std::make_unique<QCoreApplication>(argc, args);
  }
  const testing::TestInfo* const test_info = testing::UnitTest::GetInstance()->current_test_info();
  QCoreApplication::setApplicationName(
    QString("%1_%2").arg(test_info->test_suite_name()).arg(test_info->name()));
  QCoreApplication::setOrganizationName("Tiegerzahn LTD");
}

auto qtCoreFixture::TearDown() -> void
{
  app.reset();
  testing::Test::TearDown();
}

} // namespace yoyo::test
