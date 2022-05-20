#pragma once
#include "yoyo/data_node.h"
#include "yoyo/property_types.h"
#include "yoyo/yoyo_sdk_global.h"

namespace yoyo
{
///
/// \brief The base class for data nodes containing children
///
/// This class is meant as convenience base class for data-nodes which contain children.
///
class YOYO_SDK_SHARED_EXPORT data_container_node : public data_node
{
  Q_OBJECT
public:
  ///
  /// \brief Constructor
  /// \param type_name the name of the type to be displayed in gui,
  /// \param id the identifier of this node instance,
  /// \param valuetype the value-type of the container node.
  ///
  data_container_node(QString type_name, boost::uuids::uuid id, types::value_t valuetype);

  data_container_node(data_container_node const& other) = delete;
  data_container_node& operator=(data_container_node const& other) = delete;
  data_container_node(data_container_node&& other) = delete;
  data_container_node& operator=(data_container_node&& other) = delete;

  auto setValue(QVariant const& value) -> void override;
  auto acceptsChildren() const -> bool override;
  auto acceptsChild(std::shared_ptr<node_base> const& child) const -> bool override;
  auto acceptsParent(std::shared_ptr<node_base> const& parent) const -> bool override;

private:
  auto childAboutToBeAdded(std::shared_ptr<node_base> const& child) -> bool override;
  auto childAdded(std::shared_ptr<node_base> const& child) -> bool override;
  auto childAboutToBeRemoved(std::shared_ptr<node_base> const& child) -> bool override;
  auto childRemoved(std::shared_ptr<node_base> const& child) -> bool override;
  auto childMoved(std::shared_ptr<node_base> const& child, size_t from_index, size_t to_index)
    -> void override;
};
} // namespace yoyo
