#include "io_manager.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/uuid/random_generator.hpp>

class serializer_mock : public yoyo::io::node_serializer
{
public:
  using yoyo::io::node_serializer::node_serializer;
  MOCK_METHOD(void, serialize, (std::ostream & output, std::shared_ptr<yoyo::node_base> root_node),
              (const, override));
};

class deserializer_mock : public yoyo::io::node_deserializer
{
public:
  using yoyo::io::node_deserializer::node_deserializer;
  MOCK_METHOD(std::shared_ptr<yoyo::node_base>, deserialize, (std::istream & input),
              (const, override));
};

TEST(IoManagerTests, installInitialized)
{
  EXPECT_TRUE(yoyo::io::manager().expired());

  // can be initialized using no factories, because they are not really used due to mock
  // deserializer.
  yoyo::io::init_manager(
    std::make_shared<yoyo::io::io_manager>(std::vector<std::shared_ptr<yoyo::node_factory>> {}));

  EXPECT_FALSE(yoyo::io::manager().expired());

  auto manager = yoyo::io::manager().lock();
  ASSERT_TRUE(manager);

  EXPECT_THAT(manager->installed_serializers(), testing::IsEmpty());
  EXPECT_THAT(manager->installed_deserializers(), testing::IsEmpty());

  auto id1 = boost::uuids::random_generator {}();
  {
    EXPECT_EQ(manager->serializer(id1), nullptr);
    EXPECT_EQ(manager->deserializer(id1), nullptr);
    yoyo::io::io_manager::install_serializer(
      id1, "test1", "test1", "*.txt", [](auto f) { return std::make_shared<serializer_mock>(f); });

    EXPECT_THAT(manager->installed_serializers(), testing::SizeIs(1));
    EXPECT_THAT(manager->installed_deserializers(), testing::IsEmpty());
    EXPECT_NE(manager->serializer(id1), nullptr);
    EXPECT_EQ(manager->deserializer(id1), nullptr);
    EXPECT_EQ(manager->installed_serializers()[0],
              std::make_tuple(id1, std::string { "test1" }, std::string { "test1" },
                              std::string { "*.txt" }));
  }

  auto id2 = boost::uuids::random_generator {}();
  {
    EXPECT_EQ(manager->serializer(id2), nullptr);
    EXPECT_EQ(manager->deserializer(id2), nullptr);
    yoyo::io::io_manager::install_serializer(
      id2, "test2", "test2", "*.txt", [](auto f) { return std::make_shared<serializer_mock>(f); });

    EXPECT_THAT(manager->installed_serializers(), testing::SizeIs(2));
    EXPECT_THAT(manager->installed_deserializers(), testing::IsEmpty());
    EXPECT_NE(manager->serializer(id2), nullptr);
    EXPECT_EQ(manager->deserializer(id2), nullptr);
    EXPECT_EQ(manager->installed_serializers()[1],
              std::make_tuple(id2, std::string { "test2" }, std::string { "test2" },
                              std::string { "*.txt" }));
  }
  auto id3 = boost::uuids::random_generator {}();
  {
    EXPECT_EQ(manager->serializer(id3), nullptr);
    EXPECT_EQ(manager->deserializer(id3), nullptr);
    yoyo::io::io_manager::install_deserializer(id3, "test3", "test3", "*.txt", [](auto f) {
      return std::make_shared<deserializer_mock>(f);
    });

    EXPECT_THAT(manager->installed_serializers(), testing::SizeIs(2));
    EXPECT_THAT(manager->installed_deserializers(), testing::SizeIs(1));
    EXPECT_NE(manager->deserializer(id3), nullptr);
    EXPECT_EQ(manager->serializer(id3), nullptr);
    EXPECT_EQ(manager->installed_deserializers()[0],
              std::make_tuple(id3, std::string { "test3" }, std::string { "test3" },
                              std::string { "*.txt" }));
  }

  auto id4 = boost::uuids::random_generator {}();
  {
    EXPECT_EQ(manager->serializer(id4), nullptr);
    EXPECT_EQ(manager->deserializer(id4), nullptr);
    yoyo::io::io_manager::install_deserializer(id4, "test4", "test4", "*.txt", [](auto f) {
      return std::make_shared<deserializer_mock>(f);
    });

    EXPECT_THAT(manager->installed_serializers(), testing::SizeIs(2));
    EXPECT_THAT(manager->installed_deserializers(), testing::SizeIs(2));
    EXPECT_NE(manager->deserializer(id4), nullptr);
    EXPECT_EQ(manager->serializer(id4), nullptr);
    EXPECT_EQ(manager->installed_deserializers()[1],
              std::make_tuple(id4, std::string { "test4" }, std::string { "test4" },
                              std::string { "*.txt" }));
  }

  auto id5 = boost::uuids::random_generator {}();
  {
    EXPECT_EQ(manager->serializer(id5), nullptr);
    EXPECT_EQ(manager->deserializer(id5), nullptr);
    EXPECT_FALSE(manager->install_serializer(
      id1, "test5", "test5", "*.txt",
      std::make_shared<serializer_mock>(std::vector<std::shared_ptr<yoyo::node_factory>> {})));
    EXPECT_THAT(manager->installed_serializers(), testing::SizeIs(2));
    EXPECT_TRUE(manager->install_serializer(
      id5, "test5", "test5", "*.txt",
      std::make_shared<serializer_mock>(std::vector<std::shared_ptr<yoyo::node_factory>> {})));

    EXPECT_THAT(manager->installed_serializers(), testing::SizeIs(3));
    EXPECT_THAT(manager->installed_deserializers(), testing::SizeIs(2));
    EXPECT_NE(manager->serializer(id5), nullptr);
    EXPECT_EQ(manager->deserializer(id5), nullptr);
    EXPECT_EQ(manager->installed_serializers()[2],
              std::make_tuple(id5, std::string { "test5" }, std::string { "test5" },
                              std::string { "*.txt" }));
  }
  auto id6 = boost::uuids::random_generator {}();
  {
    EXPECT_EQ(manager->serializer(id6), nullptr);
    EXPECT_EQ(manager->deserializer(id6), nullptr);
    EXPECT_FALSE(manager->install_deserializer(
      id3, "test6", "test6", "*.txt",
      std::make_shared<deserializer_mock>(std::vector<std::shared_ptr<yoyo::node_factory>> {})));
    EXPECT_THAT(manager->installed_deserializers(), testing::SizeIs(2));
    EXPECT_TRUE(manager->install_deserializer(
      id6, "test6", "test6", "*.txt",
      std::make_shared<deserializer_mock>(std::vector<std::shared_ptr<yoyo::node_factory>> {})));

    EXPECT_THAT(manager->installed_serializers(), testing::SizeIs(3));
    EXPECT_THAT(manager->installed_deserializers(), testing::SizeIs(3));
    EXPECT_EQ(manager->serializer(id6), nullptr);
    EXPECT_NE(manager->deserializer(id6), nullptr);
    EXPECT_EQ(manager->installed_deserializers()[2],
              std::make_tuple(id6, std::string { "test6" }, std::string { "test6" },
                              std::string { "*.txt" }));
  }

  EXPECT_FALSE(manager->remove_serializer(id6));
  EXPECT_FALSE(manager->remove_deserializer(id1));
  EXPECT_THAT(manager->installed_serializers(), testing::SizeIs(3));
  EXPECT_THAT(manager->installed_deserializers(), testing::SizeIs(3));

  EXPECT_TRUE(manager->remove_serializer(id1));
  EXPECT_THAT(manager->installed_serializers(), testing::SizeIs(2));
  EXPECT_THAT(manager->installed_deserializers(), testing::SizeIs(3));
  EXPECT_EQ(manager->installed_serializers()[0],
            std::make_tuple(id2, std::string { "test2" }, std::string { "test2" },
                            std::string { "*.txt" }));

  EXPECT_TRUE(manager->remove_deserializer(id3));
  EXPECT_THAT(manager->installed_serializers(), testing::SizeIs(2));
  EXPECT_THAT(manager->installed_deserializers(), testing::SizeIs(2));
  EXPECT_EQ(manager->installed_deserializers()[0],
            std::make_tuple(id4, std::string { "test4" }, std::string { "test4" },
                            std::string { "*.txt" }));

  EXPECT_FALSE(manager->remove_serializer(id1));
  EXPECT_FALSE(manager->remove_deserializer(id3));

  EXPECT_TRUE(manager->remove_serializer(id2));
  EXPECT_TRUE(manager->remove_serializer(id5));
  EXPECT_TRUE(manager->remove_deserializer(id4));
  EXPECT_TRUE(manager->remove_deserializer(id6));

  yoyo::io::deinit_manager();
  EXPECT_TRUE(yoyo::io::manager().expired());
}

TEST(IoManagerTests, installUninitialized)
{
  EXPECT_TRUE(yoyo::io::manager().expired());

  auto id1 = boost::uuids::random_generator {}();
  auto id2 = boost::uuids::random_generator {}();
  auto id3 = boost::uuids::random_generator {}();
  auto id4 = boost::uuids::random_generator {}();

  yoyo::io::io_manager::install_serializer(
    id1, "test1", "test1", "*.txt", [](auto f) { return std::make_shared<serializer_mock>(f); });
  yoyo::io::io_manager::install_serializer(
    id2, "test2", "test2", "*.txt", [](auto f) { return std::make_shared<serializer_mock>(f); });
  yoyo::io::io_manager::install_deserializer(
    id3, "test3", "test3", "*.txt", [](auto f) { return std::make_shared<deserializer_mock>(f); });
  yoyo::io::io_manager::install_deserializer(
    id4, "test4", "test4", "*.txt", [](auto f) { return std::make_shared<deserializer_mock>(f); });

  EXPECT_TRUE(yoyo::io::manager().expired());

  yoyo::io::init_manager(
    std::make_shared<yoyo::io::io_manager>(std::vector<std::shared_ptr<yoyo::node_factory>> {}));

  EXPECT_FALSE(yoyo::io::manager().expired());

  auto manager = yoyo::io::manager().lock();
  ASSERT_TRUE(manager);

  ASSERT_THAT(manager->installed_serializers(), testing::SizeIs(2));
  ASSERT_THAT(manager->installed_deserializers(), testing::SizeIs(2));

  EXPECT_THAT(manager->installed_serializers()[0],
              std::make_tuple(id1, std::string { "test1" }, std::string { "test1" },
                              std::string { "*.txt" }));
  EXPECT_THAT(manager->installed_serializers()[1],
              std::make_tuple(id2, std::string { "test2" }, std::string { "test2" },
                              std::string { "*.txt" }));
  EXPECT_THAT(manager->installed_deserializers()[0],
              std::make_tuple(id3, std::string { "test3" }, std::string { "test3" },
                              std::string { "*.txt" }));
  EXPECT_THAT(manager->installed_deserializers()[1],
              std::make_tuple(id4, std::string { "test4" }, std::string { "test4" },
                              std::string { "*.txt" }));

  EXPECT_NE(manager->serializer(id1), nullptr);
  EXPECT_NE(manager->serializer(id2), nullptr);
  EXPECT_NE(manager->deserializer(id3), nullptr);
  EXPECT_NE(manager->deserializer(id4), nullptr);

  auto second_manager =
    std::make_shared<yoyo::io::io_manager>(std::vector<std::shared_ptr<yoyo::node_factory>> {});
  EXPECT_THAT(second_manager->installed_serializers(), testing::IsEmpty());
  EXPECT_THAT(second_manager->installed_deserializers(), testing::IsEmpty());

  yoyo::io::deinit_manager();
}
