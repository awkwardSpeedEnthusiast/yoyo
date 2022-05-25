#pragma once
#include "yoyo/node_base.h"

class QMimeData;

namespace yoyo
{
namespace gui
{
class data_adapter
{
public:
  virtual auto data(int column) const -> QVariant = 0;
  virtual auto mimedata() const -> QMimeData* = 0;
  virtual auto deserialize(QByteArray data, std::shared_ptr<node_base> root) const
    -> std::shared_ptr<node_base> = 0;

protected:
  data_adapter(node_base* node);

protected:
  node_base* _node;
};

auto create_data_adapter(node_base* node) -> std::shared_ptr<data_adapter>;
} // namespace gui
} // namespace yoyo
