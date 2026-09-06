#include "command_handler.h"

namespace
{
yoyo::command::command_handler* internalCommandHandler { nullptr };
}
namespace yoyo::command
{
auto commandhandler() -> command_handler*
{
  return internalCommandHandler;
}

auto initialize_commandhandler(command_handler* handler) -> void
{
  if (!internalCommandHandler) {
    internalCommandHandler = handler;
  }
}

command_handler::command_handler(size_t stackSize)
  : _maxStackSize(stackSize)
{
}

auto command_handler::execute(command c) -> void
{
  if (!c.isValid()) {
    return;
  }

  c.execute();

  if (!_editMode || _blocked || !c.canBeUndone()) {
    return;
  }

  _undoStack.push_front(c);

  if (_undoStack.size() > _maxStackSize) {
    _undoStack.pop_back();
  }

  _redoStack.clear();
  Q_EMIT topUndoItemChanged(true, c.name());
  Q_EMIT topRedoItemChanged(false, {});
}

auto command_handler::undo() -> void
{
  if (_undoStack.empty()) {
    return;
  }

  auto c = _undoStack.front();
  _undoStack.pop_front();
  c.undo();
  _redoStack.push_front(c);
  Q_EMIT topRedoItemChanged(true, c.name());

  if (_undoStack.empty()) {
    Q_EMIT topUndoItemChanged(false, {});
  } else {
    Q_EMIT topUndoItemChanged(true, _undoStack.front().name());
  }
}

auto command_handler::redo() -> void
{
  if (_redoStack.empty()) {
    return;
  }

  auto c = _redoStack.front();
  _redoStack.pop_front();
  c.redo();
  _undoStack.push_front(c);

  Q_EMIT topUndoItemChanged(true, c.name());

  if (_redoStack.empty()) {
    Q_EMIT topRedoItemChanged(false, {});
  } else {
    Q_EMIT topRedoItemChanged(true, _redoStack.front().name());
  }
}

auto command_handler::nextUndo() const -> QString
{
  if (!_undoStack.empty()) {
    return _undoStack.front().name();
  }

  return {};
}

auto command_handler::nextRedo() const -> QString
{
  if (!_redoStack.empty()) {
    return _redoStack.front().name();
  }

  return {};
}

auto command_handler::hasCommandToUndo() const -> bool
{
  return !_undoStack.empty();
}

auto command_handler::hasCommandToRedo() const -> bool
{
  return !_redoStack.empty();
}

auto command_handler::clearStack() -> void
{
  _undoStack.clear();
  _redoStack.clear();
  Q_EMIT topUndoItemChanged(false, {});
  Q_EMIT topRedoItemChanged(false, {});
}

auto command_handler::setEditMode(bool mode) -> void
{
  _editMode = mode;
  clearStack();
}

auto command_handler::blockRecording() -> void
{
  _blocked = true;
}

auto command_handler::unblockRecording() -> void
{
  _blocked = false;
}

auto command_handler::isRecordingBlocked() const -> bool
{
  return _blocked;
}

NoUndoRecording::NoUndoRecording()
{
  if (internalCommandHandler) {
    if (internalCommandHandler->isRecordingBlocked()) {
      _hasBlock = false;
    } else {
      internalCommandHandler->blockRecording();
    }
  }
}

NoUndoRecording::~NoUndoRecording()
{
  if (_hasBlock && internalCommandHandler) {
    internalCommandHandler->unblockRecording();
  }
}
} // namespace yoyo::command
