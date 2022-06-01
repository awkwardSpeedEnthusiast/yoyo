#pragma once

#include <QObject>
#include <QPoint>

#include <chrono>
#include <memory>

class QWidget;
class QMenu;
class QThread;

namespace yoyo::gui::test
{
///
/// \brief Interactions for handling a single dialog
///
/// This class holds a list of interactions to be performed when handling a single dialog.
/// If there were no actions defined, it will simply accept the dialog.
///
/// Assuming, that the dialog is not created, when the object is defined, it uses a class-name to
/// search in a parent widget's child list for the dialog to operate. Interacting with the
/// individual controls of the dialog is done similarly, but not using class-name, but object names.
///
/// Most important is the interact method, which finally acquires the dialog and applies all defined
/// actions to it.
///
/// Use with value semantics, since all necessary operators are defined.
///
class single_dialog_interaction
{
public:
  ///
  /// \brief constructor
  ///
  /// Creates a handler for a single dialog.
  /// \param dialogTypeName
  ///
  single_dialog_interaction(std::string const& dialogTypeName);
  ~single_dialog_interaction();
  single_dialog_interaction(single_dialog_interaction const&);
  single_dialog_interaction(single_dialog_interaction&&) = default;
  single_dialog_interaction& operator=(single_dialog_interaction const&);
  single_dialog_interaction& operator=(single_dialog_interaction&&) = default;

  ///
  /// \{
  /// \name Setters

  ///
  /// \brief add a mouse click
  ///
  /// Adds a mouse click interaction to the handler.
  /// \param childname the object name of the dialog's child, which shall receive the mouse click,
  /// \param childCoordinate the point in the child's coordinate system, where the click shall be
  /// performed, \param timeout maximum amount of time to wait for the child to become available.
  ///
  auto addMouseClick(QString const& childname, QPoint const& childCoordinate,
                     std::chrono::milliseconds timeout) -> single_dialog_interaction&;

  ///
  /// \brief add a mouse double-click
  ///
  /// Add a mouse double-click interaction to the handler.
  /// \param childname the object name of the dialog's child, which shall receive the mouse click,
  /// \param childCoordinate the point in the child's coordinate system, where the click shall be
  /// performed, \param timeout maximum amount of time to wait for the child to become available.
  ///
  auto addMouseDClick(QString const& childname, QPoint const& childCoordinate,
                      std::chrono::milliseconds timeout) -> single_dialog_interaction&;

  ///
  /// \brief add a key click
  ///
  /// Add a key click interaction to the handler.
  /// \param childname the object name of the dialog's child, which shall receive the key click,
  /// \param key the key to press,
  /// \param timeout maximum amount of time to wait for the child to become available.
  ///
  auto addKeyClick(QString const& childname, Qt::Key key, std::chrono::milliseconds timeout)
    -> single_dialog_interaction&;

  ///
  /// \brief add key clicks
  ///
  /// Add a series of key click interactions to the handler.
  /// \param childname the object name of the dialog's child, which shall receive the key clicks,
  /// \param keys the keys to press,
  /// \param timeout maximum amount of time to wait for the child to become available.
  ///
  auto addKeyClicks(QString const& childname, std::vector<Qt::Key> keys,
                    std::chrono::milliseconds timeout) -> single_dialog_interaction&;

  ///\}

  ///
  /// \brief start the interaction
  ///
  /// This method performs the defined actions on the specified dialog:
  /// - It will search in the list of children of the provided parent for a dialog of the specified
  ///     type.
  /// - For each previously defined action, the action will be applied to the found dialog:
  ///   - The child to act on is searched by object name,
  ///   - The action is applied to the found child widget.
  /// - If no action has been specified, accept will be called on the dialog.
  /// \param parent the widget, which is the parent of the dialog to handle,
  /// \return true, if all actions could be performed
  ///
  auto interact(QWidget* parent) -> bool;

private:
  struct impl;
  std::unique_ptr<impl> _p;
};

///
/// \brief a helper class for operating dialogs in tests
///
/// This class will help a developer to create unit-tests verifying behavior involving dialogs.
/// This needs to be done asynchronously: the main thread operates the triggering of the dialogs
/// by sending the corresponding event to the widget in question. The second thread - which can
/// be provided from extern - will operate the dialogs themselves.
///
/// This operation provides a wide range of configuration possibilities, which can be used to adapt
/// the instance to the system under test.
///
class dialog_operator
{
public:
  ///
  /// \brief constructor
  /// \param w the widget to post the event on, which will trigger the dialog.
  ///
  explicit dialog_operator(QWidget* w);
  ~dialog_operator();

  ///
  /// \{
  /// \name setter

  ///
  /// \brief handles a file dialog for opening existing files
  ///
  /// This method adds a handler for an open-file-dialog to the dialog operator, which will select
  /// the provided file.
  ///
  /// Unfortunately there is currently still a bug on windows, preventing this to be usable:
  /// https://bugreports.qt.io/browse/QTBUG-47159
  /// \param filename the file to select in the dialog
  ///
  auto openFileDialog(QString const& filename) -> dialog_operator&;

  ///
  /// \brief handles a generic dialog
  ///
  /// This method adds a generic handler for a self written dialog.
  /// \param dialog the description for the actions to be applied to the dialog.
  ///
  auto addDialog(single_dialog_interaction const& dialog) -> dialog_operator&;

  ///
  /// \brief provide thread
  ///
  /// If you want to re-use worker threads during testing, set them with this method. They will be
  /// started and stopped by operate, so don't use running threads for this purpose.
  /// \note optional
  /// \param t the thread to use.
  ///
  auto useThread(QThread* t) -> dialog_operator&;

  ///
  /// \brief timeout for dialog to be initialized
  ///
  /// Use this method to define a time to wait before the first interaction with the dialog is done.
  /// Default value is 100ms.
  /// \param t timeout before handler is started.
  ///
  auto timeoutForStart(std::chrono::milliseconds t) -> dialog_operator&;

  ///
  /// \}
  ///

  ///
  /// \brief start the dialog interaction
  ///
  /// Call this method to trigger the event, which will cause the (first) dialog to show up. This is
  /// a synchronous call, it will return after interaction finished completely.
  ///
  auto operate(std::function<void()> trigger) -> void;

private:
  struct impl;
  std::unique_ptr<impl> _p;
};
} // namespace yoyo::gui::test
