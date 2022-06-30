#include "json/json_property.hpp"
#include "json_constants.hpp"

#include "yoyo/property_types.h"

#include <QJsonArray>
#include <QJsonObject>

#include <iostream>

namespace yoyo::io::json
{
template <typename T>
auto readLimited(QJsonValue const& v) -> QVariant
{
  if (!v.isObject()) {
    return {};
  }

  auto o = v.toObject();

  if (!(o.contains(yoyo::io::json::key_minimum) && o.contains(yoyo::io::json::key_maximum)
        && o.contains(yoyo::io::json::key_value))) {
    return {};
  }

  return QVariant::fromValue(yoyo::properties::limited_value_t<T> {
    static_cast<T>(o[yoyo::io::json::key_value].toInt()),
    static_cast<T>(o[yoyo::io::json::key_minimum].toInt()),
    static_cast<T>(o[yoyo::io::json::key_maximum].toInt()) });
}

template <typename T>
auto writeLimited(QVariant const& v) -> QJsonValue
{
  auto val = v.value<T>();
  QJsonObject object;
  object.insert(yoyo::io::json::key_minimum, val._min);
  object.insert(yoyo::io::json::key_maximum, val._max);
  object.insert(yoyo::io::json::key_value, val._value);
  return object;
}

std::map<QString, std::pair<property::serialize_f, property::deserialize_f>> const property::io {
  { "QString", std::make_pair([](QVariant v) -> QJsonValue { return v.toString(); },
                              [](QJsonValue v) -> QVariant { return v.toString(); }) },
  { "bool", std::make_pair([](QVariant v) -> QJsonValue { return v.toBool(); },
                           [](QJsonValue v) -> QVariant { return v.toBool(); }) },
  { "char", std::make_pair([](QVariant v) -> QJsonValue { return v.toInt(); },
                           [](QJsonValue v) -> QVariant {
                             return QVariant::fromValue(static_cast<char>(v.toInt()));
                           }) },
  { "int", std::make_pair([](QVariant v) -> QJsonValue { return v.toInt(); },
                          [](QJsonValue v) -> QVariant { return v.toInt(); }) },
  { "long", std::make_pair([](QVariant v) -> QJsonValue { return v.toInt(); },
                           [](QJsonValue v) -> QVariant {
                             return QVariant::fromValue(static_cast<long>(v.toInt()));
                           }) },
  { "uint8_t", std::make_pair([](QVariant v) -> QJsonValue { return v.toInt(); },
                              [](QJsonValue v) -> QVariant {
                                return QVariant::fromValue(static_cast<uint8_t>(v.toInt()));
                              }) },
  { "uchar", std::make_pair([](QVariant v) -> QJsonValue { return v.toInt(); },
                            [](QJsonValue v) -> QVariant {
                              return QVariant::fromValue(static_cast<uint8_t>(v.toInt()));
                            }) },
  { "uint16_t", std::make_pair([](QVariant v) -> QJsonValue { return v.toInt(); },
                               [](QJsonValue v) -> QVariant {
                                 return QVariant::fromValue(static_cast<uint16_t>(v.toInt()));
                               }) },
  { "ushort", std::make_pair([](QVariant v) -> QJsonValue { return v.toInt(); },
                             [](QJsonValue v) -> QVariant {
                               return QVariant::fromValue(static_cast<uint16_t>(v.toInt()));
                             }) },
  { "uint32_t", std::make_pair(
                  [](QVariant v) -> QJsonValue {
                    return QString::fromStdString(std::to_string(v.value<uint32_t>()));
                  },
                  [](QJsonValue v) -> QVariant {
                    return QVariant::fromValue(static_cast<uint32_t>(
                      std::strtoul(v.toString().toStdString().data(), nullptr, 10)));
                  }) },
  { "uint", std::make_pair(
              [](QVariant v) -> QJsonValue {
                return QString::fromStdString(std::to_string(v.value<uint32_t>()));
              },
              [](QJsonValue v) -> QVariant {
                return QVariant::fromValue(static_cast<uint32_t>(
                  std::strtoul(v.toString().toStdString().data(), nullptr, 10)));
              }) },
  { "uint64_t", std::make_pair(
                  [](QVariant v) -> QJsonValue {
                    return QString::fromStdString(std::to_string(v.value<uint64_t>()));
                  },
                  [](QJsonValue v) -> QVariant {
                    return QVariant::fromValue(static_cast<uint64_t>(
                      std::strtoull(v.toString().toStdString().data(), nullptr, 10)));
                  }) },
  { "qulonglong", std::make_pair(
                    [](QVariant v) -> QJsonValue {
                      return QString::fromStdString(std::to_string(v.value<uint64_t>()));
                    },
                    [](QJsonValue v) -> QVariant {
                      return QVariant::fromValue(static_cast<uint64_t>(
                        std::strtoull(v.toString().toStdString().data(), nullptr, 10)));
                    }) },
  { "int8_t", std::make_pair([](QVariant v) -> QJsonValue { return v.toInt(); },
                             [](QJsonValue v) -> QVariant {
                               return QVariant::fromValue(static_cast<int8_t>(v.toInt()));
                             }) },
  { "signed char", std::make_pair([](QVariant v) -> QJsonValue { return v.toInt(); },
                                  [](QJsonValue v) -> QVariant {
                                    return QVariant::fromValue(static_cast<int8_t>(v.toInt()));
                                  }) },
  { "int16_t", std::make_pair([](QVariant v) -> QJsonValue { return v.toInt(); },
                              [](QJsonValue v) -> QVariant {
                                return QVariant::fromValue(static_cast<int16_t>(v.toInt()));
                              }) },
  { "short", std::make_pair([](QVariant v) -> QJsonValue { return v.toInt(); },
                            [](QJsonValue v) -> QVariant {
                              return QVariant::fromValue(static_cast<int16_t>(v.toInt()));
                            }) },
  { "int32_t", std::make_pair([](QVariant v) -> QJsonValue { return v.toInt(); },
                              [](QJsonValue v) -> QVariant {
                                return QVariant::fromValue(static_cast<int32_t>(v.toInt()));
                              }) },
  { "int64_t", std::make_pair(
                 [](QVariant v) -> QJsonValue {
                   return QString::fromStdString(std::to_string(v.value<int64_t>()));
                 },
                 [](QJsonValue v) -> QVariant {
                   return QVariant::fromValue(static_cast<int64_t>(
                     std::strtoll(v.toString().toStdString().data(), nullptr, 10)));
                 }) },
  { "qlonglong", std::make_pair(
                   [](QVariant v) -> QJsonValue {
                     return QString::fromStdString(std::to_string(v.value<int64_t>()));
                   },
                   [](QJsonValue v) -> QVariant {
                     return QVariant::fromValue(static_cast<int64_t>(
                       std::strtoll(v.toString().toStdString().data(), nullptr, 10)));
                   }) },
  { "float", std::make_pair([](QVariant v) -> QJsonValue { return v.toFloat(); },
                            [](QJsonValue v) -> QVariant {
                              return QVariant::fromValue(static_cast<float>(v.toDouble()));
                            }) },
  { "double", std::make_pair([](QVariant v) -> QJsonValue { return v.toDouble(); },
                             [](QJsonValue v) -> QVariant { return v.toDouble(); }) },
  { "yoyo::types::value_t", std::make_pair(
                              [](QVariant v) -> QJsonValue {
                                return static_cast<uint8_t>(v.value<yoyo::types::value_t>());
                              },
                              [](QJsonValue v) -> QVariant {
                                return QVariant::fromValue(
                                  static_cast<yoyo::types::value_t>(v.toInt()));
                              }) },
  { "yoyo::properties::integer_format_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        return static_cast<uint8_t>(v.value<yoyo::properties::integer_format_t>());
      },
      [](QJsonValue v) -> QVariant {
        return QVariant::fromValue(static_cast<yoyo::properties::integer_format_t>(v.toInt()));
      }) },
  { "yoyo::types::access_t", std::make_pair(
                               [](QVariant v) -> QJsonValue {
                                 return static_cast<uint8_t>(v.value<yoyo::types::access_t>());
                               },
                               [](QJsonValue v) -> QVariant {
                                 return QVariant::fromValue(
                                   static_cast<yoyo::types::access_t>(v.toInt()));
                               }) },
  { "yoyo::types::layout_direction_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        return static_cast<uint8_t>(v.value<yoyo::types::layout_direction_t>());
      },
      [](QJsonValue v) -> QVariant {
        return QVariant::fromValue(static_cast<yoyo::types::layout_direction_t>(v.toInt()));
      }) },
  { "yoyo::properties::string_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue { return v.toString(); },
      [](QJsonValue v) -> QVariant { return yoyo::properties::string_t(v.toString()); }) },
  { "yoyo::properties::text_t",
    std::make_pair([](QVariant v) -> QJsonValue { return v.value<yoyo::properties::text_t>()._s; },
                   [](QJsonValue v) -> QVariant {
                     return QVariant::fromValue(yoyo::properties::text_t { v.toString() });
                   }) },
  { "yoyo::properties::invisible_string_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        auto val = v.value<yoyo::properties::invisible_string_t>();
        QJsonObject object;
        object.insert(yoyo::io::json::key_visible, val._visible);
        object.insert(yoyo::io::json::key_value, val._s);
        return object;
      },
      [](QJsonValue v) -> QVariant {
        if (!v.isObject()) {
          return {};
        }

        auto o = v.toObject();

        if (!(o.contains(yoyo::io::json::key_visible) && o.contains(yoyo::io::json::key_value))) {
          return {};
        }

        return QVariant::fromValue(yoyo::properties::invisible_string_t {
          o[yoyo::io::json::key_value].toString(), o[yoyo::io::json::key_visible].toBool() });
      }) },
  { "yoyo::properties::invisible_layout_direction_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        auto val = v.value<yoyo::properties::invisible_layout_direction_t>();
        QJsonObject object;
        object.insert(yoyo::io::json::key_visible, val._visible);
        object.insert(yoyo::io::json::key_value, static_cast<uint8_t>(val._s));
        return object;
      },
      [](QJsonValue v) -> QVariant {
        if (!v.isObject()) {
          return {};
        }

        auto o = v.toObject();

        if (!(o.contains(yoyo::io::json::key_visible) && o.contains(yoyo::io::json::key_value))) {
          return {};
        }

        return QVariant::fromValue(yoyo::properties::invisible_layout_direction_t {
          static_cast<yoyo::types::layout_direction_t>(o[yoyo::io::json::key_value].toInt()),
          o[yoyo::io::json::key_visible].toBool() });
      }) },
  { "yoyo::properties::connected_string_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        return v.value<yoyo::properties::connected_string_t>()._own_value;
      },
      [](QJsonValue v) -> QVariant {
        return QVariant::fromValue(yoyo::properties::connected_string_t { {}, v.toString() });
      }) },
  { "yoyo::properties::connected_boolean_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        return static_cast<uint8_t>(v.value<yoyo::properties::connected_boolean_t>()._type);
      },
      [](QJsonValue v) -> QVariant {
        return QVariant::fromValue(yoyo::properties::connected_boolean_t {
          static_cast<yoyo::properties::connected_boolean_t::boolean_t>(v.toInt()), false });
      }) },
  { "yoyo::properties::connection_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        auto val = v.value<yoyo::properties::connection_t>();
        QJsonObject object;
        object.insert(yoyo::io::json::key_in, val._in);
        object.insert(yoyo::io::json::key_out, val._out);
        object.insert(yoyo::io::json::key_auto, val._auto);
        return object;
      },
      [](QJsonValue v) -> QVariant {
        if (!v.isObject()) {
          return {};
        }

        auto o = v.toObject();

        if (!(o.contains(yoyo::io::json::key_in) && o.contains(yoyo::io::json::key_out)
              && o.contains(yoyo::io::json::key_auto))) {
          return {};
        }

        return QVariant::fromValue(
          yoyo::properties::connection_t { {},
                                           {},
                                           {},
                                           o[yoyo::io::json::key_in].toString(),
                                           o[yoyo::io::json::key_out].toString(),
                                           o[yoyo::io::json::key_auto].toString() });
      }) },
  { "yoyo::properties::in_connection_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        auto val = v.value<yoyo::properties::in_connection_t>();
        QJsonObject object;
        object.insert(yoyo::io::json::key_in, val._in);
        return object;
      },
      [](QJsonValue v) -> QVariant {
        if (!v.isObject()) {
          return {};
        }

        auto o = v.toObject();

        if (!(o.contains(yoyo::io::json::key_in))) {
          return {};
        }

        return QVariant::fromValue(
          yoyo::properties::in_connection_t { {}, {}, o[yoyo::io::json::key_in].toString() });
      }) },
  { "yoyo::properties::out_connection_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        auto val = v.value<yoyo::properties::out_connection_t>();
        QJsonObject object;
        object.insert(yoyo::io::json::key_out, val._out);
        return object;
      },
      [](QJsonValue v) -> QVariant {
        if (!v.isObject()) {
          return {};
        }

        auto o = v.toObject();

        if (!(o.contains(yoyo::io::json::key_out))) {
          return {};
        }

        return QVariant::fromValue(
          yoyo::properties::out_connection_t { {}, {}, o[yoyo::io::json::key_out].toString() });
      }) },
  { "yoyo::properties::enum_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        auto val = v.value<yoyo::properties::enum_t>();
        QJsonObject object;
        object.insert(yoyo::io::json::key_enum_active, val._enabled);
        QJsonArray items;
        std::for_each(val._values.begin(), val._values.end(), [&items](auto i) {
          QJsonObject o;
          o.insert(yoyo::io::json::key_enum_data, QJsonValue::fromVariant(QVariant { i.first }));
          o.insert(yoyo::io::json::key_enum_value, std::get<0>(i.second));
          o.insert(yoyo::io::json::key_enum_title, std::get<1>(i.second));
          o.insert(yoyo::io::json::key_enum_match_text, std::get<2>(i.second)._text);
          o.insert(yoyo::io::json::key_enum_match_type,
                   static_cast<int>(std::get<2>(i.second)._script_type));
          items.append(o);
        });
        object.insert(yoyo::io::json::key_enum_items, items);
        return object;
      },
      [](QJsonValue v) -> QVariant {
        if (!v.isObject()) {
          return {};
        }
        auto object = v.toObject();
        if (!object.contains(yoyo::io::json::key_enum_active)
            || !object.contains(yoyo::io::json::key_enum_items)
            || !object[yoyo::io::json::key_enum_items].isArray()) {
          return {};
        }
        QJsonArray items = object[yoyo::io::json::key_enum_items].toArray();
        decltype(yoyo::properties::enum_t::_values) elements;
        for (auto const& item : items) {
          if (!item.isObject()) {
            continue;
          }
          auto o = item.toObject();
          if (!o.contains(yoyo::io::json::key_enum_data)
              || !o.contains(yoyo::io::json::key_enum_title)
              || !o.contains(yoyo::io::json::key_enum_value)
              || !o.contains(yoyo::io::json::key_enum_match_text)
              || !o.contains(yoyo::io::json::key_enum_match_type)) {
            continue;
          }
          elements.insert({ o[yoyo::io::json::key_enum_data].toVariant().toULongLong(),
                            { o[yoyo::io::json::key_enum_value].toString(),
                              o[yoyo::io::json::key_enum_title].toString(),
                              { o[yoyo::io::json::key_enum_match_text].toString(),
                                static_cast<yoyo::properties::script_t::type>(
                                  o[yoyo::io::json::key_enum_match_type].toInt()) } } });
        }
        return QVariant::fromValue(
          yoyo::properties::enum_t { object[yoyo::io::json::key_enum_active].toBool(), elements });
      }) },
  { "yoyo::properties::patterned_string_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue { return v.value<yoyo::properties::patterned_string_t>()._s; },
      [](QJsonValue v) -> QVariant {
        return QVariant::fromValue(yoyo::properties::patterned_string_t { v.toString(), {} });
      }) },
  { "yoyo::properties::limited_uint8_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue { return writeLimited<yoyo::properties::limited_uint8_t>(v); },
      [](QJsonValue v) -> QVariant { return readLimited<uint8_t>(v); }) },
  { "yoyo::properties::limited_uint16_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue { return writeLimited<yoyo::properties::limited_uint16_t>(v); },
      [](QJsonValue v) -> QVariant { return readLimited<uint16_t>(v); }) },
  { "yoyo::properties::limited_uint32_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        auto val = v.value<yoyo::properties::limited_uint32_t>();
        QJsonObject object;
        object.insert(yoyo::io::json::key_minimum,
                      QString::fromStdString(std::to_string(val._min)));
        object.insert(yoyo::io::json::key_maximum,
                      QString::fromStdString(std::to_string(val._max)));
        object.insert(yoyo::io::json::key_value,
                      QString::fromStdString(std::to_string(val._value)));
        return object;
      },
      [](QJsonValue v) -> QVariant {
        if (!v.isObject()) {
          return {};
        }

        auto o = v.toObject();

        if (!(o.contains(yoyo::io::json::key_minimum) && o.contains(yoyo::io::json::key_maximum)
              && o.contains(yoyo::io::json::key_value))) {
          return {};
        }

        return QVariant::fromValue(yoyo::properties::limited_uint32_t {
          static_cast<uint32_t>(std::strtoull(
            o[yoyo::io::json::key_value].toString().toStdString().data(), nullptr, 10)),
          static_cast<uint32_t>(std::strtoull(
            o[yoyo::io::json::key_minimum].toString().toStdString().data(), nullptr, 10)),
          static_cast<uint32_t>(std::strtoull(
            o[yoyo::io::json::key_maximum].toString().toStdString().data(), nullptr, 10)) });
      }) },
  { "yoyo::properties::limited_uint64_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        auto val = v.value<yoyo::properties::limited_uint64_t>();
        QJsonObject object;
        object.insert(yoyo::io::json::key_minimum,
                      QString::fromStdString(std::to_string(val._min)));
        object.insert(yoyo::io::json::key_maximum,
                      QString::fromStdString(std::to_string(val._max)));
        object.insert(yoyo::io::json::key_value,
                      QString::fromStdString(std::to_string(val._value)));
        return object;
      },
      [](QJsonValue v) -> QVariant {
        if (!v.isObject()) {
          return {};
        }

        auto o = v.toObject();

        if (!(o.contains(yoyo::io::json::key_minimum) && o.contains(yoyo::io::json::key_maximum)
              && o.contains(yoyo::io::json::key_value))) {
          return {};
        }

        return QVariant::fromValue(yoyo::properties::limited_uint64_t {
          static_cast<uint64_t>(std::strtoull(
            o[yoyo::io::json::key_value].toString().toStdString().data(), nullptr, 10)),
          static_cast<uint64_t>(std::strtoull(
            o[yoyo::io::json::key_minimum].toString().toStdString().data(), nullptr, 10)),
          static_cast<uint64_t>(std::strtoull(
            o[yoyo::io::json::key_maximum].toString().toStdString().data(), nullptr, 10)) });
      }) },
  { "yoyo::properties::limited_int8_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue { return writeLimited<yoyo::properties::limited_int8_t>(v); },
      [](QJsonValue v) -> QVariant { return readLimited<int8_t>(v); }) },
  { "yoyo::properties::limited_int16_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue { return writeLimited<yoyo::properties::limited_int16_t>(v); },
      [](QJsonValue v) -> QVariant { return readLimited<int16_t>(v); }) },
  { "yoyo::properties::limited_int32_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue { return writeLimited<yoyo::properties::limited_int32_t>(v); },
      [](QJsonValue v) -> QVariant { return readLimited<int32_t>(v); }) },
  { "yoyo::properties::limited_int64_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        auto val = v.value<yoyo::properties::limited_int64_t>();
        QJsonObject object;
        object.insert(yoyo::io::json::key_minimum,
                      QString::fromStdString(std::to_string(val._min)));
        object.insert(yoyo::io::json::key_maximum,
                      QString::fromStdString(std::to_string(val._max)));
        object.insert(yoyo::io::json::key_value,
                      QString::fromStdString(std::to_string(val._value)));
        return object;
      },
      [](QJsonValue v) -> QVariant {
        if (!v.isObject()) {
          return {};
        }

        auto o = v.toObject();

        if (!(o.contains(yoyo::io::json::key_minimum) && o.contains(yoyo::io::json::key_maximum)
              && o.contains(yoyo::io::json::key_value))) {
          return {};
        }

        return QVariant::fromValue(yoyo::properties::limited_int64_t {
          static_cast<int64_t>(std::strtoll(
            o[yoyo::io::json::key_value].toString().toStdString().data(), nullptr, 10)),
          static_cast<int64_t>(std::strtoll(
            o[yoyo::io::json::key_minimum].toString().toStdString().data(), nullptr, 10)),
          static_cast<int64_t>(std::strtoll(
            o[yoyo::io::json::key_maximum].toString().toStdString().data(), nullptr, 10)) });
      }) },
  { "yoyo::properties::limited_float_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        auto val = v.value<yoyo::properties::limited_float_t>();
        QJsonObject object;
        object.insert(yoyo::io::json::key_minimum, val._min);
        object.insert(yoyo::io::json::key_maximum, val._max);
        object.insert(yoyo::io::json::key_value, val._value);
        return object;
      },
      [](QJsonValue v) -> QVariant {
        if (!v.isObject()) {
          return {};
        }

        auto o = v.toObject();

        if (!(o.contains(yoyo::io::json::key_minimum) && o.contains(yoyo::io::json::key_maximum)
              && o.contains(yoyo::io::json::key_value))) {
          return {};
        }

        return QVariant::fromValue(yoyo::properties::limited_float_t {
          static_cast<float>(o[yoyo::io::json::key_value].toDouble()),
          static_cast<float>(o[yoyo::io::json::key_minimum].toDouble()),
          static_cast<float>(o[yoyo::io::json::key_maximum].toDouble()) });
      }) },
  { "yoyo::properties::script_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        auto val = v.value<yoyo::properties::script_t>();
        QJsonObject object;
        object.insert(yoyo::io::json::key_type, static_cast<uint8_t>(val._script_type));
        object.insert(yoyo::io::json::key_text, val._text);
        return object;
      },
      [](QJsonValue v) -> QVariant {
        if (!v.isObject()) {
          return {};
        }

        auto o = v.toObject();

        if (!(o.contains(yoyo::io::json::key_type) && o.contains(yoyo::io::json::key_text))) {
          return {};
        }

        return QVariant::fromValue(yoyo::properties::script_t {
          o[yoyo::io::json::key_text].toString(),
          static_cast<yoyo::properties::script_t::type>(o[yoyo::io::json::key_type].toInt()) });
      }) },
  { "yoyo::properties::transmission_direction_t",
    std::make_pair(
      [](QVariant v) -> QJsonValue {
        return static_cast<uint8_t>(v.value<yoyo::properties::transmission_direction_t>());
      },
      [](QJsonValue v) -> QVariant {
        return QVariant::fromValue(
          static_cast<yoyo::properties::transmission_direction_t>(v.toInt()));
      }) },
};
} // namespace yoyo::io::json
