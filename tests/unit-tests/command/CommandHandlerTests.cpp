#include "command_handler.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::_;

struct Executor {
  MOCK_METHOD(void, executeCalled, ());
  MOCK_METHOD(void, undoCalled, ());
  MOCK_METHOD(void, redoCalled, ());
};

struct SignalReceiver {
  MOCK_METHOD(void, onTopUndoItemChanged, (bool isAvailable, QString name));
  MOCK_METHOD(void, onTopRedoItemChanged, (bool isAvailable, QString name));
};

/*!
 * \brief CommandHandlerTests.undoRedoStack
 * This test checks whether all tasks pushed to the undo stack can be undone in
 * the right order and redone afterwards.
 */
TEST(CommandHandlerTests, undoRedoStack)
{
  testing::StrictMock<Executor> e1;
  testing::StrictMock<Executor> e2;
  testing::StrictMock<Executor> e3;
  testing::StrictMock<Executor> e4;
  testing::StrictMock<Executor> e5;

  yoyo::command::command_handler handler { 10 };

  testing::StrictMock<SignalReceiver> receiver;
  QObject::connect(&handler, &yoyo::command::command_handler::topUndoItemChanged,
                   [&receiver](auto a, auto n) { receiver.onTopUndoItemChanged(a, n); });
  QObject::connect(&handler, &yoyo::command::command_handler::topRedoItemChanged,
                   [&receiver](auto a, auto n) { receiver.onTopRedoItemChanged(a, n); });

  EXPECT_FALSE(handler.hasCommandToUndo());
  EXPECT_FALSE(handler.hasCommandToRedo());
  EXPECT_EQ(handler.nextUndo(), "");
  EXPECT_EQ(handler.nextRedo(), "");

  // Undo only available in edit mode:
  {
    EXPECT_CALL(receiver, onTopUndoItemChanged(_, _)).Times(0);
    EXPECT_CALL(receiver, onTopRedoItemChanged(_, _)).Times(0);
    EXPECT_CALL(e1, executeCalled()).Times(2);
    handler.execute({ "task1", [&e1] { e1.executeCalled(); }, [&e1] { e1.undoCalled(); },
                      [&e1] { e1.redoCalled(); } });
    handler.execute({ "task2", [&e1] { e1.executeCalled(); }, [&e1] { e1.undoCalled(); },
                      [&e1] { e1.redoCalled(); } });
    EXPECT_FALSE(handler.hasCommandToUndo());
    EXPECT_FALSE(handler.hasCommandToRedo());
    EXPECT_EQ(handler.nextUndo(), "");
    EXPECT_EQ(handler.nextRedo(), "");
  }

  EXPECT_CALL(receiver, onTopUndoItemChanged(false, _));
  EXPECT_CALL(receiver, onTopRedoItemChanged(false, _));
  handler.setEditMode(true);
  {
    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task1")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(false, _));
    EXPECT_CALL(e1, executeCalled());
    handler.execute({ "task1", [&e1] { e1.executeCalled(); }, [&e1] { e1.undoCalled(); },
                      [&e1] { e1.redoCalled(); } });
    EXPECT_TRUE(handler.hasCommandToUndo());
    EXPECT_FALSE(handler.hasCommandToRedo());
    EXPECT_EQ(handler.nextUndo(), "task1");
    EXPECT_EQ(handler.nextRedo(), "");
  }
  {
    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task2")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(false, QString("")));
    EXPECT_CALL(e2, executeCalled());
    handler.execute({ "task2", [&e2] { e2.executeCalled(); }, [&e2] { e2.undoCalled(); },
                      [&e2] { e2.redoCalled(); } });
    EXPECT_TRUE(handler.hasCommandToUndo());
    EXPECT_FALSE(handler.hasCommandToRedo());
    EXPECT_EQ(handler.nextUndo(), "task2");
    EXPECT_EQ(handler.nextRedo(), "");
  }
  {
    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task3")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(false, QString("")));
    EXPECT_CALL(e3, executeCalled());
    handler.execute({ "task3", [&e3] { e3.executeCalled(); }, [&e3] { e3.undoCalled(); },
                      [&e3] { e3.redoCalled(); } });
    EXPECT_TRUE(handler.hasCommandToUndo());
    EXPECT_FALSE(handler.hasCommandToRedo());
    EXPECT_EQ(handler.nextUndo(), "task3");
    EXPECT_EQ(handler.nextRedo(), "");
  }
  {
    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task2")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(true, QString("task3")));
    EXPECT_CALL(e3, undoCalled());
    handler.undo();
    EXPECT_TRUE(handler.hasCommandToUndo());
    EXPECT_TRUE(handler.hasCommandToRedo());
    EXPECT_EQ(handler.nextUndo(), "task2");
    EXPECT_EQ(handler.nextRedo(), "task3");
  }
  {
    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task1")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(true, QString("task2")));
    EXPECT_CALL(e2, undoCalled());
    handler.undo();
    EXPECT_TRUE(handler.hasCommandToUndo());
    EXPECT_TRUE(handler.hasCommandToRedo());
    EXPECT_EQ(handler.nextUndo(), "task1");
    EXPECT_EQ(handler.nextRedo(), "task2");
  }
  {
    EXPECT_CALL(receiver, onTopUndoItemChanged(false, QString("")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(true, QString("task1")));
    EXPECT_CALL(e1, undoCalled());
    handler.undo();
    EXPECT_FALSE(handler.hasCommandToUndo());
    EXPECT_TRUE(handler.hasCommandToRedo());
    EXPECT_EQ(handler.nextUndo(), "");
    EXPECT_EQ(handler.nextRedo(), "task1");
  }
  {
    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task1")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(true, QString("task2")));
    EXPECT_CALL(e1, redoCalled());
    handler.redo();
    EXPECT_TRUE(handler.hasCommandToUndo());
    EXPECT_TRUE(handler.hasCommandToRedo());
    EXPECT_EQ(handler.nextUndo(), "task1");
    EXPECT_EQ(handler.nextRedo(), "task2");
  }
  {
    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task2")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(true, QString("task3")));
    EXPECT_CALL(e2, redoCalled());
    handler.redo();
    EXPECT_TRUE(handler.hasCommandToUndo());
    EXPECT_TRUE(handler.hasCommandToRedo());
    EXPECT_EQ(handler.nextUndo(), "task2");
    EXPECT_EQ(handler.nextRedo(), "task3");
  }
  {
    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task3")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(false, QString("")));
    EXPECT_CALL(e3, redoCalled());
    handler.redo();
    EXPECT_TRUE(handler.hasCommandToUndo());
    EXPECT_FALSE(handler.hasCommandToRedo());
    EXPECT_EQ(handler.nextUndo(), "task3");
    EXPECT_EQ(handler.nextRedo(), "");
  }
}

/*!
 * \brief CommandHandlerTests.clearStackTest
 * This test checks, whether the clearing of the complete or parts of the stack is
 * done properly:
 * - CommandHandler::clearStack
 * - CommandHandler::execute
 */
TEST(CommandHandlerTests, clearStackTest)
{
  testing::StrictMock<Executor> e1;
  testing::StrictMock<Executor> e2;
  testing::StrictMock<Executor> e3;
  testing::StrictMock<Executor> e4;
  testing::StrictMock<Executor> e5;

  yoyo::command::command_handler handler { 10 };

  testing::StrictMock<SignalReceiver> receiver;
  QObject::connect(&handler, &yoyo::command::command_handler::topUndoItemChanged,
                   [&receiver](auto a, auto n) { receiver.onTopUndoItemChanged(a, n); });
  QObject::connect(&handler, &yoyo::command::command_handler::topRedoItemChanged,
                   [&receiver](auto a, auto n) { receiver.onTopRedoItemChanged(a, n); });

  EXPECT_CALL(receiver, onTopUndoItemChanged(false, _));
  EXPECT_CALL(receiver, onTopRedoItemChanged(false, _));
  handler.setEditMode(true);

  // Prepare stack with 4 tasks, one already undone
  {
    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task1")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(false, _));
    EXPECT_CALL(e1, executeCalled());
    handler.execute({ "task1", [&e1] { e1.executeCalled(); }, [&e1] { e1.undoCalled(); },
                      [&e1] { e1.redoCalled(); } });

    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task2")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(false, _));
    EXPECT_CALL(e2, executeCalled());
    handler.execute({ "task2", [&e2] { e2.executeCalled(); }, [&e2] { e2.undoCalled(); },
                      [&e2] { e2.redoCalled(); } });

    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task3")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(false, _));
    EXPECT_CALL(e3, executeCalled());
    handler.execute({ "task3", [&e3] { e3.executeCalled(); }, [&e3] { e3.undoCalled(); },
                      [&e3] { e3.redoCalled(); } });

    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task4")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(false, _));
    EXPECT_CALL(e4, executeCalled());
    handler.execute({ "task4", [&e4] { e4.executeCalled(); }, [&e4] { e4.undoCalled(); },
                      [&e4] { e4.redoCalled(); } });

    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task3")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(true, QString("task4")));
    EXPECT_CALL(e4, undoCalled());
    handler.undo();
  }
  EXPECT_TRUE(handler.hasCommandToRedo());

  // Executing a new task clears the redo stack
  EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task5")));
  EXPECT_CALL(receiver, onTopRedoItemChanged(false, _));
  EXPECT_CALL(e5, executeCalled());
  handler.execute({ "task5", [&e5] { e5.executeCalled(); }, [&e5] { e5.undoCalled(); },
                    [&e5] { e5.redoCalled(); } });
  EXPECT_FALSE(handler.hasCommandToRedo());

  // Prepare stack with 4 tasks, two undone
  {
    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task3")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(true, QString("task5")));
    EXPECT_CALL(e5, undoCalled());
    handler.undo();

    EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task2")));
    EXPECT_CALL(receiver, onTopRedoItemChanged(true, QString("task3")));
    EXPECT_CALL(e3, undoCalled());
    handler.undo();

    EXPECT_TRUE(handler.hasCommandToRedo());
    EXPECT_TRUE(handler.hasCommandToUndo());
  }

  // Clearing removes them all
  EXPECT_CALL(receiver, onTopUndoItemChanged(false, _));
  EXPECT_CALL(receiver, onTopRedoItemChanged(false, _));
  handler.clearStack();
  EXPECT_FALSE(handler.hasCommandToRedo());
  EXPECT_FALSE(handler.hasCommandToUndo());
}

/*!
 * \brief CommandHandlerTests.blockTest
 * This test checks, whether blocking the recording on the undo stack is working.
 */
TEST(CommandHandlerTests, blockTest)
{
  testing::StrictMock<Executor> e1;
  testing::StrictMock<Executor> e2;
  testing::StrictMock<Executor> e3;
  testing::StrictMock<Executor> e4;
  testing::StrictMock<Executor> e5;

  yoyo::command::command_handler handler { 10 };
  yoyo::command::initialize_commandhandler(&handler);

  testing::StrictMock<SignalReceiver> receiver;
  QObject::connect(&handler, &yoyo::command::command_handler::topUndoItemChanged,
                   [&receiver](auto a, auto n) { receiver.onTopUndoItemChanged(a, n); });
  QObject::connect(&handler, &yoyo::command::command_handler::topRedoItemChanged,
                   [&receiver](auto a, auto n) { receiver.onTopRedoItemChanged(a, n); });

  EXPECT_CALL(receiver, onTopUndoItemChanged(false, _));
  EXPECT_CALL(receiver, onTopRedoItemChanged(false, _));
  handler.setEditMode(true);

  EXPECT_FALSE(handler.isRecordingBlocked());
  // Single blockers
  {
    yoyo::command::NoUndoRecording here;
    EXPECT_TRUE(handler.isRecordingBlocked());

    EXPECT_CALL(e1, executeCalled());
    handler.execute({ "task1", [&e1] { e1.executeCalled(); }, [&e1] { e1.undoCalled(); },
                      [&e1] { e1.redoCalled(); } });
    EXPECT_FALSE(handler.hasCommandToUndo());
    EXPECT_FALSE(handler.hasCommandToRedo());

    EXPECT_CALL(e2, executeCalled());
    handler.execute({ "task2", [&e2] { e2.executeCalled(); }, [&e2] { e2.undoCalled(); },
                      [&e2] { e2.redoCalled(); } });
    EXPECT_FALSE(handler.hasCommandToUndo());
    EXPECT_FALSE(handler.hasCommandToRedo());
  }
  EXPECT_FALSE(handler.isRecordingBlocked());
  EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task1")));
  EXPECT_CALL(receiver, onTopRedoItemChanged(false, _));
  EXPECT_CALL(e1, executeCalled());
  handler.execute({ "task1", [&e1] { e1.executeCalled(); }, [&e1] { e1.undoCalled(); },
                    [&e1] { e1.redoCalled(); } });

  // Nested blockers
  {
    yoyo::command::NoUndoRecording here;
    EXPECT_TRUE(handler.isRecordingBlocked());

    EXPECT_CALL(e1, executeCalled());
    handler.execute({ "unrecorded task1", [&e1] { e1.executeCalled(); }, [&e1] { e1.undoCalled(); },
                      [&e1] { e1.redoCalled(); } });
    EXPECT_EQ(handler.nextUndo(), "task1");
    EXPECT_FALSE(handler.hasCommandToRedo());

    EXPECT_CALL(e2, executeCalled());
    handler.execute({ "unrecorded task2", [&e2] { e2.executeCalled(); }, [&e2] { e2.undoCalled(); },
                      [&e2] { e2.redoCalled(); } });
    EXPECT_EQ(handler.nextUndo(), "task1");
    EXPECT_FALSE(handler.hasCommandToRedo());
    {
      yoyo::command::NoUndoRecording here;
      EXPECT_TRUE(handler.isRecordingBlocked());

      EXPECT_CALL(e1, executeCalled());
      handler.execute({ "unrecorded task1", [&e1] { e1.executeCalled(); },
                        [&e1] { e1.undoCalled(); }, [&e1] { e1.redoCalled(); } });
      EXPECT_EQ(handler.nextUndo(), "task1");
      EXPECT_FALSE(handler.hasCommandToRedo());

      EXPECT_CALL(e2, executeCalled());
      handler.execute({ "unrecorded task2", [&e2] { e2.executeCalled(); },
                        [&e2] { e2.undoCalled(); }, [&e2] { e2.redoCalled(); } });
      EXPECT_EQ(handler.nextUndo(), "task1");
      EXPECT_FALSE(handler.hasCommandToRedo());
    }
    EXPECT_TRUE(handler.isRecordingBlocked());
    EXPECT_CALL(e1, executeCalled());
    handler.execute({ "unrecorded task1", [&e1] { e1.executeCalled(); }, [&e1] { e1.undoCalled(); },
                      [&e1] { e1.redoCalled(); } });
    EXPECT_EQ(handler.nextUndo(), "task1");
    EXPECT_FALSE(handler.hasCommandToRedo());
  }
  EXPECT_FALSE(handler.isRecordingBlocked());
  EXPECT_CALL(receiver, onTopUndoItemChanged(true, QString("task2")));
  EXPECT_CALL(receiver, onTopRedoItemChanged(false, _));
  EXPECT_CALL(e2, executeCalled());
  handler.execute({ "task2", [&e2] { e2.executeCalled(); }, [&e2] { e2.undoCalled(); },
                    [&e2] { e2.redoCalled(); } });
}

/*!
 * \brief CommandHandlerTests.stackSize
 * Push 4 commands on a CommandHandler with a maximum size of 3 commands.
 * The oldest command must be dropped.
 */
TEST(CommandHandlerTests, stackSize)
{
  yoyo::command::command_handler handler { 3 };
  handler.setEditMode(true);

  handler.execute({ "task1", [] {}, [] {} });
  handler.execute({ "task2", [] {}, [] {} });
  handler.execute({ "task3", [] {}, [] {} });
  EXPECT_EQ(handler.nextUndo(), "task3");
  handler.execute({ "task4", [] {}, [] {} });
  EXPECT_EQ(handler.nextUndo(), "task4");
  handler.undo();
  EXPECT_EQ(handler.nextUndo(), "task3");
  EXPECT_TRUE(handler.hasCommandToUndo());
  handler.undo();
  EXPECT_EQ(handler.nextUndo(), "task2");
  EXPECT_TRUE(handler.hasCommandToUndo());
  handler.undo();
  EXPECT_EQ(handler.nextUndo(), "");
  EXPECT_FALSE(handler.hasCommandToUndo());
}

/*!
 * \brief CommandHandlerTests.limitedCommands
 * This test verifies, that commands which are either invalid or do not have an
 * undo functor defined, are not pushed to undo-stack.
 */
TEST(CommandHandlerTests, limitedCommands)
{
  yoyo::command::command_handler handler { 10 };
  handler.setEditMode(true);
  testing::StrictMock<Executor> e1;
  testing::StrictMock<Executor> e2;

  // Valid task is expected to be put on the stack
  EXPECT_CALL(e1, executeCalled());
  handler.execute({ "valid task", [&e1] { e1.executeCalled(); }, [&e1] { e1.undoCalled(); } });
  EXPECT_EQ(handler.nextUndo(), "valid task");

  // Invalid task is ignored
  handler.execute({ "invalid task", {}, [&e2] { e2.undoCalled(); } });
  EXPECT_EQ(handler.nextUndo(), "valid task");

  // Not undoable task is executed but not put on the stack
  EXPECT_CALL(e2, undoCalled());
  handler.execute({ "not undoable task", [&e2] { e2.undoCalled(); }, {} });
  EXPECT_EQ(handler.nextUndo(), "valid task");
}
