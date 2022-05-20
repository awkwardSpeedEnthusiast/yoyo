#pragma once
#include "command_export.h"

#include <QString>

#include <functional>

namespace yoyo
{
namespace command
{
///
/// \brief represents a single undo-able command
/// This class defines a single undo-able operation. In contrast to the usual
/// implementation we here refrain from using an inheritance hierarchy and use
/// dependency injection to gain more flexibility.
///
class YOYO_COMMAND_SHARED_EXPORT command
{
public:
  ///
  /// Defines the type for an atomic operation, packaged into a callable object.
  ///
  using Operation = std::function<void()>;

public:
  ///
  /// \brief Constructor
  ///
  /// Creates a command object.
  /// The created command will be treated as valid, if at least the \a toDo parameter
  /// is not default initialized.
  /// \param name the name of the command to display in the Gui
  /// \param toDo the callable object to use for the initial execution,
  /// \param undo the callable object to use for undo,
  /// \param redo the callable object to use for redo; if left empty, \a toDo is used.
  ///
  command(QString const& name, Operation toDo, Operation undo, Operation redo = Operation {});
  ~command() = default;
  command(command const&) = default;
  command(command&&) = default;
  command& operator=(command const&) = default;
  command& operator=(command&&) = default;

  ///
  /// \brief name
  ///
  /// \return the name of the command.
  ///
  auto name() const -> QString;

  ///
  /// \brief execute the command
  ///
  /// The command can only be executed, if it is valid and has not yet been executed.
  /// The return value doesn't reflect whether the executed operation was successful.
  /// \return true if it could be executed.
  ///
  auto execute() -> bool;

  ///
  /// \brief executes the undo operation
  ///
  /// The command can only be undone, if it had been executed and, if the undo operation
  /// is set (\see canBeUndone).
  /// The return value doesn't reflect whether the undo operation was successful.
  /// \return true if the command could be undone.
  ///
  auto undo() -> bool;

  ///
  /// \brief executes the redo operation
  ///
  /// The command can only be redone, if it had been undone.
  /// If the redo operation was not set, the do-operation will be used instead.
  /// The return value doesn't reflect whether the redo operation was successful.
  /// \return true if the command could be undone.
  ///
  auto redo() -> bool;

  ///
  /// \brief check validity of command
  ///
  /// \return true if there is at least a do operation available.
  ///
  auto isValid() const -> bool;

  ///
  /// \brief check if command has been executed
  /// \return true if the command had been executed.
  ///
  auto isExecuted() const -> bool;

  ///
  /// \brief check if command has been undone
  /// \return true if the command had been undone.
  ///
  auto isUndone() const -> bool;

  ///
  /// \brief check if command can be undone
  /// \return true if there is an undo operation defined.
  ///
  auto canBeUndone() const -> bool;

private:
  enum class Status { invalid, initial, executed, undone };
  Status _currentStatus { Status::invalid };

  QString const _name;
  Operation _toDo;
  Operation _undo;
  Operation _redo;
};
} // namespace command
} // namespace yoyo
