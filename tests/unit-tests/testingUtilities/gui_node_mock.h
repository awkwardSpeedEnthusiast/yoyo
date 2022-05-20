#pragma once
#include "yoyo/gui_node.h"

#include <gmock/gmock.h>

namespace yoyo::test
{
class gui_node_mock : public gui_node
{
  Q_OBJECT
  Q_PROPERTY(yoyo::properties::connection_t connection READ connection WRITE setConnection NOTIFY
               connectionChanged)
public:
  using gui_node::gui_node;
  ~gui_node_mock()
  {
    destructorCalled();
  }

  MOCK_METHOD(void, destructorCalled, (), ());
  MOCK_METHOD(bool, acceptsChildren, (), (const, override));
  MOCK_METHOD(bool, acceptsChild, (std::shared_ptr<node_base> const& child), (const, override));
  MOCK_METHOD(bool, acceptsParent, (std::shared_ptr<node_base> const& parent), (const, override));
  MOCK_METHOD(bool, beforeChildAdd, (std::shared_ptr<node_base> const& child), (override));
  MOCK_METHOD(bool, afterChildAdd, (std::shared_ptr<node_base> const& child), (override));
  MOCK_METHOD(bool, beforeChildRemove, (std::shared_ptr<node_base> const& child), (override));
  MOCK_METHOD(bool, afterChildRemove, (std::shared_ptr<node_base> const& child), (override));
  MOCK_METHOD(void, afterChildMoved, (std::shared_ptr<node_base> const&, size_t, size_t),
              (override));
  MOCK_METHOD(bool, supportsValueType, (types::value_t t), (const, override));
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;

  auto connection() const -> yoyo::properties::connection_t
  {
    return _connection;
  }
  auto setConnection(yoyo::properties::connection_t c) -> void
  {
    if (c == _connection) {
      return;
    }

    _connection._in = c._in;
    _connection._out = c._out;
    _connection._auto = c._auto;
    Q_EMIT connectionChanged(_connection);
  }

Q_SIGNALS:
  void connectionChanged(yoyo::properties::connection_t c);

private:
  yoyo::properties::connection_t _connection;
};
} // namespace yoyo::test
