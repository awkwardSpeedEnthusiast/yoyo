#pragma once

#include "nodes_export.h"
#include "yoyo/data_value_node.h"

namespace yoyo
{
class YOYO_NODES_SHARED_EXPORT uint8node final : public data_value_node
{
  Q_OBJECT

  Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip NOTIFY toolTipChanged)
  Q_PROPERTY(uint8_t min READ min WRITE setMin NOTIFY minChanged)
  Q_PROPERTY(uint8_t max READ max WRITE setMax NOTIFY maxChanged)
  Q_PROPERTY(
    yoyo::properties::integer_format_t format READ format WRITE setFormat NOTIFY formatChanged)
  Q_PROPERTY(yoyo::properties::enum_t enumValues READ enumeration WRITE setEnumeration NOTIFY
               enumerationChanged)

public:
  explicit uint8node(boost::uuids::uuid identifier = boost::uuids::uuid {});
  virtual ~uint8node() = default;
  uint8node(uint8node const& other) = delete;
  uint8node& operator=(uint8node const& other) = delete;
  uint8node(uint8node&& other) = delete;
  uint8node& operator=(uint8node&& other) = delete;

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto value() const -> types::variant_t final;
  auto setValue(QVariant const& value) -> void final;

  auto toolTip() const -> QString;
  auto setToolTip(QString v) -> void;
  auto min() const -> uint8_t;
  auto setMin(uint8_t v) -> void;
  auto max() const -> uint8_t;
  auto setMax(uint8_t v) -> void;
  auto format() const -> properties::integer_format_t;
  auto setFormat(properties::integer_format_t v) -> void;
  auto enumeration() const -> properties::enum_t;
  auto setEnumeration(properties::enum_t v) -> void;
  static auto typeId() -> boost::uuids::uuid;
Q_SIGNALS:
  void toolTipChanged(QString v);
  void minChanged(uint8_t v);
  void maxChanged(uint8_t v);
  void formatChanged(yoyo::properties::integer_format_t v);
  void enumerationChanged(yoyo::properties::enum_t v);

private:
  static boost::uuids::uuid const _typeId;
  uint8_t _bufferedValue { 0 };
  QString _toolTip;
  uint8_t _min { std::numeric_limits<uint8_t>::min() };
  uint8_t _max { std::numeric_limits<uint8_t>::max() };
  properties::integer_format_t _format;
  properties::enum_t _enum;
};
} // namespace yoyo
