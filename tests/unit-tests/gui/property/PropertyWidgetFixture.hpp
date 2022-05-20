#include "property_widget.h"

#include "guiFixture.hpp"

#include "yoyo/node_base.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class PropertyWidgetTest : public yoyo::test::guiFixture
{
public:
  auto SetUp() -> void override;
  auto createWidget() -> void;

  std::shared_ptr<yoyo::gui::property_widget> _widget;
};

class mock_node : public yoyo::node_base
{
  Q_OBJECT
public:
  using yoyo::node_base::node_base;

  auto acceptsChildren() const -> bool override
  {
    return false;
  }
  auto acceptsChild(std::shared_ptr<node_base> const& /*child*/) const -> bool override
  {
    return false;
  }
  auto acceptsParent(std::shared_ptr<node_base> const& /*parent*/) const -> bool override
  {
    return true;
  }
  auto childAboutToBeAdded(std::shared_ptr<node_base> const& /*child*/) -> bool override
  {
    return false;
  }
  auto childAdded(std::shared_ptr<node_base> const& /*child*/) -> bool override
  {
    return false;
  }
  auto childAboutToBeRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool override
  {
    return false;
  }
  auto childRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool override
  {
    return false;
  }
  auto childMoved(std::shared_ptr<node_base> const& /*child*/, size_t /*from_index*/,
                  size_t /*to_index*/) -> void override
  {
  }
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }

  static boost::uuids::uuid const _id;
};

#define DECLARE_PROPERTY(TYPE, NAME)                                                               \
public:                                                                                            \
  MOCK_METHOD(TYPE, NAME, (), (const));                                                            \
  MOCK_METHOD(void, set_##NAME, (TYPE v));                                                         \
Q_SIGNALS:                                                                                         \
  void NAME##Changed(TYPE v)
