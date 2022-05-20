#pragma once
#include "io_export.h"

#include <istream>
#include <memory>
#include <vector>

namespace yoyo
{
class node_factory;
class node_base;
} // namespace yoyo

namespace yoyo::io
{
///
/// \brief base class for deserializer strategies
///
/// This class provides the interface for deserializer strategies to be used by implementations.
/// The decision in this interface went to deserialize from an input stream instead of directly from
/// file was made to allow further processing (like encryption) or to make the serialization
/// available for logging purposes.
///
class YOYO_IO_SHARED_EXPORT node_deserializer
{
public:
  ///
  /// \brief constructor
  /// \param factories the factories to be used for creating nodes during deserialization.
  ///
  explicit node_deserializer(std::vector<std::shared_ptr<node_factory>> factories);
  virtual ~node_deserializer();
  node_deserializer(node_deserializer const&) = default;
  node_deserializer(node_deserializer&&) = default;
  node_deserializer& operator=(node_deserializer const&) = default;
  node_deserializer& operator=(node_deserializer&&) = default;

  ///
  /// \brief deserialize
  ///
  /// This method is to be invoked, when deserializing from an input creating a node hierarchy.
  /// When implementing this method, make sure to verify the format (in conjunction with the
  /// appropriate serializer, prefixing the output with a format identifier).
  /// \param input the stream to read from,
  /// \return the root node of the created node hierarchy.
  ///
  virtual auto deserialize(std::istream& input) const -> std::shared_ptr<node_base> = 0;

protected:
  ///
  /// \brief node factories
  ///
  /// Use these factories for creating nodes during deserialization.
  ///
  std::vector<std::shared_ptr<node_factory>> _factories;
};
} // namespace yoyo::io
