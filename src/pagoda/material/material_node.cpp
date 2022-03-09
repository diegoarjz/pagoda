#include "material_node.h"

#include <boost/functional/hash.hpp>

using namespace pagoda::math;

namespace pagoda::material {
MaterialNode::MaterialNode(const std::string &nodeId) : m_id{nodeId} {}

MaterialNode::MaterialNode(const std::string &nodeId, const std::string &name)
    : m_name{name}, m_id{nodeId} {}

MaterialNode::MaterialNode(MaterialNode &&node)
    : m_name{std::move(node.m_name)}, m_id{std::move(node.m_id)},
      m_inputs{std::move(node.m_inputs)}, m_outputs{std::move(node.m_outputs)} {
}

MaterialNode::~MaterialNode() {}

////////////////////////////////////////
/// \name Inputs
////////////////////////////////////////
const MaterialNode::Input *
MaterialNode::GetInput(const std::string &name) const {
  auto iter = m_inputs.find(name);
  if (iter == m_inputs.end()) {
    return nullptr;
  }
  return &iter->second;
}

void MaterialNode::Inputs(std::function<void(const Input &)> f) const {
  for (const auto &i : m_inputs) {
    f(i.second);
  }
}

const MaterialNode::Type_t
MaterialNode::InputType(const std::string &name) const {
  const auto input = GetInput(name);
  if (input != nullptr) {
    return input->type;
  }
  return MaterialNode::Type_t::Invalid;
}

const MaterialNode::Value_t &
MaterialNode::InputDefaultValue(const std::string &name) const {
  const auto input = GetInput(name);
  if (input != nullptr) {
    return input->defaultValue;
  }
  static const Value_t sInvalid;
  return sInvalid;
}

bool MaterialNode::HasInput(const std::string &name) const {
  return GetInput(name) != nullptr;
}

////////////////////////////////////////
/// \name Outputs
////////////////////////////////////////
const MaterialNode::Output *
MaterialNode::GetOutput(const std::string &name) const {
  auto iter = m_outputs.find(name);
  if (iter == m_outputs.end()) {
    static const Output empty{};
    return nullptr;
  }
  return &iter->second;
}

void MaterialNode::Outputs(std::function<void(const Output &)> f) const {
  for (const auto &o : m_outputs) {
    f(o.second);
  }
}

const MaterialNode::Type_t MaterialNode::OutputType(const std::string &name) {
  const auto output = GetOutput(name);
  if (output != nullptr) {
    return output->type;
  }
  return MaterialNode::Type_t::Invalid;
}

bool MaterialNode::HasOutput(const std::string &name) const {
  return GetOutput(name) != nullptr;
}

////////////////////////////////////////
/// \name Connections
////////////////////////////////////////
bool MaterialNode::ConnectInput(const std::string &inputName,
                                const MaterialNodePtr &node,
                                const std::string &upstreamName) {
  if (!CanConnectInput(inputName, node, upstreamName)) {
    return false;
  }

  m_connections[inputName] = Connection{inputName, node, upstreamName};

  return true;
}

bool MaterialNode::CanConnectInput(const std::string &inputName,
                                   const MaterialNodePtr &node,
                                   const std::string &upstreamName) {
  if (node == nullptr || !node->HasOutput(upstreamName)) {
    return false;
  }

  if (InputType(inputName) != node->OutputType(upstreamName)) {
    return false;
  }

  return true;
}

void MaterialNode::DisconnectInput(const std::string &inputName) {
  m_connections.erase(inputName);
}

const MaterialNode::ConnectionContainer_t &
MaterialNode::GetConnections() const {
  return m_connections;
}

void MaterialNode::Connections(
    std::function<void(const Connection &)> f) const {
  for (const auto &c : m_connections) {
    f(c.second);
  }
}

bool MaterialNode::IsInputConnected(const std::string &inputName) const {
  return m_connections.find(inputName) != m_connections.end();
}

////////////////////////////////////////
/// \name Parameters
////////////////////////////////////////
const MaterialNode::Parameter *
MaterialNode::GetParameter(const std::string &name) const {
  auto iter = m_parameters.find(name);
  if (iter == m_parameters.end()) {
    return nullptr;
  }
  return &iter->second;
}

void MaterialNode::Parameters(std::function<void(const Parameter &p)> f) const {
  for (const auto &p : m_parameters) {
    f(p.second);
  }
}

bool MaterialNode::IsParameterDefault(const std::string &name) const {
  if (const auto parameter = GetParameter(name)) {
    return parameter->value == parameter->defaultValue;
  }
  return false;
}

const MaterialNode::Type_t
MaterialNode::ParameterType(const std::string &name) const {
  if (const auto parameter = GetParameter(name)) {
    return parameter->type;
  }
  return Type_t::Invalid;
}

const common::Value &
MaterialNode::ParameterValue(const std::string &name) const {
  if (const auto parameter = GetParameter(name)) {
    // if value is set return it. otherwise return the default value.
    return parameter->value.index() == std::variant_npos
               ? parameter->defaultValue
               : parameter->value;
  }
  static const Value_t sEmptyValue;
  return sEmptyValue;
}

const common::Value &
MaterialNode::ParameterDefaultValue(const std::string &name) const {
  if (const auto parameter = GetParameter(name)) {
    return parameter->defaultValue;
  }
  static const Value_t sEmptyValue;
  return sEmptyValue;
}

void MaterialNode::ResetParameter(const std::string &name) {
  auto iter = m_parameters.find(name);
  if (iter != m_parameters.end()) {
    auto &parameter = iter->second;
    parameter.value = parameter.defaultValue;
  }
}

////////////////////////////////////////
/// \name Hints
////////////////////////////////////////
const MaterialNode::Hints_t &MaterialNode::GetHints() const { return m_hints; }

////////////////////////////////////////
/// \name Hash
////////////////////////////////////////
void MaterialNode::AppendHash(std::size_t &hash) const {
  struct {
    void operator()(const int &v) { boost::hash_combine(m_hash, v); }
    void operator()(const float &v) { boost::hash_combine(m_hash, v); }
    void operator()(const double &v) { boost::hash_combine(m_hash, v); }
    void operator()(const Vec2F &v) { boost::hash_combine(m_hash, v.a); }
    void operator()(const Vec3F &v) { boost::hash_combine(m_hash, v.a); }
    void operator()(const Vec4F &v) { boost::hash_combine(m_hash, v.a); }
    void operator()(const Mat2x2F &v) { boost::hash_combine(m_hash, v.a); }
    void operator()(const Mat3x3F &v) { boost::hash_combine(m_hash, v.a); }
    void operator()(const Mat4x4F &v) { boost::hash_combine(m_hash, v.a); }
    void operator()(const std::string &v) { boost::hash_combine(m_hash, v); }

    std::size_t &m_hash;
  } paramHash{hash};

  boost::hash_combine(hash, m_name);
  boost::hash_combine(hash, m_id);
  for (const auto &i : m_inputs) {
    const auto &input = i.second;
    boost::hash_combine(hash, input.name);
    boost::hash_combine(hash, input.type);
    std::visit(paramHash, input.defaultValue);
  }

  for (const auto &c : m_connections) {
    boost::hash_combine(hash, c.second.input);
    boost::hash_combine(hash, c.second.upstreamNode);
    boost::hash_combine(hash, c.second.upstreamOutput);
  }

  for (const auto &o : m_outputs) {
    const auto &output = o.second;
    boost::hash_combine(hash, output.name);
    boost::hash_combine(hash, output.type);
  }

  for (const auto &p : m_parameters) {
    const auto &parameter = p.second;
    boost::hash_combine(hash, parameter.name);
    std::visit(paramHash, parameter.defaultValue);
    std::visit(paramHash, parameter.value);
  }
}
} // namespace pagoda::material
