#include "gui_simulator.hpp"

#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QWidget>

namespace yoyo::gui::test
{
auto mousePress(QWidget* widget, Qt::MouseButton button, Qt::KeyboardModifiers modifier, QPoint pos,
                std::chrono::milliseconds delay) -> void
{
  auto event = [widget, pos, button, modifier] {
    QApplication::postEvent(widget,
                            new QMouseEvent(QEvent::MouseButtonPress, pos, widget->mapToGlobal(pos),
                                            button, { button }, modifier));
    QApplication::processEvents();
  };

  if (delay.count() > 0) {
    QTimer::singleShot(delay.count(), event);
  } else {
    event();
  }
}

auto mouseRelease(QWidget* widget, Qt::MouseButton button, Qt::KeyboardModifiers modifier,
                  QPoint pos, std::chrono::milliseconds delay) -> void
{
  auto event = [widget, pos, button, modifier] {
    QApplication::postEvent(widget, new QMouseEvent(QEvent::MouseButtonRelease, pos,
                                                    widget->mapToGlobal(pos), button, { button },
                                                    modifier));
    QApplication::processEvents();
  };

  if (delay.count() > 0) {
    QTimer::singleShot(delay, event);
  } else {
    event();
  }
}

auto mouseClick(QWidget* widget, Qt::MouseButton button, Qt::KeyboardModifiers modifier, QPoint pos,
                std::chrono::milliseconds delay) -> void
{
  auto event = [widget, pos, button, modifier] {
    QApplication::postEvent(widget,
                            new QMouseEvent(QEvent::MouseButtonPress, pos, widget->mapToGlobal(pos),
                                            button, { button }, modifier));
    QApplication::processEvents();
    QApplication::postEvent(widget, new QMouseEvent(QEvent::MouseButtonRelease, pos,
                                                    widget->mapToGlobal(pos), button, { button },
                                                    modifier));
    QApplication::processEvents();
  };

  if (delay.count() > 0) {
    QTimer::singleShot(delay, event);
  } else {
    event();
  }
}

auto mouseDClick(QWidget* widget, Qt::MouseButton button, Qt::KeyboardModifiers modifier,
                 QPoint pos, std::chrono::milliseconds delay) -> void
{
  auto event = [widget, pos, button, modifier] {
    QApplication::postEvent(widget, new QMouseEvent(QEvent::MouseButtonDblClick, pos,
                                                    widget->mapToGlobal(pos), button, { button },
                                                    modifier));
    QApplication::processEvents();
  };

  if (delay.count() > 0) {
    QTimer::singleShot(delay, event);
  } else {
    event();
  }
}

auto mouseMove(QWidget* widget, QPoint pos, std::chrono::milliseconds delay) -> void
{
  auto event = [widget, pos] {
    QApplication::postEvent(
      widget, new QMouseEvent(QEvent::MouseMove, pos, widget->mapToGlobal(pos), {}, {}, {}));
    QApplication::processEvents();
  };

  if (delay.count() > 0) {
    QTimer::singleShot(delay, event);
  } else {
    event();
  }
}

auto keyClick(QWidget* widget, Qt::Key key, Qt::KeyboardModifiers modifier,
              std::chrono::milliseconds delay) -> void
{
  auto event = [widget, key, modifier] {
    QApplication::postEvent(widget, new QKeyEvent(QEvent::KeyPress, key, modifier));
    QApplication::processEvents();
    QApplication::postEvent(widget, new QKeyEvent(QEvent::KeyRelease, key, modifier));
    QApplication::processEvents();
  };

  if (delay.count() > 0) {
    QTimer::singleShot(delay, event);
  } else {
    event();
  }
}

auto keyClicks(QWidget* widget, std::vector<Qt::Key> key, Qt::KeyboardModifiers modifier,
               std::chrono::milliseconds delay) -> void
{
  auto event = [widget, key, modifier] {
    for (auto k : key) {
      QApplication::postEvent(widget, new QKeyEvent(QEvent::KeyPress, k, modifier));
      QApplication::processEvents();
      QApplication::postEvent(widget, new QKeyEvent(QEvent::KeyRelease, k, modifier));
      QApplication::processEvents();
    }
  };

  if (delay.count() > 0) {
    QTimer::singleShot(delay, event);
  } else {
    event();
  }
}

auto keyClicks(QWidget* widget, std::string keys, Qt::KeyboardModifiers modifier,
               std::chrono::milliseconds delay) -> void
{
  auto event = [widget, keys, modifier] {
    auto c = QString::fromStdString(keys);
    for (auto k : c) {
      QApplication::postEvent(widget, new QKeyEvent(QEvent::KeyPress, k.toLatin1(), modifier));
      QApplication::processEvents();
      QApplication::postEvent(widget, new QKeyEvent(QEvent::KeyRelease, k.toLatin1(), modifier));
      QApplication::processEvents();
    }
  };

  if (delay.count() > 0) {
    QTimer::singleShot(delay, event);
  } else {
    event();
  }
}

auto keyPress(QWidget* widget, Qt::Key key, Qt::KeyboardModifiers modifier,
              std::chrono::milliseconds delay) -> void
{
  auto event = [widget, key, modifier] {
    QApplication::postEvent(widget, new QKeyEvent(QEvent::KeyPress, key, modifier));
    QApplication::processEvents();
  };

  if (delay.count() > 0) {
    QTimer::singleShot(delay, event);
  } else {
    event();
  }
}

auto keyRelease(QWidget* widget, Qt::Key key, Qt::KeyboardModifiers modifier,
                std::chrono::milliseconds delay) -> void
{
  auto event = [widget, key, modifier] {
    QApplication::postEvent(widget, new QKeyEvent(QEvent::KeyRelease, key, modifier));
    QApplication::processEvents();
  };

  if (delay.count() > 0) {
    QTimer::singleShot(delay, event);
  } else {
    event();
  }
}
} // namespace yoyo::gui::test
