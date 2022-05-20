#pragma once
#include "node_deserializer.h"
#include "node_serializer.h"

#include "io_export.h"

#include <boost/uuid/uuid.hpp>

#include <functional>
#include <map>
#include <string>
#include <tuple>

namespace yoyo
{
class node_factory;
}

namespace yoyo::io
{
///
/// \brief a class providing access to input-/output-managing objects.
///
/// The io_manager holds a list of objects to do serialization (output) or deserialization (input)
/// of nodes. It offers install and deinstall methods for these objects. A list of installed objects
/// can be retrieved as well as each object by its id.
///
/// Even though it does not fulfill all criteria of a singleton, it is only meant to be created once
/// by the main program. Use the free function \a manager() declared at the bottom of this file to
/// access the instance to use.
///
class YOYO_IO_SHARED_EXPORT io_manager final
{
public:
  ///
  /// \brief Type alias for a tuple holding information about an io-object.
  ///
  /// Index 0: the identifier for the io-object, use this to retrieve the actual object,
  /// Index 1: the name of the io-object,
  /// Index 2: a description of the io-object for (UI-)display purposes,
  /// Index 3: the file pattern to store the serialized data in (example: "*.json", "*.xml")
  ///
  using io_info_t = std::tuple<boost::uuids::uuid, std::string, std::string, std::string>;

public:
  ///
  /// \brief Constructor
  /// \param factories the node factories to use to create nodes; node descriptions will be
  ///        accessed as well.
  ///
  io_manager(std::vector<std::shared_ptr<node_factory>> factories);
  ~io_manager();
  io_manager(io_manager const&) = delete;
  io_manager(io_manager&&) = delete;
  io_manager& operator=(io_manager const&) = delete;
  io_manager& operator=(io_manager&&) = delete;

  ///
  /// \{
  /// \name access

  ///
  /// \brief retrieve a list of descriptions for installed serializers
  /// \return the list of serializer descriptions.
  ///
  auto installed_serializers() const -> std::vector<io_info_t>;
  ///
  /// \brief retrieve a list of descriptions for installed deserializers
  /// \return the list of deserializer descriptions.
  ///
  auto installed_deserializers() const -> std::vector<io_info_t>;

  ///
  /// \brief access serializer
  ///
  /// This method is used to retrieve an installed serializer. If a serializer of the provided id
  /// is not installed, it returns a nullptr object.
  /// \param id the identifier of the serializer to retrieve,
  /// \return the installed serializer.
  ///
  auto serializer(boost::uuids::uuid id) const -> std::shared_ptr<node_serializer>;
  ///
  /// \brief access deserializer
  ///
  /// This method is used to retrieve an installed deserializer. If a serializer of the provided
  /// id is not installed, it returns a nullptr object.
  /// \param id the identifier of the deserializer to retrieve,
  /// \return the installed deserializer.
  ///
  auto deserializer(boost::uuids::uuid id) const -> std::shared_ptr<node_deserializer>;

  /// \}

  /// \{
  /// \name management

  ///
  /// \brief install a serializer
  ///
  /// Use this method to install a serializer object to an existing manager. If the manager has
  /// not yet been created, please use the static version of this method.
  /// \param id the identifier of the serializer,
  /// \param name the name of the serializer,
  /// \param description the description of the serializer to be used for UI display purposes,
  /// \param file_pattern the pattern/extension of the file which is to be used for this
  ///          serializer, if the serialized data is to be stored in a file,
  /// \param to_install the serializer object to install,
  /// \return true if the serializer could be installed.
  ///
  auto install_serializer(boost::uuids::uuid id, std::string name, std::string description,
                          std::string file_pattern, std::shared_ptr<node_serializer> to_install)
    -> bool;
  ///
  /// \brief install a deserializer
  ///
  /// Use this method to install a deserializer object to an existing manager. If the manager has
  /// not yet been created, please use the static version of this method.
  /// \param id the identifier of the deserializer,
  /// \param name the name of the deserializer,
  /// \param description the description of the deserializer to be used for UI display purposes,
  /// \param file_pattern the pattern/extension of the file which is to be used for this
  ///          serializer, if the deserialized data is to be read from a file,
  /// \param to_install the deserializer object to install,
  /// \return true if the deserializer could be installed.
  ///
  auto install_deserializer(boost::uuids::uuid id, std::string name, std::string description,
                            std::string file_pattern, std::shared_ptr<node_deserializer> to_install)
    -> bool;
  ///
  /// \brief de-install a serializer
  ///
  /// Use this method to remove a serializer from the manager. If there is no serializer with this
  /// identifier, this method will return false.
  /// \param id the identifier of the serializer to de-install,
  /// \return true if successful.
  ///
  auto remove_serializer(boost::uuids::uuid id) -> bool;
  ///
  /// \brief de-install a deserializer
  ///
  /// Use this method to remove a deserializer from the manager. If there is no deserializer with
  /// this identifier, this method will return false.
  /// \param id the identifier of the deserializer to de-install,
  /// \return true if successful.
  ///
  auto remove_deserializer(boost::uuids::uuid id) -> bool;

  ///
  /// \brief install a serializer
  ///
  /// In case there is no manager instance available yet (\a manager() below returning a nullptr),
  /// please use this method to make your serializer available to a manager as soon as it is
  /// created.
  /// \note The installers will be cleared after the first instance is created, so do not try to
  ///   create one.
  /// \param id the identifier for the serializer in this manager,
  /// \param name the name of the serialzer,
  /// \param description the description of the serializer for (UI-)display purposes,
  /// \param file_pattern the file pattern/extension to be suggested, if the serialized data is to
  ///   be written to file,
  /// \param to_install a creation function for the serializer to install. Since at this point the
  ///   factories are not yet available, a functor is necessary for delayed creation.
  ///
  static auto install_serializer(
    boost::uuids::uuid id, std::string name, std::string description, std::string file_pattern,
    std::function<std::shared_ptr<node_serializer>(std::vector<std::shared_ptr<node_factory>>)>
      to_install) -> void;
  ///
  /// \brief install a deserializer
  ///
  /// In case there is no manager instance available yet (\a manager() below returning a nullptr),
  /// please use this method to make your deserializer available to a manager as soon as it is
  /// created.
  /// \note The installers will be cleared after the first instance is created, so do not try to
  ///   create one.
  /// \param id the identifier for the deserializer in this manager,
  /// \param name the name of the deserialzer,
  /// \param description the description of the deserializer for (UI-)display purposes,
  /// \param file_pattern the file pattern/extension to be used, if the deserialized data is read
  ///   from file,
  /// \param to_install a creation function for the deserializer to install. Since at this point
  ///   the factories are not yet available, a functor is necessary for delayed creation.
  ///
  static auto install_deserializer(
    boost::uuids::uuid id, std::string name, std::string description, std::string file_pattern,
    std::function<std::shared_ptr<node_deserializer>(std::vector<std::shared_ptr<node_factory>>)>
      to_install) -> void;

private:
  class impl;
  std::unique_ptr<impl> _p;

  static std::vector<std::pair<io_info_t, std::function<std::shared_ptr<node_serializer>(
                                            std::vector<std::shared_ptr<node_factory>>)>>>
    _s_installers;
  static std::vector<std::pair<io_info_t, std::function<std::shared_ptr<node_deserializer>(
                                            std::vector<std::shared_ptr<node_factory>>)>>>
    _d_installers;
};

auto YOYO_IO_SHARED_EXPORT manager() -> std::weak_ptr<io_manager>;
auto YOYO_IO_SHARED_EXPORT init_manager(std::shared_ptr<io_manager> manager) -> void;
auto YOYO_IO_SHARED_EXPORT deinit_manager() -> void;
} // namespace yoyo::io
