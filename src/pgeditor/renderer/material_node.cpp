#include "material_node.h"

#include <boost/functional/hash.hpp>

using namespace pagoda::math;

namespace pgeditor::renderer
{
MaterialNode::MaterialNode(const std::string& nodeId, const std::string& name) : m_name{name}, m_id{nodeId} {}
MaterialNode::MaterialNode(MaterialNode&& node)
  : m_name{std::move(node.m_name)}, m_id{std::move(node.m_id)}, m_inputs{std::move(node.m_inputs)}, m_outputs{std::move(
                                                                                                      node.m_outputs)}
{
}

MaterialNode::~MaterialNode() {}

void MaterialNode::SetInput(const std::string& name, const Input& connection) { m_inputs[name] = connection; }
const MaterialNode::Input* MaterialNode::GetInput(const std::string& name) const
{
	auto iter = m_inputs.find(name);
	if (iter == m_inputs.end()) {
		return nullptr;
	}
	return &iter->second;
}

void MaterialNode::SetOutput(const std::string& name, const Output& connection) { m_outputs[name] = connection; }
const MaterialNode::Output* MaterialNode::GetOutput(const std::string& name) const
{
	auto iter = m_outputs.find(name);
	if (iter == m_outputs.end()) {
		static const Output empty{};
		return nullptr;
	}
	return &iter->second;
}

bool MaterialNode::ConnectInput(const std::string& inputName, const MaterialNode* node, const std::string& upstreamName)
{
  if (node == nullptr) {
    return false;
  }
  auto inIter = m_inputs.find(inputName);
  auto outIter = node->m_outputs.find(upstreamName);
  if (inIter == m_inputs.end() || outIter == node->m_outputs.end()) {
    return false;
  }
  if (inIter->second.type != outIter->second.type) {
    return false;
  }
  inIter->second.upstreamNode = node->GetName();
  inIter->second.upstreamOutput = upstreamName;
  return true;
}

void MaterialNode::SetParameter(const std::string& name, const Value& par) { m_parameters[name] = par; }

const Value* MaterialNode::GetParameter(const std::string& name) const
{
	auto iter = m_parameters.find(name);
	if (iter == m_parameters.end()) {
		return nullptr;
	}
	return &iter->second;
}

void MaterialNode::AppendHash(std::size_t& hash) const
{
  struct
  {
    void operator()(const int& v) { boost::hash_combine(m_hash, v); }
    void operator()(const float& v) { boost::hash_combine(m_hash, v); }
    void operator()(const double& v) { boost::hash_combine(m_hash, v); }
    void operator()(const Vec2F& v) { boost::hash_combine(m_hash, v.a); }
    void operator()(const Vec3F& v) { boost::hash_combine(m_hash, v.a); }
    void operator()(const Vec4F& v) { boost::hash_combine(m_hash, v.a); }
    void operator()(const Mat2x2F& v) { boost::hash_combine(m_hash, v.a); }
    void operator()(const Mat3x3F& v) { boost::hash_combine(m_hash, v.a); }
    void operator()(const Mat4x4F& v) { boost::hash_combine(m_hash, v.a); }
    void operator()(const std::string& v) { boost::hash_combine(m_hash, v); }

    std::size_t& m_hash;
  } paramHash{hash};

  boost::hash_combine(hash, m_name);
  boost::hash_combine(hash, m_id);
  for (const auto& i : m_inputs) {
    const auto& input = i.second;
    boost::hash_combine(hash, input.name);
    boost::hash_combine(hash, input.type);
    boost::hash_combine(hash, input.upstreamNode);
    boost::hash_combine(hash, input.upstreamOutput);
    std::visit(paramHash, input.defaultValue);
  }

  for (const auto& o : m_outputs) {
    const auto& output = o.second;
    boost::hash_combine(hash, output.name);
    boost::hash_combine(hash, output.type);
  }

  for (const auto& p : m_parameters) {
    const auto& parameter = p.second;
    boost::hash_combine(hash, p.first);
    std::visit(paramHash, parameter);
  }
}
}  // namespace pgeditor::renderer
