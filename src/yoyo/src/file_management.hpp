#pragma once

#include <QObject>

#include <boost/uuid/uuid.hpp>

#include <memory>
#include <tuple>
#include <vector>

namespace yoyo
{
class node_factory;
class node_base;

class file_management : public QObject
{
  Q_OBJECT
public:
  file_management(std::vector<std::shared_ptr<node_factory>> factories);
  ~file_management();

  auto new_file() -> std::shared_ptr<node_base>;
  auto save_file(std::shared_ptr<node_base> root, QString const& filename, boost::uuids::uuid io_id)
    -> void;
  auto save_file_as(std::shared_ptr<node_base> root) -> std::tuple<QString, boost::uuids::uuid>;
  auto open_file() -> std::tuple<QString, std::shared_ptr<node_base>, boost::uuids::uuid>;
  auto open_file(QString const& filename)
    -> std::tuple<std::shared_ptr<node_base>, boost::uuids::uuid>;

  auto connectNotify(QMetaMethod const& signal) -> void override;
Q_SIGNALS:
  void historyChanged(std::vector<QString> history);

private:
  class impl;
  std::unique_ptr<impl> _p;
};
} // namespace yoyo
