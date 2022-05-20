#pragma once
#include "command.h"
#include "command_export.h"

#include <QObject>

#include <deque>

namespace yoyo
{
namespace command
{
///
/// \brief The CommandHandler class
///
/// This class defines the access point of the undo-framework.
/// It executes commands and manages a list of commands to undo/redo.
///
class YOYO_COMMAND_SHARED_EXPORT command_handler : public QObject
{
  Q_OBJECT
public:
  ///
  /// \brief Constructor
  /// \param stackSize the maximum size of the undo stack.
  ///
  command_handler(size_t stackSize);
  ~command_handler() = default;
  command_handler(command_handler const&) = delete;
  command_handler(command_handler&&) = delete;
  command_handler& operator=(command_handler const&) = delete;
  command_handler& operator=(command_handler&&) = delete;

  ///
  /// \brief execute a command
  ///
  /// Executes the provided command and pushes it to the undo stack.
  /// If the stack exceeds the maximum size of commands, the oldest command
  /// will be dropped.
  /// The redo-stack will be cleared for consistency reasons.
  ///
  /// If the command is not undoable, it will only be executed; the undo/redo
  /// stacks will be left untouched.
  ///
  /// Emits signals topUndoItemChanged and topRedoItemChanged.
  /// \param c the command to execute.
  ///
  auto execute(command c) -> void;

  ///
  /// \brief undo
  ///
  /// Calls undo on the top-most command on the undo stack (if it exists) and moves
  /// it to the redo-stack.
  /// Emits signals topUndoItemChanged and topRedoItemChanged.
  ///
  auto undo() -> void;

  ///
  /// \brief redo
  ///
  /// Calls redo on the top-most command on the redo stack (if it exists) and moves
  /// it to the undo-stack.
  /// Emits signals topUndoItemChanged and topRedoItemChanged.
  ///
  auto redo() -> void;

  ///
  /// \brief name of next Undo operation
  /// \return the name of the top-most command on the undo-stack.
  ///
  auto nextUndo() const -> QString;

  ///
  /// \brief name of next Redo operation
  /// \return the name of the top-most command on the redo-stack.
  ///
  auto nextRedo() const -> QString;

  ///
  /// \brief query whether there are commands to undo
  /// \return true if the undo-stack is not empty.
  ///
  auto hasCommandToUndo() const -> bool;

  ///
  /// \brief query whether there are commands to redo
  /// \return true if the redo-stack is not empty.
  ///
  auto hasCommandToRedo() const -> bool;

  ///
  /// \brief clear the undo-Stack
  ///
  /// Removes all items from both stacks.
  /// Emits signals topUndoItemChanged and topRedoItemChanged.
  ///
  auto clearStack() -> void;

  ///
  /// \brief set edit mode
  ///
  /// It should only be possible to use undo, if OneEye is in edit-mode. So recording
  /// will be switched off if not in edit-mode. Commands will still be executed, but
  /// nothing will be pushed to undo-stack.
  /// \param mode
  ///
  auto setEditMode(bool mode) -> void;

  ///
  /// \brief block recording
  ///
  /// During the execution of an operation, it is forbidden to record further undoable
  /// commands. That's why they are named atomic operations.
  /// In practice it can happen, that a call to some function, which is used in an operation,
  /// would request a command to be recorded.
  /// To prevent recording, blockRecording can be called.
  /// Take care to unblock the command afterwards. Or use the NoUndoRecording helper.
  ///
  auto blockRecording() -> void;

  ///
  /// \brief unblock recording
  ///
  /// During the execution of an operation, it is forbidden to record further undoable
  /// commands. That's why they are named atomic operations.
  /// In practice it can happen, that a call to some function, which is used in an operation,
  /// would request a command to be recorded.
  /// To prevent recording, blockRecording can be called.
  /// Take care to unblock the command afterwards. Or use the NoUndoRecording helper.
  ///
  auto unblockRecording() -> void;

  ///
  /// \brief query whether recording is blocked
  /// \return true if the recording to undo-stack is currently blocked.
  ///
  auto isRecordingBlocked() const -> bool;

Q_SIGNALS:
  ///
  /// \brief the top Undo item has changed
  ///
  /// This signal is emitted, when the top-most item on the undo stack changed.
  /// \param isAvailable true if there is a command on the undo stack,
  /// \param name the name of the top-most command on the undo stack.
  ///
  void topUndoItemChanged(bool isAvailable, QString name);

  ///
  /// \brief the top Redo item has changed
  ///
  /// This signal is emitted, when the top-most item on the redo stack changed.
  /// \param isAvailable true if there is a command on the redo stack,
  /// \param name the name of the top-most command on the redo stack.
  ///
  void topRedoItemChanged(bool isAvailable, QString name);

private:
  // Note on implementation:
  // I had hoped to use the std::stack class, but since we want to restrict the
  // size of the stack (dropping the oldest elements if necessary), this was not
  // possible. So I opted for the next-most suited container: deque.
  std::deque<command> _undoStack;
  std::deque<command> _redoStack;
  size_t const _maxStackSize;

  bool _editMode { false };
  bool _blocked { false };
};

///
/// \brief The NoUndoRecording helper class
///
/// This class is a RAII/guardian class, which can be used to prevent the
/// recording of commands to the undo-stack of the CommandHandler.
///
class YOYO_COMMAND_SHARED_EXPORT NoUndoRecording
{
public:
  NoUndoRecording();
  ~NoUndoRecording();

private:
  bool _hasBlock { true };
};

///
/// \brief access global command handler
/// \return the global command handler
///
auto YOYO_COMMAND_SHARED_EXPORT commandhandler() -> command_handler*;

///
/// \brief initialize command handler
///
/// Initializes the global command handler. This has to be done once - and only once - by the main
/// application (or the test driver in case of unit-tests).
/// \param handler the command handler to use as global command handler.
///
auto YOYO_COMMAND_SHARED_EXPORT initialize_commandhandler(command_handler* handler) -> void;
} // namespace command
} // namespace yoyo
