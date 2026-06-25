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

command::command(command const& other)
  : _currentStatus { other._currentStatus }
  , _name { other._name }
  , _toDo { other._toDo }
  , _undo { other._undo }
  , _redo { other._redo }
{
}

command::command(command&& other)
  : _currentStatus { std::move(other._currentStatus) }
  , _name { std::move(other._name) }
  , _toDo { std::move(other._toDo) }
  , _undo { std::move(other._undo) }
  , _redo { std::move(other._redo) }
{
}

command& command::operator=(command const& other)
{
  _currentStatus = other._currentStatus;
  _name = other._name;
  _toDo = other._toDo;
  _undo = other._undo;
  _redo = other._redo;
  return *this;
}

command& command::operator=(command&& other)
{
  std::swap(_currentStatus, other._currentStatus);
  std::swap(_name, other._name);
  std::swap(_toDo, other._toDo);
  std::swap(_undo, other._undo);
  std::swap(_redo, other._redo);
  return *this;
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
