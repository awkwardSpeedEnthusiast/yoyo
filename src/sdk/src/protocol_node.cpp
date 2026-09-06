#include "yoyo/protocol_node.h"

#include <QVariant>

namespace yoyo
{
struct protocol_node::impl {
  impl(properties::connection_t o, properties::connection_t i)
    : _output { std::move(o) }
    , _input { std::move(i) }
  {
  }

  properties::connection_t _output;
  properties::connection_t _input;
  boost::signals2::signal<void(QVariant)> _output_changed;
  boost::signals2::signal<void(QVariant)> _input_changed;
};

protocol_node::protocol_node(QString type, boost::uuids::uuid identifier)
  : protocol_node_base(std::move(type), std::move(identifier))
  , _p(std::make_unique<impl>(
      properties::connection_t { [this](std::function<void(QVariant)> subscriber) {
                                  return _p->_output_changed.connect(subscriber);
                                },
                                 [this](QVariant v) { deserialize(v); },
                                 [this](types::value_t t) { return outputSupports(t); },
                                 {},
                                 {},
                                 {} },
      properties::connection_t { [this](std::function<void(QVariant)> subscriber) {
                                  return _p->_input_changed.connect(subscriber);
                                },
                                 [this](QVariant v) { serialize(v); },
                                 [this](types::value_t t) { return inputSupports(t); },
                                 {},
                                 {},
                                 {} }))
{
}

protocol_node::~protocol_node() = default;

auto protocol_node::output() const -> yoyo::properties::connection_t
{
  return _p->_output;
}

auto protocol_node::input() const -> yoyo::properties::connection_t
{
  return _p->_input;
}

auto protocol_node::setOutput(yoyo::properties::connection_t out) -> void
{
  if (out == _p->_output) {
    return;
  }

  _p->_output = out;
  Q_EMIT outputChanged(_p->_output);
}

auto protocol_node::setInput(yoyo::properties::connection_t in) -> void
{
  if (in == _p->_input) {
    return;
  }

  _p->_input = in;
  Q_EMIT inputChanged(_p->_input);
}

auto protocol_node::notify_value_on_input_changed(QVariant v) -> void
{
  _p->_input_changed(v);
}

auto protocol_node::notify_value_on_output_changed(QVariant v) -> void
{
  _p->_output_changed(v);
}
} // namespace yoyo
