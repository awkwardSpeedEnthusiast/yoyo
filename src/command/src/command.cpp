#include "command.h"

namespace yoyo::command
{
command::command(QString const& name, Operation toDo, Operation undo, Operation redo)
  : _currentStatus(static_cast<bool>(toDo) ? Status::initial : Status::invalid)
  , _name(name)
  , _toDo(toDo)
  , _undo(undo)
  , _redo(redo)
{
}

auto command::name() const -> QString
{
  return _name;
}

auto command::execute() -> bool
{
  if (_currentStatus != Status::initial) {
    return false;
  }

  _toDo();
  _currentStatus = Status::executed;
  return true;
}

auto command::undo() -> bool
{
  if ((_currentStatus != Status::executed) || !static_cast<bool>(_undo)) {
    return false;
  }

  _undo();
  _currentStatus = Status::undone;
  return true;
}

auto command::redo() -> bool
{
  if (_currentStatus != Status::undone) {
    return false;
  }

  if (static_cast<bool>(_redo)) {
    _redo();
  } else {
    _toDo();
  }

  _currentStatus = Status::executed;
  return true;
}

auto command::isValid() const -> bool
{
  return _currentStatus != Status::invalid;
}

auto command::isExecuted() const -> bool
{
  return _currentStatus == Status::executed;
}

auto command::isUndone() const -> bool
{
  return _currentStatus == Status::undone;
}

auto command::canBeUndone() const -> bool
{
  return static_cast<bool>(_undo);
}
} // namespace yoyo::command
