#include "PropertyWidgetFixture.hpp"

#include "command_handler.h"

auto PropertyWidgetTest::SetUp() -> void
{
  yoyo::command::initialize_commandhandler(new yoyo::command::command_handler(100));
  yoyo::command::commandhandler()->setEditMode(true);
}

auto PropertyWidgetTest::createWidget() -> void
{
  _widget = std::make_shared<yoyo::gui::property_widget>();
  _widget->resize(50, 200);
  _widget->show();
}

boost::uuids::uuid const mock_node::_id {};
