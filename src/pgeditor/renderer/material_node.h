#pragma once

#include "types.h"

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>

#include <boost/functional/hash.hpp>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>

namespace pgeditor::renderer
{
class MaterialNode
{
	public:
	using Parameter_t = std::variant<int, float, double, pagoda::math::Vec2F, pagoda::math::Vec3F, pagoda::math::Vec4F,
	                                 pagoda::math::Mat2x2F, pagoda::math::Mat3x3F, pagoda::math::Mat4x4F, std::string>;

	struct Input
	{
		Input() = default;

		Input(const std::string& name, const Type type) : m_name{name}, m_type{type} {}

		void AppendHash(std::size_t& hash) const
		{
			boost::hash_combine(hash, m_name);
			boost::hash_combine(hash, m_type);
			boost::hash_combine(hash, m_upstreamNode);
			boost::hash_combine(hash, m_upstreamOutput);
		}

		std::string m_name;
		Type m_type;
		std::string m_upstreamNode;
		std::string m_upstreamOutput;
		Parameter_t m_defaultValue;
	};

	struct Output
	{
    std::string m_name;
    Type m_type;
	};

	using InputContainer_t = std::unordered_map<std::string, Input>;
	using OutputContainer_t = std::unordered_map<std::string, Output>;

	MaterialNode(const std::string& nodeId, const std::string& name);
	MaterialNode(MaterialNode&& node);
	~MaterialNode();

	MaterialNode(const MaterialNode&) = delete;
	MaterialNode& operator=(const MaterialNode&) = delete;

	const std::string& GetName() const { return m_name; }
	const std::string& GetId() const { return m_id; }

	void SetInput(const std::string& name, const Input& connection);
	Input* GetInput(const std::string& name);
	const Input* GetInput(const std::string& name) const;
	const InputContainer_t& GetInputs() const { return m_inputs; }

	void SetOutput(const std::string& name, const Output& connection);
	Output* GetOutput(const std::string& name);
	const Output* GetOutput(const std::string& name) const;
	const OutputContainer_t& GetOutputs() const { return m_outputs; }

	void SetParameter(const std::string& name, const Parameter_t& par);
	const Parameter_t* GetParameter(const std::string& name) const;

	private:
	std::string m_name;
	std::string m_id;
	InputContainer_t m_inputs;
	OutputContainer_t m_outputs;
	std::unordered_map<std::string, Parameter_t> m_parameters;
};
}  // namespace pgeditor::renderer
