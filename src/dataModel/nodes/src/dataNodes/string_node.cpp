#include "string_node.hpp"

#include <QVariant>

#include <boost/uuid/string_generator.hpp>

namespace yoyo
{
boost::uuids::uuid const string_node::_typeId =
  boost::uuids::string_generator {}("5f73d9e1-6da0-4c35-9c9e-88bc0c405941");

string_node::string_node(boost::uuids::uuid identifier)
  : data_value_node("string", identifier, types::value_t::STRING)
{
  setUnit({ "", false });
}

auto string_node::value() const -> types::variant_t
{
  return _bufferedValue;
}

auto string_node::setValue(QVariant const& value) -> void
{
  if ((access() != types::access_t::READ_WRITE) && (access() != types::access_t::WRITE)) {
    return;
  }

  if (static_cast<QMetaType::Type>(value.type()) != QMetaType::QString) {
    return;
  }

  _bufferedValue = value.toString();

  emitValueChanged(value);
}

auto string_node::typeId() -> boost::uuids::uuid
{
  return _typeId;
}

auto string_node::staticTypeId() const -> boost::uuids::uuid
{
  return _typeId;
}
} // namespace yoyo
