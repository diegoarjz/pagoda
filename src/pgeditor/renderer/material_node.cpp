#include "material_node.h"

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
MaterialNode::Input* MaterialNode::GetInput(const std::string& name)
{
	auto iter = m_inputs.find(name);
	if (iter == m_inputs.end()) {
		return nullptr;
	}
	return &iter->second;
}
const MaterialNode::Input* MaterialNode::GetInput(const std::string& name) const
{
	auto iter = m_inputs.find(name);
	if (iter == m_inputs.end()) {
		return nullptr;
	}
	return &iter->second;
}

void MaterialNode::SetOutput(const std::string& name, const Output& connection) { m_outputs[name] = connection; }
MaterialNode::Output* MaterialNode::GetOutput(const std::string& name)
{
	auto iter = m_outputs.find(name);
	if (iter == m_outputs.end()) {
		static const Output empty{};
		return nullptr;
	}
	return &iter->second;
}
const MaterialNode::Output* MaterialNode::GetOutput(const std::string& name) const
{
	auto iter = m_outputs.find(name);
	if (iter == m_outputs.end()) {
		static const Output empty{};
		return nullptr;
	}
	return &iter->second;
}

void MaterialNode::SetParameter(const std::string& name, const Parameter_t& par) { m_parameters[name] = par; }

const MaterialNode::Parameter_t* MaterialNode::GetParameter(const std::string& name) const
{
	auto iter = m_parameters.find(name);
	if (iter == m_parameters.end()) {
		return nullptr;
	}
	return &iter->second;
}

}  // namespace pgeditor::renderer
