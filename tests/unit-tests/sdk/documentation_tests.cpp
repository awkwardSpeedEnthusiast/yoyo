#include "yoyo/documentation.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::StrictMock;

namespace yoyo
{
TEST(documentationTest, builder)
{
  yoyo::documentation::builder builder("test_stuff");
  builder.description("foo")
    .styling("bar")
    .property("prop1", "Property 1", "prop1 description", "foo", {})
    .property("prop2", "Property 2", "prop2 description", "bar", QVariant(42))
    .property("prop3", "Property 3", "prop3 description", "fuu", QVariant(QString("Hello World!")));
  auto doc = builder.build();

  ASSERT_TRUE(doc.get() != nullptr);
  EXPECT_EQ(doc->type(), "test_stuff");
  EXPECT_EQ(doc->typeDescription(), "foo");
  EXPECT_EQ(doc->styling(), "bar");

  auto [title, description, tooltip, defaultvalue] = doc->property("prop1");
  EXPECT_EQ(title, "Property 1");
  EXPECT_EQ(description, "prop1 description");
  EXPECT_EQ(tooltip, "foo");
  EXPECT_FALSE(defaultvalue.isValid());

  std::tie(title, description, tooltip, defaultvalue) = doc->property("prop2");
  EXPECT_EQ(title, "Property 2");
  EXPECT_EQ(description, "prop2 description");
  EXPECT_EQ(tooltip, "bar");
  EXPECT_EQ(defaultvalue.toInt(), 42);

  std::tie(title, description, tooltip, defaultvalue) = doc->property("prop3");
  EXPECT_EQ(title, "Property 3");
  EXPECT_EQ(description, "prop3 description");
  EXPECT_EQ(tooltip, "fuu");
  EXPECT_EQ(defaultvalue.toString().toStdString(), "Hello World!");

  std::tie(title, description, tooltip, defaultvalue) = doc->property("prop4");
  EXPECT_EQ(title, "");
  EXPECT_EQ(description, "");
  EXPECT_EQ(tooltip, "");
  EXPECT_FALSE(defaultvalue.isValid());

  builder.styling("shoo")
    .description("tor")
    .property("prop1", "Property 1", "other description", "foo", {})
    .property("prop5", "Property 5", "property 5", "", { -1 });
  auto doc2 = builder.build();
  ASSERT_TRUE(doc2.get() != nullptr);
  EXPECT_EQ(doc2->type(), "test_stuff");
  EXPECT_EQ(doc2->typeDescription(), "tor");
  EXPECT_EQ(doc2->styling(), "shoo");
  EXPECT_EQ(doc2->property("prop1"),
            std::make_tuple(QString { "Property 1" }, QString { "other description" },
                            QString { "foo" }, QVariant {}));
  EXPECT_EQ(doc2->property("prop2"),
            std::make_tuple(QString { "Property 2" }, QString { "prop2 description" },
                            QString { "bar" }, QVariant { 42 }));
  EXPECT_EQ(doc2->property("prop3"),
            std::make_tuple(QString { "Property 3" }, QString { "prop3 description" },
                            QString { "fuu" }, QVariant { "Hello World!" }));
  EXPECT_EQ(doc2->property("prop4"),
            std::make_tuple(QString {}, QString {}, QString {}, QVariant {}));
  EXPECT_EQ(doc2->property("prop5"),
            std::make_tuple(QString { "Property 5" }, QString { "property 5" }, QString { "" },
                            QVariant { -1 }));
}
} // namespace yoyo
