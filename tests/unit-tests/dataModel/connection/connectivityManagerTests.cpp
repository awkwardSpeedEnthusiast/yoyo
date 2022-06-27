#include "guiFixture.hpp"

#include "configuration.hpp"
#include "connectivity_manager.hpp"
#include "factory_provider.hpp"

#include "yoyo/data_container_node.h"
#include "yoyo/data_node.h"
#include "yoyo/data_value_node.h"
#include "yoyo/gui_node.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <QMetaProperty>

#include <boost/uuid/random_generator.hpp>

using testing::_;
using testing::Invoke;
using testing::Return;

class leaf_gui : public yoyo::gui_node
{
  Q_OBJECT

public:
  using yoyo::gui_node::gui_node;
  auto acceptsChildren() const -> bool final
  {
    return false;
  }
  auto acceptsChild(std::shared_ptr<node_base> const&) const -> bool final
  {
    return false;
  }
  auto acceptsParent(std::shared_ptr<node_base> const&) const -> bool override
  {
    return true;
  }

private:
  auto beforeChildAdd(std::shared_ptr<node_base> const&) -> bool final
  {
    return false;
  }
  auto afterChildAdd(std::shared_ptr<node_base> const&) -> bool final
  {
    return false;
  }
  auto beforeChildRemove(std::shared_ptr<node_base> const&) -> bool final
  {
    return false;
  }
  auto afterChildRemove(std::shared_ptr<node_base> const&) -> bool final
  {
    return false;
  }
  auto afterChildMoved(std::shared_ptr<node_base> const&, size_t, size_t) -> void final {}
};

class gui_type_1 : public leaf_gui
{
  Q_OBJECT
  Q_PROPERTY(yoyo::properties::connection_t connection READ connection WRITE setConnection NOTIFY
               connectionChanged)
  Q_PROPERTY(
    yoyo::properties::connected_string_t title READ title WRITE setTitle NOTIFY titleChanged)
public:
  gui_type_1()
    : leaf_gui { "test_gui_1", nullptr, boost::uuids::uuid {} }
    , _c { [this](std::function<void(QVariant)> s) { return _s.connect(s); },
           [this](QVariant v) { setter(v); },
           [this](yoyo::types::value_t v) { return supportsValueType(v); },
           "",
           "",
           "" }
  {
  }
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _typeId;
  }
  MOCK_METHOD(void, setter, (QVariant v));
  MOCK_METHOD(bool, supportsValueType, (yoyo::types::value_t t), (const, override));
  MOCK_METHOD(yoyo::properties::connected_string_t, title, (), (const));
  MOCK_METHOD(void, setTitle, (yoyo::properties::connected_string_t));
  auto set(QVariant v)
  {
    _s(v);
  }
  auto connection() const -> yoyo::properties::connection_t
  {
    return _c;
  }
  auto setConnection(yoyo::properties::connection_t c)
  {
    if (c != _c) {
      _c.assign(c);
      Q_EMIT(connectionChanged(_c));
    }
  }

Q_SIGNALS:
  void connectionChanged(yoyo::properties::connection_t c);
  void titleChanged(yoyo::properties::connected_string_t);

private:
  static boost::uuids::uuid const _typeId;
  yoyo::properties::connection_t _c;
  boost::signals2::signal<void(QVariant)> _s;
};

class gui_type_2 : public leaf_gui
{
  Q_OBJECT
  Q_PROPERTY(yoyo::properties::in_connection_t connection READ connection WRITE setConnection NOTIFY
               connectionChanged)
  Q_PROPERTY(
    yoyo::properties::connected_string_t title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(yoyo::properties::connected_string_t unit READ unit WRITE setUnit NOTIFY unitChanged)
public:
  gui_type_2()
    : leaf_gui { "test_gui_2", nullptr, boost::uuids::uuid {} }
    , _c { [this](QVariant v) { setter(v); },
           [this](yoyo::types::value_t v) { return supportsValueType(v); }, "" }
  {
  }
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _typeId;
  }
  MOCK_METHOD(void, setter, (QVariant v));
  MOCK_METHOD(bool, supportsValueType, (yoyo::types::value_t t), (const, override));
  MOCK_METHOD(yoyo::properties::connected_string_t, title, (), (const));
  MOCK_METHOD(void, setTitle, (yoyo::properties::connected_string_t));
  MOCK_METHOD(yoyo::properties::connected_string_t, unit, (), (const));
  MOCK_METHOD(void, setUnit, (yoyo::properties::connected_string_t));
  auto connection() const -> yoyo::properties::in_connection_t
  {
    return _c;
  }
  auto setConnection(yoyo::properties::in_connection_t c)
  {
    if (c != _c) {
      _c.assign(c);
      Q_EMIT(connectionChanged(_c));
    }
  }

Q_SIGNALS:
  void connectionChanged(yoyo::properties::in_connection_t c);
  void titleChanged(yoyo::properties::connected_string_t);
  void unitChanged(yoyo::properties::connected_string_t);

private:
  static boost::uuids::uuid const _typeId;
  yoyo::properties::in_connection_t _c;
};

class gui_type_3 : public leaf_gui
{
  Q_OBJECT
  Q_PROPERTY(yoyo::properties::out_connection_t connection READ connection WRITE setConnection
               NOTIFY connectionChanged)
  Q_PROPERTY(
    yoyo::properties::connected_string_t title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(yoyo::properties::connected_string_t unit READ unit WRITE setUnit NOTIFY unitChanged)
  Q_PROPERTY(yoyo::properties::enum_t enumValues READ enumValues WRITE setEnumValues NOTIFY
               enumValuesChanged)
public:
  gui_type_3()
    : leaf_gui { "test_gui_3", nullptr, boost::uuids::uuid {} }
    , _c { [this](std::function<void(QVariant)> s) { return _s.connect(s); },
           [this](yoyo::types::value_t v) { return supportsValueType(v); }, "" }
  {
  }
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _typeId;
  }
  MOCK_METHOD(void, setter, (QVariant v));
  MOCK_METHOD(bool, supportsValueType, (yoyo::types::value_t t), (const, override));
  MOCK_METHOD(yoyo::properties::connected_string_t, title, (), (const));
  MOCK_METHOD(void, setTitle, (yoyo::properties::connected_string_t));
  MOCK_METHOD(yoyo::properties::connected_string_t, unit, (), (const));
  MOCK_METHOD(void, setUnit, (yoyo::properties::connected_string_t));
  MOCK_METHOD(yoyo::properties::enum_t, enumValues, (), (const));
  MOCK_METHOD(void, setEnumValues, (yoyo::properties::enum_t));
  auto set(QVariant v)
  {
    _s(v);
  }
  auto connection() const -> yoyo::properties::out_connection_t
  {
    return _c;
  }
  auto setConnection(yoyo::properties::out_connection_t c)
  {
    if (c != _c) {
      _c.assign(c);
      Q_EMIT(connectionChanged(_c));
    }
  }

Q_SIGNALS:
  void connectionChanged(yoyo::properties::out_connection_t c);
  void titleChanged(yoyo::properties::connected_string_t);
  void unitChanged(yoyo::properties::connected_string_t);
  void enumValuesChanged(yoyo::properties::enum_t);

private:
  static boost::uuids::uuid const _typeId;
  yoyo::properties::out_connection_t _c;
  boost::signals2::signal<void(QVariant)> _s;
};

class data_type_1 : public yoyo::data_value_node
{
  Q_OBJECT
public:
  data_type_1()
    : yoyo::data_value_node { "data_type_1", {}, yoyo::types::value_t::BIT }
  {
  }
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _typeId;
  }
  MOCK_METHOD(yoyo::types::variant_t, value, (), (const, override));
  MOCK_METHOD(void, setValue, (QVariant const& value), (override));
  auto emitValue(QVariant v) -> void
  {
    emitValueChanged(v);
  }

private:
  static boost::uuids::uuid const _typeId;
};

class data_type_2 : public yoyo::data_value_node
{
  Q_OBJECT
  Q_PROPERTY(yoyo::properties::enum_t enumValues READ enumValues WRITE setEnumValues NOTIFY
               enumValuesChanged)
public:
  data_type_2()
    : yoyo::data_value_node { "data_type_2", {}, yoyo::types::value_t::UINT8 }
  {
  }
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _typeId;
  }
  MOCK_METHOD(yoyo::types::variant_t, value, (), (const, override));
  MOCK_METHOD(void, setValue, (QVariant const& value), (override));

  MOCK_METHOD(yoyo::properties::enum_t, enumValues, (), (const));
  MOCK_METHOD(void, setEnumValues, (yoyo::properties::enum_t));
  auto emitValue(QVariant v) -> void
  {
    emitValueChanged(v);
  }

Q_SIGNALS:
  void enumValuesChanged(yoyo::properties::enum_t);

private:
  static boost::uuids::uuid const _typeId;
};

class data_type_3 : public yoyo::data_value_node
{
  Q_OBJECT
public:
  data_type_3()
    : yoyo::data_value_node { "data_type_3", {}, yoyo::types::value_t::STRING }
  {
  }
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _typeId;
  }
  MOCK_METHOD(yoyo::types::variant_t, value, (), (const, override));
  MOCK_METHOD(void, setValue, (QVariant const& value), (override));

  auto emitValue(QVariant v) -> void
  {
    emitValueChanged(v);
  }

private:
  static boost::uuids::uuid const _typeId;
};

class data_container_mock : public yoyo::data_container_node
{
  Q_OBJECT
public:
  data_container_mock()
    : yoyo::data_container_node { "group_mock", {}, yoyo::types::value_t::CUSTOM }
  {
  }

  MOCK_METHOD(yoyo::types::variant_t, value, (), (const, override));
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
};

boost::uuids::uuid data_container_mock::_id = boost::uuids::random_generator {}();
boost::uuids::uuid const gui_type_1::_typeId = boost::uuids::random_generator {}();
boost::uuids::uuid const gui_type_2::_typeId = boost::uuids::random_generator {}();
boost::uuids::uuid const gui_type_3::_typeId = boost::uuids::random_generator {}();
boost::uuids::uuid const data_type_1::_typeId = boost::uuids::random_generator {}();
boost::uuids::uuid const data_type_2::_typeId = boost::uuids::random_generator {}();
boost::uuids::uuid const data_type_3::_typeId = boost::uuids::random_generator {}();

class ConnectivityTests : public yoyo::test::guiFixture
{
public:
  auto SetUp() -> void override
  {
    yoyo::test::guiFixture::SetUp();

    auto fundamentalFactory =
      std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject);
    yoyo::install_fundamental_nodes(*fundamentalFactory);

    _factories.push_back(fundamentalFactory);
  }

  auto setupConfiguration() -> std::shared_ptr<yoyo::node_base>
  {
    auto configuration =
      _factories[0]->createNode(std::get<0>(_factories[0]->installed_nodes()[0]), {});

    auto data_root =
      _factories[0]->createNode(std::get<0>(_factories[0]->installed_nodes()[1]), {});
    EXPECT_TRUE(configuration->acceptsChild(data_root));
    configuration->addChild(data_root);

    auto gui_root = _factories[0]->createNode(std::get<0>(_factories[0]->installed_nodes()[2]), {});
    EXPECT_TRUE(configuration->acceptsChild(gui_root));
    configuration->addChild(gui_root);

    return configuration;
  }

  template <typename T>
  auto createData(std::shared_ptr<yoyo::node_base> parent, QString const& name,
                  QString const& title, QString const& unit)
    -> std::shared_ptr<testing::StrictMock<T>>
  {
    auto some_data = std::make_shared<testing::StrictMock<T>>();
    parent->addChild(some_data);
    some_data->setName({ name, true });
    if constexpr (!std::is_same_v<T, data_container_mock>) {
      some_data->setTitle(title);
      some_data->setUnit({ unit, true });
    }
    return some_data;
  }

  template <typename T>
  auto createGui(std::shared_ptr<yoyo::node_base> parent, QString const& connection)
    -> std::shared_ptr<testing::StrictMock<T>>
  {
    auto some_gui = std::make_shared<testing::StrictMock<T>>();
    some_gui->init();
    parent->addChild(some_gui);
    auto c = some_gui->connection();
    if constexpr (std::is_same_v<decltype(c), yoyo::properties::connection_t>) {
      c._auto = connection;
    } else if constexpr (std::is_same_v<decltype(c), yoyo::properties::in_connection_t>) {
      c._in = connection;
    } else if constexpr (std::is_same_v<decltype(c), yoyo::properties::out_connection_t>) {
      c._out = connection;
    }
    some_gui->setConnection(c);
    return some_gui;
  }

  template <typename T, typename U>
  auto expectNodeConnected(std::shared_ptr<testing::StrictMock<T>> some_gui,
                           std::shared_ptr<testing::StrictMock<U>> some_data,
                           yoyo::types::value_t valueType) -> void
  {
    EXPECT_CALL(*some_gui, supportsValueType(valueType)).WillOnce(Return(true));
    EXPECT_CALL(*some_gui, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui,
                setTitle(yoyo::properties::connected_string_t { some_data->title(), "" }));
    if constexpr (std::is_same_v<T, gui_type_3> || std::is_same_v<T, gui_type_2>) {
      EXPECT_CALL(*some_gui, unit())
        .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
      EXPECT_CALL(*some_gui,
                  setUnit(yoyo::properties::connected_string_t { some_data->unit()._s, "" }));
    }
    if constexpr (std::is_same_v<T, gui_type_3>) {
      EXPECT_CALL(*some_gui, enumValues()).WillOnce(Return(yoyo::properties::enum_t { false, {} }));
      EXPECT_CALL(*some_gui, setEnumValues(some_data->enumValues()));
    }
  }

  template <typename T>
  auto expectNodeDisConnected(std::shared_ptr<testing::StrictMock<T>> some_gui) -> void
  {
    EXPECT_CALL(*some_gui, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui, setTitle(yoyo::properties::connected_string_t { "", "" }));
    if constexpr (std::is_same_v<T, gui_type_2> || std::is_same_v<T, gui_type_3>) {
      EXPECT_CALL(*some_gui, unit())
        .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
      EXPECT_CALL(*some_gui, setUnit(yoyo::properties::connected_string_t { "", "" }));
    }
  }

  std::vector<std::shared_ptr<yoyo::node_factory>> _factories;
};

///
/// \brief ConnectivityTests.connectEmptyConfiguration
///
/// This test verifies, that, on an empty configuration in particular, creation and deletion of
/// nodes is tracked and handled respectively.
/// In addition it verifies connections in case of changing data-paths.
///
TEST_F(ConnectivityTests, connectEmptyConfiguration)
{
  auto configuration = setupConfiguration();
  yoyo::connectivity_manager manager(configuration);

  auto some_data = std::make_shared<testing::StrictMock<data_type_1>>();
  configuration->childAt(0)->addChild(some_data);
  some_data->setName({ "myData", true });
  some_data->setTitle("My Title");

  auto some_gui = std::make_shared<testing::StrictMock<gui_type_1>>();
  some_gui->init();
  configuration->childAt(1)->addChild(some_gui);

  EXPECT_CALL(*some_gui, setter(_)).Times(0);
  some_data->emitValue({ true });

  // Connection changes
  {
    auto c = some_gui->connection();
    c._auto = "myData";

    EXPECT_CALL(*some_gui, supportsValueType(yoyo::types::value_t::BIT)).WillOnce(Return(true));
    EXPECT_CALL(*some_gui, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui, setTitle(yoyo::properties::connected_string_t { "My Title", "" }));
    some_gui->setConnection(c);

    EXPECT_CALL(*some_gui, setter(QVariant(42)));
    some_data->emitValue(42);
    EXPECT_CALL(*some_gui, setter(QVariant(true)));
    some_data->emitValue(true);

    EXPECT_CALL(*some_data, setValue(QVariant(5)));
    some_gui->set(5);
    EXPECT_CALL(*some_data, setValue(QVariant("bar")));
    some_gui->set("bar");
  }
  {
    auto c = some_gui->connection();
    c._auto = "myData1";

    EXPECT_CALL(*some_gui, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui, setTitle(yoyo::properties::connected_string_t { "", "" }));
    some_gui->setConnection(c);

    some_data->emitValue("foo");
    some_data->emitValue(false);
    some_gui->set(5);
    some_gui->set("bar");
  }
  // Name changes
  {
    EXPECT_CALL(*some_gui, supportsValueType(yoyo::types::value_t::BIT)).WillOnce(Return(true));
    EXPECT_CALL(*some_gui, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui, setTitle(yoyo::properties::connected_string_t { "My Title", "" }));
    some_data->setName({ "myData1", true });

    EXPECT_CALL(*some_gui, setter(QVariant(42)));
    some_data->emitValue(42);
    EXPECT_CALL(*some_gui, setter(QVariant(true)));
    some_data->emitValue(true);

    EXPECT_CALL(*some_data, setValue(QVariant(5)));
    some_gui->set(5);
    EXPECT_CALL(*some_data, setValue(QVariant("bar")));
    some_gui->set("bar");
  }
  {
    EXPECT_CALL(*some_gui, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui, setTitle(yoyo::properties::connected_string_t { "", "" }));
    some_data->setName({ "myData", true });

    some_data->emitValue("foo");
    some_data->emitValue(false);
    some_gui->set(5);
    some_gui->set("bar");
  }

  // Add a second set of items
  auto some_data1 = std::make_shared<testing::StrictMock<data_type_2>>();
  configuration->childAt(0)->addChild(some_data1);
  some_data1->setName({ "myOtherData", true });
  some_data1->setTitle("My other Title");
  some_data1->setUnit({ "A Unit", true });

  auto some_gui1 = std::make_shared<testing::StrictMock<gui_type_2>>();
  some_gui1->init();
  configuration->childAt(1)->addChild(some_gui1);

  {
    auto c = some_gui1->connection();
    c._in = "myOtherData";

    EXPECT_CALL(*some_gui1, supportsValueType(yoyo::types::value_t::UINT8)).WillOnce(Return(true));
    EXPECT_CALL(*some_gui1, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui1,
                setTitle(yoyo::properties::connected_string_t { "My other Title", "" }));
    EXPECT_CALL(*some_gui1, unit())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui1, setUnit(yoyo::properties::connected_string_t { "A Unit", "" }));
    some_gui1->setConnection(c);

    EXPECT_CALL(*some_gui1, setter(QVariant(42)));
    some_data1->emitValue(42);
    EXPECT_CALL(*some_gui1, setter(QVariant(true)));
    some_data1->emitValue(true);
    some_data->emitValue(0);
    some_data->emitValue("bar");

    EXPECT_CALL(*some_gui1, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui1, setTitle(yoyo::properties::connected_string_t { "changed title", "" }));
    some_data1->setTitle("changed title");
    EXPECT_CALL(*some_gui1, unit())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui1, setUnit(yoyo::properties::connected_string_t { "changed Unit", "" }));
    some_data1->setUnit({ "changed Unit", true });
  }
  {
    auto c = some_gui1->connection();
    c._in = "myData1";

    EXPECT_CALL(*some_gui1, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui1, setTitle(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui1, unit())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui1, setUnit(yoyo::properties::connected_string_t { "", "" }));
    some_gui1->setConnection(c);

    some_data1->emitValue("foo");
    some_data1->emitValue(false);
    some_data->emitValue(0);
    some_data->emitValue("bar");
    some_data1->setTitle("My other Title");
    some_data1->setUnit({ "A Unit", true });
  }
  // And a second set of items
  auto some_data2 = std::make_shared<testing::StrictMock<data_type_2>>();
  configuration->childAt(0)->addChild(some_data2);
  some_data2->setName({ "myOtherData2", true });
  some_data2->setTitle("My other Title");
  some_data2->setUnit({ "A Unit", true });

  auto some_gui2 = std::make_shared<testing::StrictMock<gui_type_2>>();
  some_gui2->init();
  configuration->childAt(1)->addChild(some_gui2);

  {
    auto c = some_gui2->connection();
    c._in = "myOtherData2";

    EXPECT_CALL(*some_gui2, supportsValueType(yoyo::types::value_t::UINT8)).WillOnce(Return(true));
    EXPECT_CALL(*some_gui2, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui2,
                setTitle(yoyo::properties::connected_string_t { "My other Title", "" }));
    EXPECT_CALL(*some_gui2, unit())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui2, setUnit(yoyo::properties::connected_string_t { "A Unit", "" }));
    some_gui2->setConnection(c);

    EXPECT_CALL(*some_gui2, setter(QVariant(42)));
    some_data2->emitValue(42);
    EXPECT_CALL(*some_gui2, setter(QVariant(true)));
    some_data2->emitValue(true);
    some_data->emitValue(0);
    some_data->emitValue("bar");

    EXPECT_CALL(*some_gui2, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui2,
                setTitle(yoyo::properties::connected_string_t { "other changed title", "" }));
    some_data2->setTitle("other changed title");
    EXPECT_CALL(*some_gui2, unit())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui2,
                setUnit(yoyo::properties::connected_string_t { "other changed Unit", "" }));
    some_data2->setUnit({ "other changed Unit", true });
  }
  {
    auto c = some_gui2->connection();
    c._in = "myData1";

    EXPECT_CALL(*some_gui2, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui2, setTitle(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui2, unit())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui2, setUnit(yoyo::properties::connected_string_t { "", "" }));
    some_gui2->setConnection(c);

    some_data2->emitValue("foo");
    some_data2->emitValue(false);
    some_data1->emitValue(0);
    some_data1->emitValue("bar");
    some_data->emitValue(0);
    some_data->emitValue("bar");
    some_data2->setTitle("My other Title");
    some_data2->setUnit({ "A Unit", true });
  }
  // Connect all gui to one data
  {
    EXPECT_CALL(*some_gui, supportsValueType(yoyo::types::value_t::UINT8)).WillOnce(Return(true));
    EXPECT_CALL(*some_gui, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui, setTitle(yoyo::properties::connected_string_t { "My other Title", "" }));

    EXPECT_CALL(*some_gui1, supportsValueType(yoyo::types::value_t::UINT8)).WillOnce(Return(true));
    EXPECT_CALL(*some_gui1, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui1,
                setTitle(yoyo::properties::connected_string_t { "My other Title", "" }));
    EXPECT_CALL(*some_gui1, unit())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui1, setUnit(yoyo::properties::connected_string_t { "A Unit", "" }));

    EXPECT_CALL(*some_gui2, supportsValueType(yoyo::types::value_t::UINT8)).WillOnce(Return(true));
    EXPECT_CALL(*some_gui2, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui2,
                setTitle(yoyo::properties::connected_string_t { "My other Title", "" }));
    EXPECT_CALL(*some_gui2, unit())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui2, setUnit(yoyo::properties::connected_string_t { "A Unit", "" }));

    some_data1->setName({ "myData1", true });

    EXPECT_CALL(*some_gui, setter(QVariant(42)));
    EXPECT_CALL(*some_gui1, setter(QVariant(42)));
    EXPECT_CALL(*some_gui2, setter(QVariant(42)));
    some_data1->emitValue(42);
  }
  {
    EXPECT_CALL(*some_gui, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui, setTitle(yoyo::properties::connected_string_t { "", "" }));

    EXPECT_CALL(*some_gui1, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui1, setTitle(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui1, unit())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui1, setUnit(yoyo::properties::connected_string_t { "", "" }));

    EXPECT_CALL(*some_gui2, title())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui2, setTitle(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui2, unit())
      .WillOnce(Return(yoyo::properties::connected_string_t { "", "" }));
    EXPECT_CALL(*some_gui2, setUnit(yoyo::properties::connected_string_t { "", "" }));
  }
}

///
/// \brief ConnectivityTests.connectLoadedConfiguration
///
/// This test verifies, that the connection manager connects items correctly, if the hierarchy is
/// already finished at creation.
///
TEST_F(ConnectivityTests, connectLoadedConfiguration)
{
  auto configuration = setupConfiguration();

  auto some_data1 =
    createData<data_type_1>(configuration->childAt(0), "myData1", "My Title 1", "Super Unit");
  auto some_data2 =
    createData<data_type_3>(configuration->childAt(0), "myData2", "My Title 2", "Mega Unit");
  auto some_data3 =
    createData<data_type_2>(configuration->childAt(0), "myData3", "My Title 3", "Giga Unit");

  auto some_gui1 = createGui<gui_type_1>(configuration->childAt(1), "myData1");
  auto some_gui2 = createGui<gui_type_2>(configuration->childAt(1), "myData2");
  auto some_gui3 = createGui<gui_type_3>(configuration->childAt(1), "myData3");

  auto data_enumValues = yoyo::properties::enum_t { true,
                                                    {
                                                      { 0, { "v1", "v1", {} } },
                                                      { 1, { "v2", "v2", {} } },
                                                      { 2, { "v3", "v3", {} } },
                                                    } };
  ON_CALL(*some_data3, enumValues()).WillByDefault(Return(data_enumValues));

  EXPECT_CALL(*some_data3, enumValues()).Times(2);
  expectNodeConnected(some_gui1, some_data1, yoyo::types::value_t::BIT);
  expectNodeConnected(some_gui2, some_data2, yoyo::types::value_t::STRING);
  expectNodeConnected(some_gui3, some_data3, yoyo::types::value_t::UINT8);

  yoyo::connectivity_manager manager(configuration);

  // auto connect
  EXPECT_CALL(*some_gui1, setter(QVariant(42)));
  some_data1->emitValue(42);
  EXPECT_CALL(*some_data1, setValue(QVariant(-5)));
  some_gui1->set(-5);

  // only supports in connection
  EXPECT_CALL(*some_gui2, setter(QVariant("foo")));
  some_data2->emitValue("foo");

  // only supports out connection
  EXPECT_CALL(*some_gui3, setter(QVariant(-10))).Times(0);
  some_data3->emitValue(-10);
  EXPECT_CALL(*some_data1, setValue(QVariant(-5)));
  some_gui1->set(-5);

  expectNodeDisConnected(some_gui1);
  expectNodeDisConnected(some_gui2);
  expectNodeDisConnected(some_gui3);
}

///
/// \brief ConnectivityTests.connectWithHierarchy
///
/// This test verifies, that the connection manager connects items correctly, in case the hierarchy
/// has more depth than 1.
///
TEST_F(ConnectivityTests, connectWithHierarchy)
{
  auto configuration = setupConfiguration();

  auto group1 =
    createData<data_container_mock>(configuration->childAt(0), "myGroup1", "My Group Title", "");
  auto group2 = createData<data_container_mock>(group1, "myGroup2", "My Group Title", "");
  auto group3 = createData<data_container_mock>(group2, "myGroup3", "My Group Title", "");
  auto some_data1 = createData<data_type_1>(group3, "myData1", "My Title 1", "Super Unit");

  auto some_gui1 =
    createGui<gui_type_1>(configuration->childAt(1), "myGroup1.myGroup2.myGroup3.myData1");

  expectNodeConnected(some_gui1, some_data1, yoyo::types::value_t::BIT);

  yoyo::connectivity_manager manager(configuration);

  // auto connect
  EXPECT_CALL(*some_gui1, setter(QVariant(42)));
  some_data1->emitValue(42);
  EXPECT_CALL(*some_data1, setValue(QVariant(-5)));
  some_gui1->set(-5);

  expectNodeDisConnected(some_gui1);
}

///
/// \brief ConnectivityTests.connectedPropertiesConfiguration
///
/// This test verifies, that for all tracked property types, changes in the property on the data-
/// side will change the connected property on the gui-side.
///
/// \todo still missing: enabled property
///
TEST_F(ConnectivityTests, connectedPropertiesConfiguration)
{
  using c_string = yoyo::properties::connected_string_t;
  auto configuration = setupConfiguration();

  auto some_data1 =
    createData<data_type_1>(configuration->childAt(0), "myData1", "My Title 1", "Super Unit");
  auto some_data2 =
    createData<data_type_3>(configuration->childAt(0), "myData2", "My Title 2", "Mega Unit");
  auto some_data3 =
    createData<data_type_2>(configuration->childAt(0), "myData3", "My Title 3", "Giga Unit");

  auto some_gui1 = createGui<gui_type_1>(configuration->childAt(1), "myData1");
  auto some_gui2 = createGui<gui_type_2>(configuration->childAt(1), "myData2");
  auto some_gui3 = createGui<gui_type_3>(configuration->childAt(1), "myData3");

  auto data_enumValues = yoyo::properties::enum_t { true,
                                                    {
                                                      { 0, { "v1", "v1", {} } },
                                                      { 1, { "v2", "v2", {} } },
                                                      { 2, { "v3", "v3", {} } },
                                                    } };
  ON_CALL(*some_data3, enumValues()).WillByDefault(Return(data_enumValues));

  EXPECT_CALL(*some_data3, enumValues()).Times(2);
  expectNodeConnected(some_gui1, some_data1, yoyo::types::value_t::BIT);
  expectNodeConnected(some_gui2, some_data2, yoyo::types::value_t::STRING);
  expectNodeConnected(some_gui3, some_data3, yoyo::types::value_t::UINT8);

  yoyo::connectivity_manager manager(configuration);

  EXPECT_CALL(*some_gui1, title()).WillOnce(Return(c_string { "My Title 1", "" }));
  EXPECT_CALL(*some_gui1, setTitle(c_string { "My changed Title1", "" }));
  some_data1->setTitle("My changed Title1");
  some_data1->setUnit({ "My changed Unit1", true });

  EXPECT_CALL(*some_gui2, title()).WillOnce(Return(c_string { "My Title 2", "" }));
  EXPECT_CALL(*some_gui2, setTitle(c_string { "My changed Title2", "" }));
  some_data2->setTitle("My changed Title2");
  EXPECT_CALL(*some_gui2, unit()).WillOnce(Return(c_string { "Mega Unit", "" }));
  EXPECT_CALL(*some_gui2, setUnit(c_string { "My changed Unit2", "" }));
  some_data2->setUnit({ "My changed Unit2", true });

  EXPECT_CALL(*some_gui3, title()).WillOnce(Return(c_string { "My Title 3", "" }));
  EXPECT_CALL(*some_gui3, setTitle(c_string { "My changed Title3", "" }));
  some_data3->setTitle("My changed Title3");
  EXPECT_CALL(*some_gui3, unit()).WillOnce(Return(c_string { "Giga Unit", "" }));
  EXPECT_CALL(*some_gui3, setUnit(c_string { "My changed Unit3", "" }));
  some_data3->setUnit({ "My changed Unit3", true });

  auto data_enumValues2 = yoyo::properties::enum_t { true,
                                                     {
                                                       { 0, { "v1", "v1", {} } },
                                                       { 2, { "v3", "v3", {} } },
                                                     } };
  ON_CALL(*some_data3, enumValues()).WillByDefault(Return(data_enumValues2));
  EXPECT_CALL(*some_gui3, setEnumValues(data_enumValues2));
  some_data3->enumValuesChanged(data_enumValues2);

  expectNodeDisConnected(some_gui1);
  expectNodeDisConnected(some_gui2);
  expectNodeDisConnected(some_gui3);
}

#include "connectivityManagerTests.moc"
