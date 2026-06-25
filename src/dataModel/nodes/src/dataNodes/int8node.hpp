#pragma once

#include "nodes_export.h"
#include "yoyo/data_value_node.h"

namespace yoyo
{
class YOYO_NODES_SHARED_EXPORT int8node final : public data_value_node
{
  Q_OBJECT

  Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip NOTIFY toolTipChanged)
  Q_PROPERTY(uint8_t min READ min WRITE setMin NOTIFY minChanged)
  Q_PROPERTY(uint8_t max READ max WRITE setMax NOTIFY maxChanged)
public:
  explicit int8node(boost::uuids::uuid identifier = boost::uuids::uuid {});
  virtual ~int8node() = default;
  int8node(int8node const& other) = delete;
  int8node& operator=(int8node const& other) = delete;
  int8node(int8node&& other) = delete;
  int8node& operator=(int8node&& other) = delete;

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto value() const -> types::variant_t final;
  auto setValue(QVariant const& value) -> void final;

  auto toolTip() const -> QString;
  auto setToolTip(QString v) -> void;
  auto min() const -> int8_t;
  auto setMin(int8_t v) -> void;
  auto max() const -> int8_t;
  auto setMax(int8_t v) -> void;
  static auto typeId() -> boost::uuids::uuid;
Q_SIGNALS:
  void toolTipChanged(QString v);
  void minChanged(int8_t v);
  void maxChanged(int8_t v);

private:
  static boost::uuids::uuid const _typeId;
  int8_t _bufferedValue { 0 };
  QString _toolTip;
  int8_t _min { std::numeric_limits<int8_t>::min() };
  int8_t _max { std::numeric_limits<int8_t>::max() };
};
} // namespace yoyo
