#pragma once

#include <QObject>
#include <QPoint>

#include <chrono>
#include <memory>

class QWidget;
class QMenu;
class QThread;

namespace yoyo
{
namespace gui
{
namespace test
{
///
/// \brief a helper class for operating context menus in tests
///
/// This class will help a developer to create unit-tests verifying context menu behavior.
/// This needs to be done asynchronously: the main thread operates the triggering of the context
/// menu by sending the corresponding event to the widget in question. The second thread - which can
/// be provided from extern - will operate the context menu itself.
///
/// This operation provides a wide range of configuration possibilities, which can be used to adapt
/// the instance to the system under test.
/// As main feature it allows for navigation in sub-menus.
///
/// Only restriction: the menu to navigate on must be a child of the widget already before the
/// context menu is triggered. Use the appropriate setter.
///
class contextmenu_operator
{
public:
  ///
  /// \brief constructor
  /// \param w the widget to post the context menu event on.
  ///
  explicit contextmenu_operator(QWidget* w);
  ~contextmenu_operator();

  ///
  /// \{
  /// \name setter

  ///
  /// \brief point of the event
  ///
  /// This point will be used both to determine, where the context menu will be visible, as well
  /// as which child widget might be involved.
  /// \param p point for event.
  ///
  auto point(QPoint const& p) -> contextmenu_operator&;

  ///
  /// \brief set the menu
  ///
  /// Sets the menu to operate on.
  /// \warning if this is not set, operate will throw an exception.
  /// \param m the menu to operate on.
  ///
  auto menu(QMenu* m) -> contextmenu_operator&;

  ///
  /// \brief keyboard modifiers for event
  ///
  /// \note optional
  /// \param modifier keyboard modifiers present for the context menu event.
  ///
  auto modifiers(Qt::KeyboardModifiers modifier) -> contextmenu_operator&;

  ///
  /// \brief global point of event
  ///
  /// This is the on point for the event.
  /// \note optional
  /// \param p the global event point
  ///
  auto globalPoint(QPoint const& p) -> contextmenu_operator&;

  ///
  /// \brief provide thread
  ///
  /// If you want to re-use worker threads during testing, set them with this method. They will be
  /// started and stopped by operate, so don't use running threads for this purpose.
  /// \note optional
  /// \param t the thread to use.
  ///
  auto useThread(QThread* t) -> contextmenu_operator&;

  ///
  /// \brief add menu interaction
  ///
  /// Use this method to specify what to do with the context menu. Each invokation adds an
  /// interaction step, like navigating to a sub-menu or triggering an action.
  ///
  /// \warning This method needs to be called at least once. If no interaction is defined, when
  ///     operate is called, an exception is thrown.
  /// \param wait_before_action timeout to wait before the interaction with the menu,
  /// \param menu_item_to_be_selected the item in the menu to be selected,
  /// \param minimum_children_next_menu for sub-menu selection: this is the minimum number of
  ///     children the sub-menu needs to have before it is proceeded, giving the menu time to
  ///     build-up.
  /// \param wait_after_action timeout to wait after the interaction with the menu.
  ///
  auto addMenuInteraction(std::chrono::milliseconds wait_before_action,
                          size_t menu_item_to_be_selected, size_t minimum_children_next_menu,
                          std::chrono::milliseconds wait_after_action) -> contextmenu_operator&;

  ///
  /// \brief timeout for menu to be initialized
  ///
  /// Use this method to define a time to wait before the first interaction with the menu is done.
  /// Default value is 100ms.
  /// \param t timeout before handler is started.
  ///
  auto timeoutForMenu(std::chrono::milliseconds t) -> contextmenu_operator&;

  ///
  /// \}
  ///

  ///
  /// \brief operate the context menu
  ///
  /// Call this method to operate the context menu. This is a synchronous call, it will return
  /// after interaction finished completely.
  ///
  auto operate() -> void;

private:
  class impl;
  std::unique_ptr<impl> _p;
};
} // namespace test
} // namespace gui
} // namespace yoyo
