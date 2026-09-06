#include "command.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

struct Executor {
  MOCK_METHOD(void, executeCalled, ());
  MOCK_METHOD(void, undoCalled, ());
  MOCK_METHOD(void, redoCalled, ());
};

TEST(CommandTests, singleCommand)
{
  testing::StrictMock<Executor> executor;

  yoyo::command::command c { "test command", [&executor]() { executor.executeCalled(); },
                             [&executor]() { executor.undoCalled(); },
                             [&executor]() { executor.redoCalled(); } };

  EXPECT_EQ(c.name(), "test command");
  EXPECT_TRUE(c.isValid());
  EXPECT_FALSE(c.isExecuted());
  EXPECT_FALSE(c.isUndone());
  EXPECT_TRUE(c.canBeUndone());

  EXPECT_CALL(executor, executeCalled());
  EXPECT_TRUE(c.execute());
  EXPECT_TRUE(c.isValid());
  EXPECT_TRUE(c.isExecuted());
  EXPECT_FALSE(c.isUndone());
  EXPECT_TRUE(c.canBeUndone());

  EXPECT_FALSE(c.execute());
  EXPECT_TRUE(c.isValid());
  EXPECT_TRUE(c.isExecuted());
  EXPECT_FALSE(c.isUndone());
  EXPECT_TRUE(c.canBeUndone());

  EXPECT_FALSE(c.redo());
  EXPECT_TRUE(c.isValid());
  EXPECT_TRUE(c.isExecuted());
  EXPECT_FALSE(c.isUndone());
  EXPECT_TRUE(c.canBeUndone());

  EXPECT_CALL(executor, undoCalled());
  EXPECT_TRUE(c.undo());
  EXPECT_TRUE(c.isValid());
  EXPECT_FALSE(c.isExecuted());
  EXPECT_TRUE(c.isUndone());
  EXPECT_TRUE(c.canBeUndone());

  EXPECT_FALSE(c.execute());
  EXPECT_TRUE(c.isValid());
  EXPECT_FALSE(c.isExecuted());
  EXPECT_TRUE(c.isUndone());
  EXPECT_TRUE(c.canBeUndone());
  EXPECT_FALSE(c.undo());
  EXPECT_TRUE(c.isValid());
  EXPECT_FALSE(c.isExecuted());
  EXPECT_TRUE(c.isUndone());
  EXPECT_TRUE(c.canBeUndone());

  EXPECT_CALL(executor, redoCalled());
  EXPECT_TRUE(c.redo());
  EXPECT_TRUE(c.isValid());
  EXPECT_TRUE(c.isExecuted());
  EXPECT_FALSE(c.isUndone());
  EXPECT_TRUE(c.canBeUndone());

  EXPECT_FALSE(c.execute());
  EXPECT_TRUE(c.isValid());
  EXPECT_TRUE(c.isExecuted());
  EXPECT_FALSE(c.isUndone());
  EXPECT_TRUE(c.canBeUndone());

  EXPECT_CALL(executor, undoCalled());
  EXPECT_TRUE(c.undo());
  EXPECT_TRUE(c.isValid());
  EXPECT_FALSE(c.isExecuted());
  EXPECT_TRUE(c.isUndone());
  EXPECT_TRUE(c.canBeUndone());
}

TEST(CommandTests, twoCommands)
{
  testing::StrictMock<Executor> executor1;
  yoyo::command::command c1 { "test command", [&executor1]() { executor1.executeCalled(); },
                              [&executor1]() { executor1.undoCalled(); },
                              [&executor1]() { executor1.redoCalled(); } };

  testing::StrictMock<Executor> executor2;
  yoyo::command::command c2 { "test command2", [&executor2]() { executor2.executeCalled(); },
                              [&executor2]() { executor2.undoCalled(); },
                              [&executor2]() { executor2.redoCalled(); } };

  EXPECT_EQ(c1.name(), "test command");
  EXPECT_EQ(c2.name(), "test command2");

  EXPECT_CALL(executor1, executeCalled());
  EXPECT_TRUE(c1.execute());

  EXPECT_CALL(executor2, executeCalled());
  EXPECT_TRUE(c2.execute());

  EXPECT_CALL(executor1, undoCalled());
  EXPECT_TRUE(c1.undo());

  EXPECT_CALL(executor1, redoCalled());
  EXPECT_TRUE(c1.redo());

  EXPECT_CALL(executor2, undoCalled());
  EXPECT_TRUE(c2.undo());

  EXPECT_CALL(executor2, redoCalled());
  EXPECT_TRUE(c2.redo());

  EXPECT_CALL(executor2, undoCalled());
  EXPECT_TRUE(c2.undo());

  EXPECT_CALL(executor1, undoCalled());
  EXPECT_TRUE(c1.undo());
}

TEST(CommandTests, limitedCommands)
{
  testing::StrictMock<Executor> e1;
  yoyo::command::command c1 { "invalid", {}, {} };
  EXPECT_FALSE(c1.isValid());
  EXPECT_FALSE(c1.execute());

  yoyo::command::command c2 { "do only command", [&e1] { e1.executeCalled(); }, {} };
  EXPECT_TRUE(c2.isValid());
  EXPECT_FALSE(c2.canBeUndone());
  EXPECT_CALL(e1, executeCalled());
  EXPECT_TRUE(c2.execute());
  EXPECT_FALSE(c2.undo());

  yoyo::command::command c3 { "no redo command", [&e1] { e1.executeCalled(); },
                              [&e1] { e1.undoCalled(); } };
  EXPECT_TRUE(c3.isValid());
  EXPECT_TRUE(c3.canBeUndone());
  EXPECT_CALL(e1, executeCalled());
  EXPECT_TRUE(c3.execute());
  EXPECT_CALL(e1, undoCalled());
  EXPECT_TRUE(c3.undo());
  EXPECT_CALL(e1, executeCalled());
  EXPECT_TRUE(c3.redo());
}
