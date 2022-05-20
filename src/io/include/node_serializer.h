#pragma once
#include "io_export.h"

#include <memory>
#include <ostream>
#include <vector>

namespace yoyo
{
class node_factory;
class node_base;
} // namespace yoyo

namespace yoyo::io
{
///
/// \brief base class for serializer strategies
///
/// This class provides the interface for serializer strategies to be used by implementations.
/// The decision in this interface went to serialize to an output stream instead of directly writing
/// to file was made to allow further processing (like encryption) or to make the serialization
/// available for logging purposes.
///
class YOYO_IO_SHARED_EXPORT node_serializer
{
public:
  ///
  /// \brief constructor
  /// \param factories the node factories to access the description of the nodes to deserialize.
  ///
  explicit node_serializer(std::vector<std::shared_ptr<node_factory>> factories);
  virtual ~node_serializer();
  node_serializer(node_serializer const&) = default;
  node_serializer(node_serializer&&) = default;
  node_serializer& operator=(node_serializer const&) = default;
  node_serializer& operator=(node_serializer&&) = default;

  ///
  /// \brief serialize
  ///
  /// This method is to be invoked, when serializing a hierarchy of nodes to an output stream.
  /// When implementing this method, make sure to provide a format identifier first thing to the
  /// stream to make it identifiable for the corresponding deserializer.
  /// \param output the stream to write the output to,
  /// \param root_node the root of the hierarchy to write.
  ///
  virtual auto serialize(std::ostream& output, std::shared_ptr<node_base> root_node) const
    -> void = 0;

protected:
  ///
  /// Use these factories to access the descriptions of the nodes to serialize, in order to avoid
  /// writing properties, which have default values (example).
  ///
  std::vector<std::shared_ptr<node_factory>> _factories;
};
} // namespace yoyo::io
