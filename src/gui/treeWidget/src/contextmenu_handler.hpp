#pragma once

#include <QObject>
#include <QPoint>

#include <boost/uuid/uuid.hpp>

#include <memory>
#include <vector>

class QWidget;
class QMenu;
namespace yoyo
{
class node_base;
class node_factory;
namespace gui
{
class contextmenu_handler : public QObject
{
  Q_OBJECT
public:
  contextmenu_handler(std::vector<std::shared_ptr<node_factory>> factories, QWidget* parent);
  ~contextmenu_handler();

  auto update() -> void;

  auto execute(std::shared_ptr<node_base> current, QPoint p) -> void;

  auto defaultForDataRequested(QString path, std::shared_ptr<node_base> parent, int index,
                               bool connectDefaultInput, bool connectDefaultOutput,
                               bool connectDefaultAutoConnect) -> void;
  auto exchangeRequested(std::shared_ptr<node_base> nodeToExchange, boost::uuids::uuid nodeType)
    -> void;
  auto addRequested(std::shared_ptr<node_base> parentNode, boost::uuids::uuid nodeType, int index)
    -> void;

private:
  std::vector<std::shared_ptr<node_factory>> _factories;
  std::unique_ptr<QMenu> _menu;
};
} // namespace gui
} // namespace yoyo
