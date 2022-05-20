#include "connectivity_manager.hpp"
#include "treeUtils.hpp"

#include "yoyo/data_node.h"

#include <QMetaProperty>

#include <iostream>
#include <string>
using std::string_literals::operator""s;
namespace
{
std::shared_ptr<yoyo::node_base> findChild(QString const& path,
                                           std::shared_ptr<yoyo::node_base> parent)
{
  return yoyo::utilities::retrieveFromPath<yoyo::utilities::path_strategy_t::NAME>(path, parent);
}

class connected_property_tracker : public QObject
{
  Q_OBJECT

public:
  connected_property_tracker(std::function<void(QVariant)> callback);
  ~connected_property_tracker() override;

protected:
  std::function<void(QVariant)> _callback;
  QMetaObject::Connection _connection;
};

class connected_property_tracker_qstring : public connected_property_tracker
{
  Q_OBJECT

public:
  connected_property_tracker_qstring(QObject* o, QMetaProperty& p,
                                     std::function<void(QVariant)> callback);
  ~connected_property_tracker_qstring() override;
private Q_SLOTS:
  void onChange(QString v);
};

class connected_property_tracker_invisible_string : public connected_property_tracker
{
  Q_OBJECT

public:
  connected_property_tracker_invisible_string(QObject* o, QMetaProperty& p,
                                              std::function<void(QVariant)> callback);
  ~connected_property_tracker_invisible_string() override;
private Q_SLOTS:
  void onChange(yoyo::properties::invisible_string_t v);
};

class connected_property_tracker_enum : public connected_property_tracker
{
  Q_OBJECT

public:
  connected_property_tracker_enum(QObject* o, QObject* t, QMetaProperty& p,
                                  std::function<void(QVariant)> callback);
  ~connected_property_tracker_enum() override = default;
private Q_SLOTS:
  void onSourceChange(yoyo::properties::enum_t v);
  void onTargetChange(yoyo::properties::enum_t v);

private:
  bool _targetsOwn { false };
  QMetaObject::Connection _target_connection;
};

auto create_property_tracker(QObject* source, QObject* target, QMetaProperty& p,
                             std::function<void(QVariant)> callback)
  -> std::shared_ptr<connected_property_tracker>
{
  if (p.typeName() == "QString"s) {
    return std::make_shared<connected_property_tracker_qstring>(source, p, std::move(callback));
  }

  if (p.typeName() == "yoyo::properties::invisible_string_t"s) {
    return std::make_shared<connected_property_tracker_invisible_string>(source, p,
                                                                         std::move(callback));
  }

  if (p.typeName() == "yoyo::properties::enum_t"s) {
    return std::make_shared<connected_property_tracker_enum>(source, target, p,
                                                             std::move(callback));
  }

  return {};
}

connected_property_tracker::connected_property_tracker(std::function<void(QVariant)> callback)
  : QObject()
  , _callback(callback)
{
}

connected_property_tracker_qstring::connected_property_tracker_qstring(
  QObject* o, QMetaProperty& p, std::function<void(QVariant)> callback)
  : connected_property_tracker(callback)
{
  _connection =
    connect(o, p.notifySignal(), this, staticMetaObject.method(staticMetaObject.methodOffset()));
  _callback(o->property(p.name()));
}

connected_property_tracker_invisible_string::connected_property_tracker_invisible_string(
  QObject* o, QMetaProperty& p, std::function<void(QVariant)> callback)
  : connected_property_tracker(callback)
{
  _connection =
    connect(o, p.notifySignal(), this, staticMetaObject.method(staticMetaObject.methodOffset()));
  _callback(o->property(p.name()).value<yoyo::properties::invisible_string_t>()._s);
}

connected_property_tracker_enum::connected_property_tracker_enum(
  QObject* o, QObject* t, QMetaProperty& p, std::function<void(QVariant)> callback)
  : connected_property_tracker(callback)
{
  auto vs = o->property(p.name()).value<yoyo::properties::enum_t>();
  auto vt = t->property(p.name()).value<yoyo::properties::enum_t>();
  _targetsOwn = !(vs == vt || vt == yoyo::properties::enum_t {});

  _connection =
    connect(o, p.notifySignal(), this, staticMetaObject.method(staticMetaObject.methodOffset()));
  _target_connection =
    connect(t, t->metaObject()->property(t->metaObject()->indexOfProperty(p.name())).notifySignal(),
            this, staticMetaObject.method(staticMetaObject.methodOffset() + 1));

  if (!_targetsOwn) {
    _callback(QVariant::fromValue(vs));
  }
}

connected_property_tracker::~connected_property_tracker()
{
  disconnect(_connection);
}

connected_property_tracker_qstring::~connected_property_tracker_qstring()
{
  _callback(QString { "" });
}

connected_property_tracker_invisible_string::~connected_property_tracker_invisible_string()
{
  _callback(QString { "" });
}

void connected_property_tracker_qstring::onChange(QString v)
{
  _callback(v);
}

void connected_property_tracker_invisible_string::onChange(yoyo::properties::invisible_string_t v)
{
  _callback(v._s);
}

void connected_property_tracker_enum::onSourceChange(yoyo::properties::enum_t v)
{
  if (!_targetsOwn) {
    _callback(QVariant::fromValue(v));
  }
}

void connected_property_tracker_enum::onTargetChange(yoyo::properties::enum_t v)
{
  if (v == yoyo::properties::enum_t {}) {
    _targetsOwn = false;
  }
}

class single_connection_tracker : public QObject
{
  Q_OBJECT

public:
  single_connection_tracker(std::weak_ptr<yoyo::node_base> node, int propertyIndex,
                            std::weak_ptr<yoyo::node_base> data_root);
  ~single_connection_tracker();
public Q_SLOTS:
  void on_connection_changed();

private:
  std::weak_ptr<yoyo::node_base> _node;
  int _propertyIndex;
  std::weak_ptr<yoyo::node_base> _data_root;
  std::weak_ptr<yoyo::node_base> _connected_data_in;
  std::weak_ptr<yoyo::node_base> _connected_data_out;
  boost::signals2::scoped_connection _in_connection;
  boost::signals2::scoped_connection _out_connection;
  std::vector<std::shared_ptr<connected_property_tracker>> _data_connections;
};

single_connection_tracker::single_connection_tracker(std::weak_ptr<yoyo::node_base> node,
                                                     int propertyIndex,
                                                     std::weak_ptr<yoyo::node_base> data_root)
  : _node { node }
  , _propertyIndex { propertyIndex }
  , _data_root { data_root }
{
  static int slot_index = staticMetaObject.methodOffset();
  auto n = _node.lock();

  if (!n) {
    throw std::runtime_error("node not valid");
  }

  auto property = n->metaObject()->property(propertyIndex);
  connect(n.get(), property.notifySignal(), this, staticMetaObject.method(slot_index));
  on_connection_changed();
}

auto single_connection_tracker::on_connection_changed() -> void
{
  auto n = _node.lock();

  if (!n) {
    return;
  }

  auto prop = n->property(n->metaObject()->property(_propertyIndex).name());

  if (!prop.isValid()) {
    return;
  }

  auto connectInput = [this](std::shared_ptr<yoyo::data_node> data, auto& connection) {
    _in_connection = data->connectToValue(connection._setter);
    _connected_data_in = data;
  };
  auto connectOutput = [this](std::shared_ptr<yoyo::data_node> data, auto& connection) {
    _out_connection = connection._connector([data](QVariant v) { data->setValue(v); });
    _connected_data_out = data;
  };
  auto connectData = [n, this](std::shared_ptr<yoyo::data_node> data) {
    _data_connections.clear();
    auto n_meta = n->metaObject();
    auto d_meta = data->metaObject();

    auto connect_property = [&n, &data, this](QMetaObject const* d_meta, QMetaObject const* n_meta,
                                              int i) {
      auto prop = d_meta->property(d_meta->indexOfProperty(n_meta->property(i).name()));
      std::function<void(QVariant)> connected_string_setter =
        [n, name = n_meta->property(i).name()](QVariant v) {
          auto cv = n->property(name).value<yoyo::properties::connected_string_t>();
          cv._connected_value = v.toString();
          n->setProperty(name, QVariant::fromValue(cv));
        };
      std::function<void(QVariant)> enum_setter =
        [n, name = n_meta->property(i).name()](QVariant v) { n->setProperty(name, v); };
      auto cp_handler = create_property_tracker(
        data.get(), n.get(), prop,
        (prop.typeName() == "yoyo::properties::enum_t"s ? enum_setter : connected_string_setter));

      if (cp_handler) {
        _data_connections.push_back(cp_handler);
      }
    };

    for (int i = yoyo::node_base::staticMetaObject.propertyOffset(); i < n_meta->propertyCount();
         i++) {
      if (d_meta->indexOfProperty(n_meta->property(i).name()) >= 0) {
        if ((n_meta->property(i).typeName() == "yoyo::properties::connected_string_t"s)
            || (n_meta->property(i).typeName() == "yoyo::properties::enum_t"s)) {
          connect_property(d_meta, n_meta, i);
        }
      }
    }
  };

  if (prop.typeName() == std::string { "yoyo::properties::connection_t" }) {
    auto connection = prop.value<yoyo::properties::connection_t>();

    if (!connection._auto.isEmpty()) {
      if (auto data = std::dynamic_pointer_cast<yoyo::data_node>(
            ::findChild(connection._auto, _data_root.lock()))) {
        if (data->canConnect() && connection._supports(data->valueType())
            && (data != _connected_data_in.lock()) && (data != _connected_data_out.lock())) {
          connectInput(data, connection);
          connectOutput(data, connection);
          connectData(data);
        }
      } else {
        _in_connection.disconnect();
        _out_connection.disconnect();
        _connected_data_in = {};
        _connected_data_out = {};
        _data_connections.clear();
      }
    } else {
      if (!connection._in.isEmpty()) {
        if (auto data = std::dynamic_pointer_cast<yoyo::data_node>(
              ::findChild(connection._in, _data_root.lock()))) {
          if (data->canConnect() && connection._supports(data->valueType())
              && (data != _connected_data_in.lock())) {
            connectInput(data, connection);
          }
        } else {
          _in_connection.disconnect();
          _connected_data_in = {};
        }
      }

      if (!connection._out.isEmpty()) {
        if (auto data = std::dynamic_pointer_cast<yoyo::data_node>(
              ::findChild(connection._out, _data_root.lock()))) {
          if (data->canConnect() && connection._supports(data->valueType())
              && (data != _connected_data_out.lock())) {
            connectOutput(data, connection);
          }
        } else {
          _out_connection.disconnect();
          _connected_data_out = {};
        }
      }
    }
  }

  if (prop.typeName() == std::string { "yoyo::properties::in_connection_t" }) {
    auto connection = prop.value<yoyo::properties::in_connection_t>();

    if (!connection._in.isEmpty()) {
      if (auto data = std::dynamic_pointer_cast<yoyo::data_node>(
            ::findChild(connection._in, _data_root.lock()))) {
        if (data->canConnect() && connection._supports(data->valueType())
            && (data != _connected_data_in.lock())) {
          connectInput(data, connection);
          connectData(data);
        }
      } else {
        _in_connection.disconnect();
        _connected_data_in = {};
        _data_connections.clear();
      }
    }
  }

  if (prop.typeName() == std::string { "yoyo::properties::out_connection_t" }) {
    auto connection = prop.value<yoyo::properties::out_connection_t>();

    if (!connection._out.isEmpty()) {
      if (auto data = std::dynamic_pointer_cast<yoyo::data_node>(
            ::findChild(connection._out, _data_root.lock()))) {
        if (data->canConnect() && connection._supports(data->valueType())
            && (data != _connected_data_out.lock())) {
          connectOutput(data, connection);
          connectData(data);
        }
      } else {
        _out_connection.disconnect();
        _connected_data_out = {};
        _data_connections.clear();
      }
    }
  }
}

single_connection_tracker::~single_connection_tracker() {}
} // namespace
namespace yoyo
{
class connectivity_manager::impl
{
public:
  using connection_set =
    std::tuple<std::vector<QMetaObject::Connection>, std::vector<boost::signals2::connection>>;

  impl(std::weak_ptr<node_base> root)
  {
    auto n = root.lock();

    if (!n) {
      throw std::runtime_error("Connectivity manager: Invalid root node");
    }

    QObject::connect(n.get(), &node_base::treeChanged,
                     [this](std::weak_ptr<node_base>, std::weak_ptr<node_base> object,
                            node_base::ChangeOperation operationType) {
                       switch (operationType) {
                       case node_base::ChangeOperation::MOVED: return;
                       case node_base::ChangeOperation::ADDED: on_node_add(object); break;
                       case node_base::ChangeOperation::REMOVED: on_node_remove(object); break;
                       }
                     });

    for (auto& child : *n) {
      on_node_add(child);
    }
  }

  auto on_node_add(std::weak_ptr<node_base> node) -> void;
  auto on_node_remove(std::weak_ptr<node_base> node) -> void;
  //    auto connection_changed(int propertyIndex, std::weak_ptr<node_base> to_node)->void;
  auto reevaluate_connections() -> void;

private:
  std::map<node_base*, std::vector<std::shared_ptr<single_connection_tracker>>> _nodes;
  std::map<node_base*, QMetaObject::Connection> _reference_nodes;
  std::weak_ptr<node_base> _data_root;
};

auto connectivity_manager::impl::on_node_add(std::weak_ptr<node_base> node) -> void
{
  if (auto n = node.lock()) {
    if (n->type() == "Signals") {
      _data_root = n;
    }

    auto meta = n->metaObject();

    if (meta->inherits(&data_node::staticMetaObject)) {
      _reference_nodes.insert(
        std::make_pair(n.get(), QObject::connect(n.get(), &data_node::nameChanged,
                                                 [this](QString) { reevaluate_connections(); })));
    } else {
      static QStringList const connection_types { "yoyo::properties::connection_t",
                                                  "yoyo::properties::in_connection_t",
                                                  "yoyo::properties::out_connection_t" };
      std::vector<std::shared_ptr<single_connection_tracker>> connections;

      for (int i = node_base::staticMetaObject.propertyCount(); i < meta->propertyCount(); i++) {
        if (connection_types.contains(meta->property(i).typeName())) {
          connections.push_back(std::make_shared<single_connection_tracker>(n, i, _data_root));
        }
      }

      if (!connections.empty()) {
        _nodes.insert(std::make_pair(n.get(), connections));
      }
    }

    for (auto& child : *n) {
      on_node_add(child);
    }
  }
}

auto connectivity_manager::impl::on_node_remove(std::weak_ptr<node_base> node) -> void
{
  if (auto n = node.lock()) {
    auto it = _nodes.find(n.get());

    if (it != _nodes.end()) {
      _nodes.erase(it);
    }
  }
}

auto connectivity_manager::impl::reevaluate_connections() -> void
{
  std::for_each(_nodes.begin(), _nodes.end(), [](auto n) {
    std::for_each(n.second.begin(), n.second.end(), [](auto c) { c->on_connection_changed(); });
  });
}

// auto connectivity_manager::impl::connection_changed(int propertyIndex, std::weak_ptr<node_base>
// to_node)->void
//{
//    auto n = to_node.lock();

//    if (!n)
//    {
//        return;
//    }

//    auto it = _nodes.find(n.get());

//    if (it == _nodes.end())
//    {
//        return;
//    }

//    auto property = n->metaObject()->property(propertyIndex);
//    auto value    = n->property(property.name());
//}

connectivity_manager::connectivity_manager(std::shared_ptr<node_base> root)
  : _p { std::make_unique<impl>(root) }
{
}

connectivity_manager::~connectivity_manager() = default;
} // namespace yoyo
#include "connectivity_manager.moc"
