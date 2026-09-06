#pragma once
#include "io_export.h"

#include <QJsonValue>
#include <QString>
#include <QVariant>

#include <functional>
#include <map>

namespace yoyo::io::json
{
struct YOYO_IO_SHARED_EXPORT property {
  using serialize_f = std::function<QJsonValue(QVariant)>;
  using deserialize_f = std::function<QVariant(QJsonValue)>;

  static std::map<QString, std::pair<serialize_f, deserialize_f>> const io;
};
} // namespace yoyo::io::json
