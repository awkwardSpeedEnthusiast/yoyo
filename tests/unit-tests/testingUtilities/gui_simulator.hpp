#pragma once

#include <QPoint>
#include <qnamespace.h>

#include <chrono>
#include <string>
#include <vector>

class QWidget;

namespace yoyo::gui::test
{
///
/// \brief mousePress
///
/// This function provides a mouse press event. Events are requested to be processed.
/// \param widget the widget to post the event for,
/// \param button the button of the mouse, which is pressed,
/// \param modifier the list of keyboard modifiers pressed, while the press event is created,
/// \param pos the position of the mouse pointer local to the widget, where the event occurred,
/// \param delay if the delay is bigger than 0, the function will return before the events have been
///              processed; the event is scheduled in \a delay milliseconds.
///
auto mousePress(QWidget* widget, Qt::MouseButton button, Qt::KeyboardModifiers modifier = {},
                QPoint pos = {}, std::chrono::milliseconds delay = {}) -> void;

///
/// \brief mouseRelease
///
/// This function provides a mouse release event. Events are requested to be processed.
/// \param widget the widget to post the event for,
/// \param button the button of the mouse, which is now to be released,
/// \param modifier the list of keyboard modifiers pressed, while the release event is created,
/// \param pos the position of the mouse pointer local to the widget, where the event occurred,
/// \param delay if the delay is bigger than 0, the function will return before the events have been
///              processed; the event is scheduled in \a delay milliseconds.
///
auto mouseRelease(QWidget* widget, Qt::MouseButton button, Qt::KeyboardModifiers modifier = {},
                  QPoint pos = {}, std::chrono::milliseconds delay = {}) -> void;

///
/// \brief mouseClick
///
/// This function provides a mouse click event containing of a press and a release event. Events are
/// requested to be processed.
/// \param widget the widget to post the events for,
/// \param button the button of the mouse, which is clicked,
/// \param modifier the list of keyboard modifiers pressed, while the click events are created,
/// \param pos the position of the mouse pointer local to the widget, where the event occurred,
/// \param delay if the delay is bigger than 0, the function will return before the events have been
///              processed; the event is scheduled in \a delay milliseconds.
///
auto mouseClick(QWidget* widget, Qt::MouseButton button, Qt::KeyboardModifiers modifier = {},
                QPoint pos = {}, std::chrono::milliseconds delay = {}) -> void;

///
/// \brief mouseDClick
///
/// This function provides a mouse double click event. Events are requested to be processed.
/// \param widget the widget to post the event for,
/// \param button the button of the mouse, which is double clicked,
/// \param modifier the list of keyboard modifiers pressed, while the press event is created,
/// \param pos the position of the mouse pointer local to the widget, where the event occurred,
/// \param delay if the delay is bigger than 0, the function will return before the events have been
///              processed; the event is scheduled in \a delay milliseconds.
///
auto mouseDClick(QWidget* widget, Qt::MouseButton button, Qt::KeyboardModifiers modifier = {},
                 QPoint pos = {}, std::chrono::milliseconds delay = {}) -> void;

///
/// \brief mouseMove
///
/// This function provides a mouse move event inside the provided widget. Events are requested to be
/// processed
/// \param widget the widget to post the event for,
/// \param pos the position of the mouse pointer for the move event,
/// \param delay if the delay is bigger than 0, the function will return before the events have been
///              processed; the event is scheduled in \a delay milliseconds.
///
auto mouseMove(QWidget* widget, QPoint pos = {}, std::chrono::milliseconds delay = {}) -> void;

///
/// \brief keyClick
///
/// This function provides a keyboard event, where a single key is pressed and released again.
/// Events are requested to be processed. \param widget the widget to post the event for, \param key
/// the key, which is clicked, \param modifier the list of keyboard modifiers pressed, while the
/// click event is created, \param delay if the delay is bigger than 0, the function will return
/// before the events have been
///              processed; the event is scheduled in \a delay milliseconds.
///
auto keyClick(QWidget* widget, Qt::Key key, Qt::KeyboardModifiers modifier = {},
              std::chrono::milliseconds delay = {}) -> void;

///
/// \brief keyClicks
///
/// This function provides a series of keyboard events, where a single key is pressed and released
/// again. Events are requested to be processed.
/// \param widget the widget to post the event for,
/// \param key the list of keys, which are clicked,
/// \param modifier the list of keyboard modifiers pressed, while the click events are created,
/// \param delay if the delay is bigger than 0, the function will return before the events have been
///              processed; the event is scheduled in \a delay milliseconds.
///
auto keyClicks(QWidget* widget, std::vector<Qt::Key> key, Qt::KeyboardModifiers modifier = {},
               std::chrono::milliseconds delay = {}) -> void;

///
/// \brief keyClicks
///
/// This function provides a series of keyboard events, where a single key is pressed and released
/// again. Events are requested to be processed.
/// \param widget the widget to post the event for,
/// \param key the list of keys, which are clicked (in this version as string),
/// \param modifier the list of keyboard modifiers pressed, while the click events are created,
/// \param delay if the delay is bigger than 0, the function will return before the events have been
///              processed; the event is scheduled in \a delay milliseconds.
///
auto keyClicks(QWidget* widget, std::string keys, Qt::KeyboardModifiers modifier = {},
               std::chrono::milliseconds delay = {}) -> void;

///
/// \brief keyPress
///
/// This function provides a keyboard event, where a single key is pressed. Events are requested to
/// be processed.
/// \param widget the widget to post the event for,
/// \param key the key, which is pressed,
/// \param modifier the list of keyboard modifiers pressed, while the press event is created,
/// \param delay if the delay is bigger than 0, the function will return before the events have been
///              processed; the event is scheduled in \a delay milliseconds.
///
auto keyPress(QWidget* widget, Qt::Key key, Qt::KeyboardModifiers modifier = {},
              std::chrono::milliseconds delay = {}) -> void;

///
/// \brief keyRelease
///
/// This function provides a keyboard event, where a single key is released. Events are requested to
/// be processed.
/// \param widget the widget to post the event for,
/// \param key the key, which is released,
/// \param modifier the list of keyboard modifiers pressed, while the release event is created,
/// \param delay if the delay is bigger than 0, the function will return before the events have been
///              processed; the event is scheduled in \a delay milliseconds.
///
auto keyRelease(QWidget* widget, Qt::Key key, Qt::KeyboardModifiers modifier = {},
                std::chrono::milliseconds delay = {}) -> void;
} // namespace yoyo::gui::test
