#include "io_fixture.hpp"

#include <QMetaProperty>

#include <boost/uuid/random_generator.hpp>

#include <random>

namespace
{
template <typename T>
auto generateRandomValue(T min, T max) -> std::enable_if_t<std::is_floating_point_v<T>, T>
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<T> distrib(min, max);
  return distrib(gen);
}

template <typename T>
auto generateRandomValue(T min, T max) -> std::enable_if_t<std::is_integral_v<T>, T>
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<T> distrib(min, max);
  return distrib(gen);
}

template <typename T>
auto generateRandomValue(T min, T max) -> std::enable_if_t<std::is_enum_v<T>, T>
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib(static_cast<int>(min), static_cast<int>(max));
  return static_cast<T>(distrib(gen));
}

template <typename T>
auto generateRandomValue(T, T) -> std::enable_if_t<std::is_same_v<T, QString>, T>
{
  static std::vector<QString> const values { "foo", "bar",          "bla",
                                             "fuu", "hello world!", "lorem ipsum" };
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> distrib(0, values.size() - 1);

  return values[distrib(gen)];
}
} // namespace

class node_dummy : public yoyo::node_base
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
  static boost::uuids::uuid _id;
};
class group_dummy : public yoyo::node_base
{
  Q_OBJECT
  Q_PROPERTY(bool prop MEMBER _prop)
public:
  using yoyo::node_base::node_base;

  auto acceptsChildren() const -> bool override
  {
    return true;
  }
  auto acceptsChild(std::shared_ptr<node_base> const& /*child*/) const -> bool override
  {
    return true;
  }
  auto acceptsParent(std::shared_ptr<node_base> const& /*parent*/) const -> bool override
  {
    return true;
  }
  auto childAboutToBeAdded(std::shared_ptr<node_base> const& /*child*/) -> bool override
  {
    return true;
  }
  auto childAdded(std::shared_ptr<node_base> const& /*child*/) -> bool override
  {
    return true;
  }
  auto childAboutToBeRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool override
  {
    return true;
  }
  auto childRemoved(std::shared_ptr<node_base> const& /*child*/) -> bool override
  {
    return true;
  }
  auto childMoved(std::shared_ptr<node_base> const& /*child*/, size_t /*from_index*/,
                  size_t /*to_index*/) -> void override
  {
  }
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
  bool _prop { true };
};
class node_dumm1 : public node_dummy
{
  Q_OBJECT
  Q_PROPERTY(char prop1 MEMBER _prop1)
  Q_PROPERTY(int prop2 MEMBER _prop2)
  Q_PROPERTY(long prop3 MEMBER _prop3)
  Q_PROPERTY(int8_t prop4 MEMBER _prop4)
  Q_PROPERTY(int16_t prop5 MEMBER _prop5)
  Q_PROPERTY(int32_t prop6 MEMBER _prop6)
public:
  using node_dummy::node_dummy;
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
  char _prop1 { 'a' };
  int _prop2 { 42 };
  long _prop3 { 123456789 };
  int8_t _prop4 { -126 };
  int16_t _prop5 { -12345 };
  int32_t _prop6 { 43346434 };
};
class node_dumm2 : public node_dummy
{
  Q_OBJECT
  Q_PROPERTY(int64_t prop1 MEMBER _prop1)
  Q_PROPERTY(uint8_t prop2 MEMBER _prop2)
  Q_PROPERTY(uint16_t prop3 MEMBER _prop3)
public:
  using node_dummy::node_dummy;
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
  int64_t _prop1 { -15 };
  uint8_t _prop2 { 0xfe };
  uint16_t _prop3 { 0xabcd };
};
class node_dumm3 : public node_dummy
{
  Q_OBJECT
  Q_PROPERTY(uint32_t prop1 MEMBER _prop1)
  Q_PROPERTY(uint64_t prop2 MEMBER _prop2)
  Q_PROPERTY(float prop3 MEMBER _prop3)
  Q_PROPERTY(double prop4 MEMBER _prop4)
  Q_PROPERTY(yoyo::types::value_t prop5 MEMBER _prop5)
  Q_PROPERTY(yoyo::types::access_t prop6 MEMBER _prop6)
  Q_PROPERTY(yoyo::types::layout_direction_t prop7 MEMBER _prop7)
  Q_PROPERTY(QString prop8 MEMBER _prop8)
  Q_PROPERTY(yoyo::properties::invisible_string_t prop9 MEMBER _prop9)
public:
  using node_dummy::node_dummy;
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
  uint32_t _prop1 { 0xabcd0123 };
  uint64_t _prop2 { 0xabcd0123456789ab };
  float _prop3 { -26.5f };
  double _prop4 { 1234.25 };
  yoyo::types::value_t _prop5 { yoyo::types::value_t::CUSTOM };
  yoyo::types::access_t _prop6 { yoyo::types::access_t::READ_WRITE };
  yoyo::types::layout_direction_t _prop7 { yoyo::types::layout_direction_t::HORIZONTAL };
  QString _prop8 { "foo" };
  yoyo::properties::invisible_string_t _prop9 { "bar", true };
};
class node_dumm4 : public node_dummy
{
  Q_OBJECT
  Q_PROPERTY(yoyo::properties::connected_string_t prop1 MEMBER _prop1)
  Q_PROPERTY(yoyo::properties::connected_boolean_t prop2 MEMBER _prop2)
  Q_PROPERTY(yoyo::properties::connection_t prop3 MEMBER _prop3)
  Q_PROPERTY(yoyo::properties::invisible_layout_direction_t prop4 MEMBER _prop4)
  Q_PROPERTY(yoyo::properties::patterned_string_t prop5 MEMBER _prop5)
  Q_PROPERTY(yoyo::properties::limited_uint8_t prop6 MEMBER _prop6)
public:
  using node_dummy::node_dummy;
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
  yoyo::properties::connected_string_t _prop1 { "", "foo" };
  yoyo::properties::connected_boolean_t _prop2 {
    yoyo::properties::connected_boolean_t::boolean_t::DEFINED_BY_CONNECTION, false
  };
  yoyo::properties::connection_t _prop3 { {}, {}, {}, "foo", "bar", "fuu" };
  yoyo::properties::invisible_layout_direction_t _prop4 { yoyo::types::layout_direction_t::FREE,
                                                          true };
  yoyo::properties::patterned_string_t _prop5 { "foo", {} };
  yoyo::properties::limited_uint8_t _prop6 { 5, 0, 10 };
};
class node_dumm5 : public node_dummy
{
  Q_OBJECT
  Q_PROPERTY(yoyo::properties::limited_uint16_t prop1 MEMBER _prop1)
  Q_PROPERTY(yoyo::properties::limited_uint32_t prop2 MEMBER _prop2)
public:
  using node_dummy::node_dummy;
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
  yoyo::properties::limited_uint16_t _prop1 { 7, 0, 10 };
  yoyo::properties::limited_uint32_t _prop2 { 6, 1, 20 };
};
class node_dumm6 : public node_dummy
{
  Q_OBJECT
  Q_PROPERTY(yoyo::properties::limited_uint64_t prop1 MEMBER _prop1)
  Q_PROPERTY(yoyo::properties::limited_int8_t prop2 MEMBER _prop2)
  Q_PROPERTY(yoyo::properties::limited_int16_t prop3 MEMBER _prop3)
  Q_PROPERTY(yoyo::properties::limited_int32_t prop4 MEMBER _prop4)
  Q_PROPERTY(yoyo::properties::limited_int64_t prop5 MEMBER _prop5)
  Q_PROPERTY(yoyo::properties::text_t prop6 MEMBER _prop6)
  Q_PROPERTY(int prop7 MEMBER _prop7)
  Q_PROPERTY(int prop8 MEMBER _prop8)
  Q_PROPERTY(int prop9 MEMBER _prop9)
  Q_PROPERTY(int prop10 MEMBER _prop10)
  Q_PROPERTY(int prop11 MEMBER _prop11)
  Q_PROPERTY(int prop12 MEMBER _prop12)
  Q_PROPERTY(int prop13 MEMBER _prop13)
  Q_PROPERTY(int prop14 MEMBER _prop14)
public:
  using node_dummy::node_dummy;
  auto staticTypeId() const -> boost::uuids::uuid override
  {
    return _id;
  }
  static boost::uuids::uuid _id;
  yoyo::properties::limited_uint64_t _prop1 { 5, 0, 10 };
  yoyo::properties::limited_int8_t _prop2 { 5, -10, 10 };
  yoyo::properties::limited_int16_t _prop3 { 5, -10, 10 };
  yoyo::properties::limited_int32_t _prop4 { 5, -10, 10 };
  yoyo::properties::limited_int64_t _prop5 { 5, -10, 10 };
  yoyo::properties::text_t _prop6;
  int _prop7 { 0 };
  int _prop8 { 0 };
  int _prop9 { 0 };
  int _prop10 { 0 };
  int _prop11 { 0 };
  int _prop12 { 0 };
  int _prop13 { 0 };
  int _prop14 { 0 };
};
boost::uuids::uuid node_dummy::_id = boost::uuids::random_generator {}();
boost::uuids::uuid group_dummy::_id = boost::uuids::random_generator {}();
boost::uuids::uuid node_dumm1::_id = boost::uuids::random_generator {}();
boost::uuids::uuid node_dumm2::_id = boost::uuids::random_generator {}();
boost::uuids::uuid node_dumm3::_id = boost::uuids::random_generator {}();
boost::uuids::uuid node_dumm4::_id = boost::uuids::random_generator {}();
boost::uuids::uuid node_dumm5::_id = boost::uuids::random_generator {}();
boost::uuids::uuid node_dumm6::_id = boost::uuids::random_generator {}();

auto IoFixture::SetUp() -> void
{
  installNodes();
  yoyo::registerAllPropertyTypes();
}

auto IoFixture::TearDown() -> void {}

auto IoFixture::installNodes() -> void
{
  _factories.push_back(std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject));
  _factories.push_back(std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject));
  _factories.push_back(std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject));
  _factories.push_back(std::make_shared<yoyo::node_factory>(&yoyo::node_base::staticMetaObject));

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 3);

  _factories[0]->install_node(
    group_dummy::_id, "dummy group", &group_dummy::staticMetaObject,
    yoyo::documentation::builder { "dummy group" }
      .description("bla")
      .property("name", "", "", "", QVariant { QString {} })
      .property("prop", "", "", "", QVariant { true })
      .build(),
    [](boost::uuids::uuid id) { return std::make_shared<group_dummy>("dummy group", id); },
    [](yoyo::node_factory::node_id_list l) { return l; });
  _ids.push_back(std::make_pair(group_dummy::_id, 0));
  auto rnd = dist(rng);
  ASSERT_LE(rnd, 3);
  _factories[rnd]->install_node(
    node_dummy::_id, "dummy node", &node_dummy::staticMetaObject,
    yoyo::documentation::builder { "dummy node" }
      .description("bla")
      .property("name", "", "", "", QVariant { QString {} })
      .build(),
    [](boost::uuids::uuid id) { return std::make_shared<node_dummy>("dummy node", id); },
    [](yoyo::node_factory::node_id_list) { return yoyo::node_factory::node_id_list {}; });
  _ids.push_back(std::make_pair(node_dummy::_id, rnd));
  rnd = dist(rng);
  ASSERT_LE(rnd, 3);
  _factories[rnd]->install_node(
    node_dumm1::_id, "dummy node 1", &node_dumm1::staticMetaObject,
    yoyo::documentation::builder { "dummy node 1" }
      .description("bla")
      .property("name", "", "", "", QVariant { QString {} })
      .property("prop1", "", "", "", QVariant::fromValue(char('a')))
      .property("prop2", "", "", "", QVariant::fromValue(int(42)))
      .property("prop3", "", "", "", QVariant::fromValue(long(123456789)))
      .property("prop4", "", "", "", QVariant::fromValue(int8_t(-126)))
      .property("prop5", "", "", "", QVariant::fromValue(int16_t(-12345)))
      .property("prop6", "", "", "", QVariant::fromValue(int32_t(43346434)))
      .build(),
    [](boost::uuids::uuid id) { return std::make_shared<node_dumm1>("dummy node 1", id); },
    [](yoyo::node_factory::node_id_list) { return yoyo::node_factory::node_id_list {}; });
  _ids.push_back(std::make_pair(node_dumm1::_id, rnd));
  rnd = dist(rng);
  ASSERT_LE(rnd, 3);
  _factories[rnd]->install_node(
    node_dumm2::_id, "dummy node 2", &node_dumm2::staticMetaObject,
    yoyo::documentation::builder { "dummy node 2" }
      .description("bla")
      .property("name", "", "", "", QVariant { QString {} })
      .property("prop1", "", "", "", QVariant::fromValue(int64_t(-15)))
      .property("prop2", "", "", "", QVariant::fromValue(uint8_t(0xfe)))
      .property("prop3", "", "", "", QVariant::fromValue(uint16_t(0xabcd)))
      .build(),
    [](boost::uuids::uuid id) { return std::make_shared<node_dumm2>("dummy node 2", id); },
    [](yoyo::node_factory::node_id_list) { return yoyo::node_factory::node_id_list {}; });
  _ids.push_back(std::make_pair(node_dumm2::_id, rnd));
  rnd = dist(rng);
  ASSERT_LE(rnd, 3);
  _factories[rnd]->install_node(
    node_dumm3::_id, "dummy node 3", &node_dumm3::staticMetaObject,
    yoyo::documentation::builder { "dummy node 3" }
      .description("bla")
      .property("name", "", "", "", QVariant { QString {} })
      .property("prop1", "", "", "", QVariant::fromValue(uint32_t(0xabcd0123)))
      .property("prop2", "", "", "", QVariant::fromValue(uint64_t(0xabcd0123456789ab)))
      .property("prop3", "", "", "", QVariant::fromValue(float(-26.5f)))
      .property("prop4", "", "", "", QVariant::fromValue(double(1234.25)))
      .property("prop5", "", "", "", QVariant::fromValue(yoyo::types::value_t::CUSTOM))
      .property("prop6", "", "", "", QVariant::fromValue(yoyo::types::access_t::READ_WRITE))
      .property("prop7", "", "", "",
                QVariant::fromValue(yoyo::types::layout_direction_t::HORIZONTAL))
      .property("prop8", "", "", "", QVariant::fromValue(QString("foo")))
      .property("prop9", "", "", "",
                QVariant::fromValue(yoyo::properties::invisible_string_t { "bar", true }))
      .build(),
    [](boost::uuids::uuid id) { return std::make_shared<node_dumm3>("dummy node 3", id); },
    [](yoyo::node_factory::node_id_list) { return yoyo::node_factory::node_id_list {}; });
  _ids.push_back(std::make_pair(node_dumm3::_id, rnd));
  rnd = dist(rng);
  ASSERT_LE(rnd, 3);
  _factories[rnd]->install_node(
    node_dumm4::_id, "dummy node 4", &node_dumm4::staticMetaObject,
    yoyo::documentation::builder { "dummy node 4" }
      .description("bla")
      .property("name", "", "", "", QVariant { QString {} })
      .property("prop1", "", "", "",
                QVariant::fromValue(yoyo::properties::connected_string_t { "", "foo" }))
      .property("prop2", "", "", "",
                QVariant::fromValue(yoyo::properties::connected_boolean_t {
                  yoyo::properties::connected_boolean_t::boolean_t::DEFINED_BY_CONNECTION, false }))
      .property(
        "prop3", "", "", "",
        QVariant::fromValue(yoyo::properties::connection_t { {}, {}, {}, "foo", "bar", "fuu" }))
      .property("prop4", "", "", "",
                QVariant::fromValue(yoyo::properties::invisible_layout_direction_t {
                  yoyo::types::layout_direction_t::FREE, true }))
      .property("prop5", "", "", "",
                QVariant::fromValue(yoyo::properties::patterned_string_t { "foo", {} }))
      .property("prop6", "", "", "",
                QVariant::fromValue(yoyo::properties::limited_uint8_t { 5, 0, 10 }))
      .build(),
    [](boost::uuids::uuid id) { return std::make_shared<node_dumm4>("dummy node 4", id); },
    [](yoyo::node_factory::node_id_list) { return yoyo::node_factory::node_id_list {}; });
  _ids.push_back(std::make_pair(node_dumm4::_id, rnd));
  rnd = dist(rng);
  ASSERT_LE(rnd, 3);
  _factories[rnd]->install_node(
    node_dumm5::_id, "dummy node 5", &node_dumm5::staticMetaObject,
    yoyo::documentation::builder { "dummy node 5" }
      .description("bla")
      .property("name", "", "", "", QVariant { QString {} })
      .property("prop1", "", "", "",
                QVariant::fromValue(yoyo::properties::limited_uint16_t { 7, 0, 10 }))
      .property("prop2", "", "", "",
                QVariant::fromValue(yoyo::properties::limited_uint32_t { 6, 1, 20 }))
      .build(),
    [](boost::uuids::uuid id) { return std::make_shared<node_dumm5>("dummy node 5", id); },
    [](yoyo::node_factory::node_id_list) { return yoyo::node_factory::node_id_list {}; });
  _ids.push_back(std::make_pair(node_dumm5::_id, rnd));
  rnd = dist(rng);
  ASSERT_LE(rnd, 3);
  _factories[rnd]->install_node(
    node_dumm6::_id, "dummy node 6", &node_dumm5::staticMetaObject,
    yoyo::documentation::builder { "dummy node 6" }
      .description("bla")
      .property("name", "", "", "", QVariant { QString {} })
      .property("prop1", "", "", "",
                QVariant::fromValue(yoyo::properties::limited_uint64_t { 5, 0, 10 }))
      .property("prop2", "", "", "",
                QVariant::fromValue(yoyo::properties::limited_int8_t { 5, -10, 10 }))
      .property("prop3", "", "", "",
                QVariant::fromValue(yoyo::properties::limited_int16_t { 5, -10, 10 }))
      .property("prop4", "", "", "",
                QVariant::fromValue(yoyo::properties::limited_int32_t { 5, -10, 10 }))
      .property("prop5", "", "", "",
                QVariant::fromValue(yoyo::properties::limited_int64_t { 5, -10, 10 }))
      .property("prop6", "", "", "", QVariant::fromValue(yoyo::properties::text_t {}))
      .property("prop7", "", "", "", QVariant::fromValue(int(0)))
      .property("prop8", "", "", "", QVariant::fromValue(int(0)))
      .property("prop9", "", "", "", QVariant::fromValue(int(0)))
      .property("prop10", "", "", "", QVariant::fromValue(int(0)))
      .property("prop11", "", "", "", QVariant::fromValue(int(0)))
      .property("prop12", "", "", "", QVariant::fromValue(int(0)))
      .property("prop13", "", "", "", QVariant::fromValue(int(0)))
      .property("prop14", "", "", "", QVariant::fromValue(int(0)))
      .build(),
    [](boost::uuids::uuid id) { return std::make_shared<node_dumm6>("dummy node 6", id); },
    [](yoyo::node_factory::node_id_list) { return yoyo::node_factory::node_id_list {}; });
  _ids.push_back(std::make_pair(node_dumm6::_id, rnd));
}

auto IoFixture::expectValuesEqual(QVariant const& v1, QVariant const& v2) -> void
{
  ASSERT_EQ(std::string { v1.typeName() }, v2.typeName());
  static std::map<std::string, std::function<void(QVariant const&, QVariant const&)>> const
    expecter = {
      { { "yoyo::properties::patterned_string_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::patterned_string_t>(),
                    v2.value<yoyo::properties::patterned_string_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::invisible_string_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::invisible_string_t>(),
                    v2.value<yoyo::properties::invisible_string_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::text_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::text_t>(), v2.value<yoyo::properties::text_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::connected_string_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::connected_string_t>(),
                    v2.value<yoyo::properties::connected_string_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::connected_boolean_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::connected_boolean_t>(),
                    v2.value<yoyo::properties::connected_boolean_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::connection_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::connection_t>(),
                    v2.value<yoyo::properties::connection_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::invisible_layout_direction_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::invisible_layout_direction_t>(),
                    v2.value<yoyo::properties::invisible_layout_direction_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::limited_uint8_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_uint8_t>(),
                    v2.value<yoyo::properties::limited_uint8_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::limited_uint16_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_uint16_t>(),
                    v2.value<yoyo::properties::limited_uint16_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::limited_uint32_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_uint32_t>(),
                    v2.value<yoyo::properties::limited_uint32_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::limited_uint64_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_uint64_t>(),
                    v2.value<yoyo::properties::limited_uint64_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::limited_int8_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_int8_t>(),
                    v2.value<yoyo::properties::limited_int8_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::limited_int16_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_int16_t>(),
                    v2.value<yoyo::properties::limited_int16_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::limited_int32_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_int32_t>(),
                    v2.value<yoyo::properties::limited_int32_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::limited_int64_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::limited_int64_t>(),
                    v2.value<yoyo::properties::limited_int64_t>())
            << v1.typeName();
        } },
      { { "yoyo::properties::script_t" },
        [](QVariant const& v1, QVariant const& v2) {
          EXPECT_EQ(v1.value<yoyo::properties::script_t>(), v2.value<yoyo::properties::script_t>())
            << v1.typeName();
        } },
    };

  auto it = expecter.find(v1.typeName());

  if (it == expecter.end()) {
    EXPECT_EQ(v1, v2) << v1.typeName();
  } else {
    it->second(v1, v2);
  }
}

auto IoFixture::compareNodes(std::shared_ptr<yoyo::node_base> actual,
                             std::shared_ptr<yoyo::node_base> expected) -> void
{
  ASSERT_EQ(actual->staticTypeId(), expected->staticTypeId());
  ASSERT_EQ(actual->metaObject(), expected->metaObject());
  EXPECT_EQ(actual->identifier(), expected->identifier());
  auto meta = expected->metaObject();

  for (auto i = yoyo::node_base::staticMetaObject.propertyOffset(); i < meta->propertyCount();
       ++i) {
    auto prop = meta->property(i);

    EXPECT_EQ(actual->metaObject()->property(i).name(), prop.name());
    EXPECT_EQ(actual->metaObject()->property(i).type(), prop.type());
    EXPECT_EQ(actual->metaObject()->property(i).typeName(), prop.typeName());
    EXPECT_EQ(actual->metaObject()->property(i).isReadable(), prop.isReadable());
    EXPECT_EQ(actual->metaObject()->property(i).isWritable(), prop.isWritable());
    EXPECT_EQ(actual->metaObject()->property(i).hasNotifySignal(), prop.hasNotifySignal());
    expectValuesEqual(actual->property(prop.name()), expected->property(prop.name()));
  }

  ASSERT_EQ(actual->childCount(), expected->childCount());

  for (size_t i = 0; i < actual->childCount(); i++) {
    compareNodes(actual->childAt(i), expected->childAt(i));
  }
}

auto IoFixture::setNodeValues(std::shared_ptr<yoyo::node_base> node) -> void
{
  auto meta = node->metaObject();

  static std::map<std::string, std::function<QVariant()>> const randomizer {
    { std::string { "yoyo::types::value_t" },
      []() -> QVariant {
        return QVariant::fromValue(generateRandomValue<yoyo::types::value_t>(
          yoyo::types::value_t::BIT, yoyo::types::value_t::CUSTOM));
      } },
    { std::string { "yoyo::types::access_t" },
      []() -> QVariant {
        return QVariant::fromValue(generateRandomValue<yoyo::types::access_t>(
          yoyo::types::access_t::NONE, yoyo::types::access_t::READ_WRITE));
      } },
    { std::string { "yoyo::types::layout_direction_t" },
      []() -> QVariant {
        return QVariant::fromValue(generateRandomValue<yoyo::types::layout_direction_t>(
          yoyo::types::layout_direction_t::FREE, yoyo::types::layout_direction_t::VERTICAL));
      } },
    { std::string { "QString" },
      []() -> QVariant { return QVariant { generateRandomValue<QString>("", "") }; } },
    { std::string { "yoyo::properties::text_t" },
      []() -> QVariant { return QVariant::fromValue(generateRandomValue<QString>("", "")); } },
    { std::string { "yoyo::properties::string_t" },
      []() -> QVariant { return QVariant { generateRandomValue<QString>("", "") }; } },
    { std::string { "yoyo::properties::invisible_string_t" },
      []() -> QVariant {
        return QVariant::fromValue(yoyo::properties::invisible_string_t {
          generateRandomValue<QString>("", ""), generateRandomValue<int>(0, 1) == 1 });
      } },
    { std::string { "yoyo::properties::invisible_layout_direction_t" },
      []() -> QVariant {
        return QVariant::fromValue(yoyo::properties::invisible_layout_direction_t {
          generateRandomValue<yoyo::types::layout_direction_t>(
            yoyo::types::layout_direction_t::FREE, yoyo::types::layout_direction_t::VERTICAL),
          generateRandomValue<int>(0, 1) == 1 });
      } },
    { std::string { "yoyo::properties::connected_string_t" },
      []() -> QVariant {
        return QVariant::fromValue(
          yoyo::properties::connected_string_t { {}, generateRandomValue<QString>("", "") });
      } },
    { std::string { "yoyo::properties::connected_boolean_t" },
      []() -> QVariant {
        return QVariant::fromValue(yoyo::properties::connected_boolean_t {
          generateRandomValue<yoyo::properties::connected_boolean_t::boolean_t>(
            yoyo::properties::connected_boolean_t::boolean_t::TRUE,
            yoyo::properties::connected_boolean_t::boolean_t::DEFINED_BY_CONNECTION),
          false });
      } },
    { std::string { "yoyo::properties::connection_t" },
      []() -> QVariant {
        return QVariant::fromValue(
          yoyo::properties::connection_t { {},
                                           {},
                                           {},
                                           generateRandomValue<QString>("", ""),
                                           generateRandomValue<QString>("", ""),
                                           generateRandomValue<QString>("", "") });
      } },
    { std::string { "yoyo::properties::enum_t" }, []() -> QVariant { return QVariant(); } },
    { std::string { "yoyo::properties::patterned_string_t" },
      []() -> QVariant {
        return QVariant::fromValue(
          yoyo::properties::patterned_string_t { generateRandomValue<QString>("", ""), {} });
      } },
    { std::string { "yoyo::properties::limited_uint8_t" },
      []() -> QVariant {
        return QVariant::fromValue(yoyo::properties::limited_uint8_t {
          generateRandomValue<uint8_t>(std::numeric_limits<uint8_t>::min(),
                                       std::numeric_limits<uint8_t>::max()),
          std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max() });
      } },
    { std::string { "yoyo::properties::limited_uint16_t" },
      []() -> QVariant {
        return QVariant::fromValue(yoyo::properties::limited_uint16_t {
          generateRandomValue<uint16_t>(std::numeric_limits<uint16_t>::min(),
                                        std::numeric_limits<uint16_t>::max()),
          std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max() });
      } },
    { std::string { "yoyo::properties::limited_uint32_t" },
      []() -> QVariant {
        return QVariant::fromValue(yoyo::properties::limited_uint32_t {
          generateRandomValue<uint32_t>(std::numeric_limits<uint32_t>::min(),
                                        std::numeric_limits<uint32_t>::max()),
          std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max() });
      } },
    { std::string { "yoyo::properties::limited_uint64_t" },
      []() -> QVariant {
        return QVariant::fromValue(yoyo::properties::limited_uint64_t {
          generateRandomValue<uint64_t>(std::numeric_limits<uint64_t>::min(),
                                        std::numeric_limits<uint64_t>::max()),
          std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max() });
      } },
    { std::string { "yoyo::properties::limited_int8_t" },
      []() -> QVariant {
        return QVariant::fromValue(yoyo::properties::limited_int8_t {
          generateRandomValue<int8_t>(std::numeric_limits<int8_t>::min(),
                                      std::numeric_limits<int8_t>::max()),
          std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max() });
      } },
    { std::string { "yoyo::properties::limited_int16_t" },
      []() -> QVariant {
        return QVariant::fromValue(yoyo::properties::limited_int16_t {
          generateRandomValue<int16_t>(std::numeric_limits<int16_t>::min(),
                                       std::numeric_limits<int16_t>::max()),
          std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max() });
      } },
    { std::string { "yoyo::properties::limited_int32_t" },
      []() -> QVariant {
        return QVariant::fromValue(yoyo::properties::limited_int32_t {
          generateRandomValue<int32_t>(std::numeric_limits<int32_t>::min(),
                                       std::numeric_limits<int32_t>::max()),
          std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max() });
      } },
    { std::string { "yoyo::properties::limited_int64_t" },
      []() -> QVariant {
        return QVariant::fromValue(yoyo::properties::limited_int64_t {
          generateRandomValue<int64_t>(std::numeric_limits<int64_t>::min(),
                                       std::numeric_limits<int64_t>::max()),
          std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max() });
      } },
    { std::string { "yoyo::properties::limited_float_t" },
      []() -> QVariant {
        return QVariant::fromValue(yoyo::properties::limited_float_t {
          generateRandomValue<float>(std::numeric_limits<float>::min(),
                                     std::numeric_limits<float>::max()),
          std::numeric_limits<float>::min(), std::numeric_limits<float>::max() });
      } },
    { std::string { "yoyo::properties::script_t" }, []() -> QVariant { return {}; } },
    { std::string { "int8_t" },
      []() -> QVariant {
        return QVariant::fromValue(generateRandomValue<int8_t>(std::numeric_limits<int8_t>::min(),
                                                               std::numeric_limits<int8_t>::max()));
      } },
    { std::string { "int16_t" },
      []() -> QVariant {
        return QVariant::fromValue(generateRandomValue<int16_t>(
          std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max()));
      } },
    { std::string { "int32_t" },
      []() -> QVariant {
        return QVariant::fromValue(generateRandomValue<int32_t>(
          std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()));
      } },
    { std::string { "int64_t" },
      []() -> QVariant {
        return QVariant::fromValue(generateRandomValue<int64_t>(
          std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max()));
      } },
    { std::string { "uint8_t" },
      []() -> QVariant {
        return QVariant::fromValue(generateRandomValue<uint8_t>(
          std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max()));
      } },
    { std::string { "uint16_t" },
      []() -> QVariant {
        return QVariant::fromValue(generateRandomValue<uint16_t>(
          std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()));
      } },
    { std::string { "uint32_t" },
      []() -> QVariant {
        return QVariant::fromValue(generateRandomValue<uint32_t>(
          std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()));
      } },
    { std::string { "uint64_t" },
      []() -> QVariant {
        return QVariant::fromValue(generateRandomValue<uint64_t>(
          std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max()));
      } },
  };

  for (auto i = yoyo::node_base::staticMetaObject.propertyOffset(); i < meta->propertyCount();
       i++) {
    if (generateRandomValue<int>(0, 10) < 2) {
      continue;
    }

    auto prop = meta->property(i);
    auto it = randomizer.find(prop.typeName());

    if (it != randomizer.end()) {
      node->setProperty(prop.name(), it->second());
    }
  }
}

auto IoFixture::createRandomNode() -> std::shared_ptr<yoyo::node_base>
{
  auto rnd = generateRandomValue<size_t>(1, _ids.size() - 1);
  auto node = _factories[_ids[rnd].second]->createNode(_ids[rnd].first, {});
  setNodeValues(node);
  return node;
}

auto IoFixture::roll(int min, int max) -> int
{
  return generateRandomValue<int>(min, max);
}

#include "io_fixture.moc"
