#include "PropertyWidgetFixture.hpp"

#include "command_handler.h"

#include <QScrollArea>

#include <string>

using std::string_literals::operator""s;

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

  ASSERT_EQ(_widget->children().count(), 5);
  ASSERT_EQ(_widget->children()[4]->metaObject()->className(), "QWidget"s);
  ASSERT_EQ(_widget->children()[4]->children().count(), 2);
  ASSERT_EQ(_widget->children()[4]->children()[0]->metaObject()->className(), "QVBoxLayout"s);
  ASSERT_EQ(_widget->children()[4]->children()[1]->metaObject()->className(), "QScrollArea"s);
  auto scrollarea = dynamic_cast<QScrollArea*>(_widget->children()[4]->children()[1]);
  ASSERT_NE(scrollarea, nullptr);

  ASSERT_EQ(scrollarea->viewport()->children().count(), 1);
  EXPECT_EQ(scrollarea->viewport()->children()[0]->metaObject()->className(), "QWidget"s);
  _content = dynamic_cast<QWidget*>(scrollarea->viewport()->children()[0]);
}

auto PropertyWidgetTest::get_child(size_t index) -> QWidget*
{
  if (!_content || static_cast<size_t>(_content->children().count()) <= index) {
    return nullptr;
  }
  return dynamic_cast<QWidget*>(_content->children().at(index));
}

auto PropertyWidgetTest::child_count() -> size_t
{
  if (!_content) {
    return 0;
  }
  return _content->children().count();
}

boost::uuids::uuid const mock_node::_id {};
