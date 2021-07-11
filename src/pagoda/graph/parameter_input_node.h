#pragma once

#include "node.h"

namespace pagoda::graph
{
class ParameterInputNode : public Node
{
	public:
	ParameterInputNode();
	~ParameterInputNode() override;

	void SetConstructionArguments(ConstructionArgumentCallback* cb) override;
	void SetExecutionArguments(ExecutionArgumentCallback* cb) override;

	void Execute(const NodeSet& inNodes, const NodeSet& outNodes) override;
	void ForEachConstructionArgument(std::function<void(const std::string&, dynamic::DynamicValueBasePtr)> f) override;
	void ForEachExecutionArgument(std::function<void(const std::string&, dynamic::DynamicValueBasePtr)> f) override;

	const std::string& GetParameterName() const;

	const char* const GetNodeType() override;

	private:
	std::string m_parameterName;
};
}  // namespace pagoda::graph
