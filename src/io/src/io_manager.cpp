#include "io_manager.h"
#include "json/node_deserializer_json.hpp"
#include "json/node_serializer_json.hpp"

#include "yoyo/node_factory.h"

namespace
{
std::shared_ptr<yoyo::io::io_manager> global_io_manager { nullptr };
}
namespace yoyo::io
{
std::vector<std::pair<io_manager::io_info_t, std::function<std::shared_ptr<node_serializer>(
                                               std::vector<std::shared_ptr<node_factory>>)>>>
  io_manager::_s_installers;
std::vector<std::pair<io_manager::io_info_t, std::function<std::shared_ptr<node_deserializer>(
                                               std::vector<std::shared_ptr<node_factory>>)>>>
  io_manager::_d_installers;

auto manager() -> std::weak_ptr<io_manager>
{
  return ::global_io_manager;
}

auto init_manager(std::shared_ptr<io_manager> manager) -> void
{
  ::global_io_manager = manager;
}

auto deinit_manager() -> void
{
  ::global_io_manager.reset();
}

auto add_default() -> void
{
  io_manager::install_serializer({ 0x1c, 0xbe, 0x13, 0xed, 0xa1, 0x6f, 0x4d, 0x58, 0xb0, 0xae, 0x27,
                                   0x1c, 0xf0, 0x0b, 0x17, 0x55 },
                                 "Json", "Json file format", "*.json", [](auto const& factories) {
                                   return std::make_shared<node_serializer_json>(factories);
                                 });
  io_manager::install_deserializer({ 0x1c, 0xbe, 0x13, 0xed, 0xa1, 0x6f, 0x4d, 0x58, 0xb0, 0xae,
                                     0x27, 0x1c, 0xf0, 0x0b, 0x17, 0x55 },
                                   "Json", "Json file format", "*.json", [](auto const& factories) {
                                     return std::make_shared<node_deserializer_json>(factories);
                                   });
}

class io_manager::impl
{
public:
  impl(std::vector<std::shared_ptr<node_factory>> factories)
    : _factories { std::move(factories) }
  {
  }
  std::vector<std::shared_ptr<node_factory>> _factories;
  std::vector<io_info_t> _serializer_info;
  std::vector<io_info_t> _deserializer_info;
  std::map<boost::uuids::uuid, std::shared_ptr<node_serializer>> _serializers;
  std::map<boost::uuids::uuid, std::shared_ptr<node_deserializer>> _deserializers;
};

io_manager::io_manager(std::vector<std::shared_ptr<node_factory>> factories)
  : _p { std::make_unique<impl>(std::move(factories)) }
{
  for (auto i : _s_installers) {
    install_serializer(std::get<boost::uuids::uuid>(i.first), std::get<1>(i.first),
                       std::get<2>(i.first), std::get<3>(i.first), i.second(_p->_factories));
  }

  for (auto i : _d_installers) {
    install_deserializer(std::get<boost::uuids::uuid>(i.first), std::get<1>(i.first),
                         std::get<2>(i.first), std::get<3>(i.first), i.second(_p->_factories));
  }

  _s_installers.clear();
  _d_installers.clear();
}

io_manager::~io_manager() = default;

auto io_manager::installed_serializers() const -> std::vector<io_manager::io_info_t>
{
  return _p->_serializer_info;
}

auto io_manager::installed_deserializers() const -> std::vector<io_manager::io_info_t>
{
  return _p->_deserializer_info;
}

auto io_manager::serializer(boost::uuids::uuid id) const -> std::shared_ptr<node_serializer>
{
  auto it = _p->_serializers.find(id);

  if (it == std::end(_p->_serializers)) {
    return {};
  }

  return it->second;
}

auto io_manager::deserializer(boost::uuids::uuid id) const -> std::shared_ptr<node_deserializer>
{
  auto it = _p->_deserializers.find(id);

  if (it == std::end(_p->_deserializers)) {
    return {};
  }

  return it->second;
}

auto io_manager::install_serializer(boost::uuids::uuid id, std::string name,
                                    std::string description, std::string file_pattern,
                                    std::shared_ptr<node_serializer> to_install) -> bool
{
  auto it = _p->_serializers.find(id);

  if (it != std::end(_p->_serializers)) {
    return false;
  }

  _p->_serializers.insert(std::make_pair(id, to_install));
  _p->_serializer_info.push_back({ id, name, description, file_pattern });
  return true;
}

auto io_manager::install_deserializer(boost::uuids::uuid id, std::string name,
                                      std::string description, std::string file_pattern,
                                      std::shared_ptr<node_deserializer> to_install) -> bool
{
  auto it = _p->_deserializers.find(id);

  if (it != std::end(_p->_deserializers)) {
    return false;
  }

  _p->_deserializers.insert(std::make_pair(id, to_install));
  _p->_deserializer_info.push_back({ id, name, description, file_pattern });
  return true;
}

auto io_manager::remove_serializer(boost::uuids::uuid id) -> bool
{
  auto it = _p->_serializers.find(id);

  if (it == std::end(_p->_serializers)) {
    return false;
  }

  _p->_serializers.erase(it);
  _p->_serializer_info.erase(
    std::remove_if(std::begin(_p->_serializer_info), std::end(_p->_serializer_info),
                   [id](auto e) { return std::get<boost::uuids::uuid>(e) == id; }),
    std::end(_p->_serializer_info));
  return true;
}

auto io_manager::remove_deserializer(boost::uuids::uuid id) -> bool
{
  auto it = _p->_deserializers.find(id);

  if (it == std::end(_p->_deserializers)) {
    return false;
  }

  _p->_deserializers.erase(it);
  _p->_deserializer_info.erase(
    std::remove_if(std::begin(_p->_deserializer_info), std::end(_p->_deserializer_info),
                   [id](auto e) { return std::get<boost::uuids::uuid>(e) == id; }),
    std::end(_p->_deserializer_info));
  return true;
}

auto io_manager::install_serializer(
  boost::uuids::uuid id, std::string name, std::string description, std::string file_pattern,
  std::function<std::shared_ptr<node_serializer>(std::vector<std::shared_ptr<node_factory>>)>
    to_install) -> void
{
  if (::global_io_manager) {
    ::global_io_manager->install_serializer(std::move(id), std::move(name), std::move(description),
                                            std::move(file_pattern),
                                            to_install(::global_io_manager->_p->_factories));
  } else {
    _s_installers.push_back(std::make_pair(
      io_info_t { std::move(id), std::move(name), std::move(description), std::move(file_pattern) },
      to_install));
  }
}

auto io_manager::install_deserializer(
  boost::uuids::uuid id, std::string name, std::string description, std::string file_pattern,
  std::function<std::shared_ptr<node_deserializer>(std::vector<std::shared_ptr<node_factory>>)>
    to_install) -> void
{
  if (::global_io_manager) {
    ::global_io_manager->install_deserializer(std::move(id), std::move(name),
                                              std::move(description), std::move(file_pattern),
                                              to_install(::global_io_manager->_p->_factories));
  } else {
    _d_installers.push_back(std::make_pair(
      io_info_t { std::move(id), std::move(name), std::move(description), std::move(file_pattern) },
      to_install));
  }
}

node_deserializer::node_deserializer(std::vector<std::shared_ptr<node_factory>> factories)
  : _factories(factories)
{
}
node_deserializer::~node_deserializer() = default;
node_serializer::node_serializer(std::vector<std::shared_ptr<node_factory>> factories)
  : _factories(factories)
{
}
node_serializer::~node_serializer() = default;
} // namespace yoyo::io
