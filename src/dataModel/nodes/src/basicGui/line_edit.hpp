#pragma once

#include "export_on_test.hpp"
#include "leaf_node.hpp"

#include <memory>

namespace yoyo
{
class YOYO_TESTING_EXPORT line_edit final : public leaf_node
{
  Q_OBJECT

  Q_PROPERTY(
    yoyo::properties::connected_string_t title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(yoyo::properties::connected_string_t unit READ unit WRITE setUnit NOTIFY unitChanged)
  Q_PROPERTY(yoyo::properties::connection_t connection READ connection WRITE setConnection NOTIFY
               connectionChanged)
  Q_PROPERTY(bool captureKey READ captureKey WRITE setCaptureKey NOTIFY captureKeyChanged)

public:
  explicit line_edit(boost::uuids::uuid identifier = boost::uuids::uuid {});
  ~line_edit() override;
  line_edit(line_edit const& other) = delete;
  line_edit& operator=(line_edit const& other) = delete;
  line_edit(line_edit&& other) = delete;
  line_edit& operator=(line_edit&& other) = delete;
  auto init() -> void final;

  auto staticTypeId() const -> boost::uuids::uuid final;
  auto supportsValueType(types::value_t valueType) const -> bool final;

  auto title() const -> yoyo::properties::connected_string_t;
  auto setTitle(yoyo::properties::connected_string_t const& t) -> void;

  auto unit() const -> yoyo::properties::connected_string_t;
  auto setUnit(yoyo::properties::connected_string_t const& t) -> void;

  auto connection() const -> yoyo::properties::connection_t;
  auto setConnection(yoyo::properties::connection_t s) -> void;

  auto captureKey() const -> bool;
  auto setCaptureKey(bool v) -> void;

  static auto typeId() -> boost::uuids::uuid;
Q_SIGNALS:
  void titleChanged(yoyo::properties::connected_string_t t);
  void unitChanged(yoyo::properties::connected_string_t t);
  void connectionChanged(yoyo::properties::connection_t s);
  void captureKeyChanged(bool capture);

private:
  static boost::uuids::uuid const _typeId;
  struct Impl;
  std::unique_ptr<Impl> _p;
};
} // namespace yoyo
