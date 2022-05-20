#include "yoyo/data_container_node.h"
#include "yoyo/data_node.h"
#include "yoyo/data_value_node.h"
#include "yoyo/property_types.h"

#include <QDebug>
#include <QMetaMethod>
#include <QMetaProperty>
#include <QSignalSpy>
#include <QVariant>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/uuid/random_generator.hpp>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;
using testing::WithArg;

namespace yoyo
{
class data_mock : public data_node
{
  Q_OBJECT
public:
  using data_node::data_node;
  MOCK_METHOD(bool, acceptsChildren, (), (const, override));
  MOCK_METHOD(bool, acceptsChild, (std::shared_ptr<node_base> const& child), (const, override));
  MOCK_METHOD(bool, acceptsParent, (std::shared_ptr<node_base> const& parent), (const, override));
  MOCK_METHOD(bool, childAboutToBeAdded, (std::shared_ptr<node_base> const& child), (override));
  MOCK_METHOD(bool, childAdded, (std::shared_ptr<node_base> const& child), (override));
  MOCK_METHOD(bool, childAboutToBeRemoved, (std::shared_ptr<node_base> const& child), (override));
  MOCK_METHOD(bool, childRemoved, (std::shared_ptr<node_base> const& child), (override));
  MOCK_METHOD(void, childMoved,
              (std::shared_ptr<node_base> const& child, size_t from_index, size_t to_index),
              (override));
  MOCK_METHOD(void, setValue, (QVariant const& v), (override));
  MOCK_METHOD(types::variant_t, value, (), (const, override));
  using data_node::emitValueChanged;
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
};
boost::uuids::uuid data_mock::_id;

class data_container_mock : public data_container_node
{
  Q_OBJECT
public:
  using data_container_node::data_container_node;
  MOCK_METHOD(types::variant_t, value, (), (const, override));
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
};
boost::uuids::uuid data_container_mock::_id;

class data_value_mock : public data_value_node
{
  Q_OBJECT
public:
  using data_value_node::data_value_node;
  MOCK_METHOD(types::variant_t, value, (), (const, override));
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
};
boost::uuids::uuid data_value_mock::_id;
} // namespace yoyo

TEST(dataNodeTest, properties)
{
  yoyo::registerAllPropertyTypes();
  struct Receiver {
    MOCK_METHOD(void, valueReceived, (QVariant v));
  } receiver;

  // Connectable
  {
    auto data = std::make_shared<testing::StrictMock<yoyo::data_mock>>(
      QString("basic"), boost::uuids::uuid {}, yoyo::types::value_t::INT8, true);

    EXPECT_EQ(data->valueType(), yoyo::types::value_t::INT8);
    auto vt = data->property("valueType");
    EXPECT_EQ(vt.value<yoyo::types::value_t>(), yoyo::types::value_t::INT8);

    EXPECT_TRUE(data->canConnect());

    boost::signals2::scoped_connection connection =
      data->connectToValue([&receiver](auto v) { receiver.valueReceived(v); });
    EXPECT_TRUE(connection.connected());

    EXPECT_CALL(receiver, valueReceived(QVariant(3)));
    data->emitValueChanged(QVariant(3));
  }
  // Non connectable
  {
    auto data = std::make_shared<testing::StrictMock<yoyo::data_mock>>(
      QString("non connectable"), boost::uuids::uuid {}, yoyo::types::value_t::CUSTOM, false);

    EXPECT_EQ(data->valueType(), yoyo::types::value_t::CUSTOM);
    auto vt = data->property("valueType");
    EXPECT_EQ(vt.value<yoyo::types::value_t>(), yoyo::types::value_t::CUSTOM);

    EXPECT_FALSE(data->canConnect());

    boost::signals2::scoped_connection connection =
      data->connectToValue([&receiver](auto v) { receiver.valueReceived(v); });
    EXPECT_FALSE(connection.connected());

    EXPECT_CALL(receiver, valueReceived(QVariant(3))).Times(0);
    data->emitValueChanged(QVariant(3));
  }
}

TEST(dataContainerNode, basic)
{
  auto data = std::make_shared<testing::StrictMock<yoyo::data_container_mock>>(
    "container", boost::uuids::uuid {}, yoyo::types::value_t::CUSTOM);

  EXPECT_EQ(data->valueType(), yoyo::types::value_t::CUSTOM);
  auto vt = data->property("valueType");
  EXPECT_EQ(vt.value<yoyo::types::value_t>(), yoyo::types::value_t::CUSTOM);

  // Containers typically cannot connect.
  EXPECT_FALSE(data->canConnect());

  // Accepts all parents and children indiscriminating
  EXPECT_TRUE(data->acceptsChildren());
  EXPECT_TRUE(data->acceptsChild({}));
  EXPECT_TRUE(data->acceptsParent({}));
}

TEST(dataValueNode, basic)
{
  auto data = std::make_shared<testing::StrictMock<yoyo::data_value_mock>>(
    "value", boost::uuids::uuid {}, yoyo::types::value_t::BIT);
  QSignalSpy titleSpy(data.get(), &yoyo::data_value_node::titleChanged);
  QSignalSpy unitSpy(data.get(), &yoyo::data_value_node::unitChanged);
  QSignalSpy accessSpy(data.get(), &yoyo::data_value_node::accessChanged);
  QSignalSpy onChangeSpy(data.get(), &yoyo::data_value_node::onChangeChanged);

  EXPECT_EQ(data->valueType(), yoyo::types::value_t::BIT);
  auto vt = data->property("valueType");
  EXPECT_EQ(vt.value<yoyo::types::value_t>(), yoyo::types::value_t::BIT);

  EXPECT_EQ(data->title(), "");
  vt = data->property("title");
  EXPECT_EQ(vt.toString(), "");
  data->setProperty("title", "");
  EXPECT_EQ(titleSpy.count(), 0);
  data->setProperty("title", "my Title");
  EXPECT_EQ(titleSpy.count(), 1);
  EXPECT_EQ(data->title(), "my Title");

  yoyo::properties::invisible_string_t unit { "", true };
  EXPECT_EQ(data->unit(), unit);
  vt = data->property("unit");
  EXPECT_EQ(vt.value<yoyo::properties::invisible_string_t>(), unit);
  data->setProperty("unit", QVariant::fromValue(unit));
  EXPECT_EQ(unitSpy.count(), 0);
  unit._s = "my Unit";
  data->setProperty("unit", QVariant::fromValue(unit));
  EXPECT_EQ(unitSpy.count(), 1);
  EXPECT_EQ(data->unit(), unit);

  EXPECT_EQ(data->access(), yoyo::types::access_t::READ_WRITE);
  vt = data->property("access");
  EXPECT_EQ(vt.value<yoyo::types::access_t>(), yoyo::types::access_t::READ_WRITE);
  data->setProperty("access", QVariant::fromValue(yoyo::types::access_t::READ_WRITE));
  EXPECT_EQ(accessSpy.count(), 0);
  data->setProperty("access", QVariant::fromValue(yoyo::types::access_t::WRITE));
  EXPECT_EQ(accessSpy.count(), 1);
  EXPECT_TRUE(data->access() == yoyo::types::access_t::WRITE);

  EXPECT_EQ(data->onChange(), yoyo::properties::script_t {});
  vt = data->property("onChange");
  EXPECT_EQ(vt.value<yoyo::properties::script_t>(), yoyo::properties::script_t {});
  data->setProperty("onChange", QVariant::fromValue(yoyo::properties::script_t {}));
  EXPECT_EQ(onChangeSpy.count(), 0);
  yoyo::properties::script_t oc { "foo", yoyo::properties::script_t::type::JS };
  data->setProperty("onChange", QVariant::fromValue(oc));
  EXPECT_EQ(onChangeSpy.count(), 1);
  EXPECT_TRUE(data->onChange() == oc);

  // Value nodes are able to connect
  EXPECT_TRUE(data->canConnect());

  // Accepts all parents and children indiscriminating
  EXPECT_FALSE(data->acceptsChildren());
  EXPECT_FALSE(data->acceptsChild({}));
  EXPECT_TRUE(data->acceptsParent({}));
}

TEST(dataValueNode, connections)
{
  auto data = std::make_shared<testing::StrictMock<yoyo::data_value_mock>>(
    "value", boost::uuids::uuid {}, yoyo::types::value_t::BIT);

  /// \todo add onChange emission check

  struct Receiver {
    MOCK_METHOD(void, valueReceived, (QVariant v));
    MOCK_METHOD(void, scriptExecutionRequested, (yoyo::properties::script_t s));
  } receiver;
  boost::signals2::scoped_connection connection =
    data->connectToValue([&receiver](auto v) { receiver.valueReceived(v); });
  QObject::connect(data.get(), &yoyo::data_node::scriptExecutionRequested,
                   [&receiver](auto s) { receiver.scriptExecutionRequested(s); });
  EXPECT_TRUE(connection.connected());

  // Write enabled:
  EXPECT_CALL(receiver, scriptExecutionRequested(_)).Times(0);
  EXPECT_CALL(receiver, valueReceived(QVariant(1)));
  data->setValue(QVariant(1));

  // Read only:
  data->setAccess(yoyo::types::access_t::READ);
  EXPECT_CALL(receiver, valueReceived(_)).Times(0);
  data->setValue(QVariant(0));

  // On change script set:
  yoyo::properties::script_t script { "foo", yoyo::properties::script_t::type::JS };
  data->setOnChange(script);
  data->setAccess(yoyo::types::access_t::READ_WRITE);
  testing::Sequence seq;
  EXPECT_CALL(receiver, valueReceived(QVariant(42))).InSequence(seq);
  EXPECT_CALL(receiver, scriptExecutionRequested(script)).InSequence(seq);
  data->setValue(QVariant(42));
}

TEST(dataNodes, hierarchy)
{
  auto data_value = std::make_shared<testing::StrictMock<yoyo::data_value_mock>>(
    "value", boost::uuids::uuid {}, yoyo::types::value_t::BIT);
  auto data_container = std::make_shared<testing::StrictMock<yoyo::data_container_mock>>(
    "container", boost::uuids::uuid {}, yoyo::types::value_t::CUSTOM);

  data_value->addChild(data_container);
  EXPECT_EQ(data_value->childCount(), 0);

  data_container->addChild(data_value);
  EXPECT_EQ(data_container->childCount(), 1);

  data_container->removeChild(data_value);
  EXPECT_EQ(data_container->childCount(), 0);
}

#include "data_node_tests.moc"
