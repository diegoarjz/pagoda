#pragma once

#include "pagoda/graph/node.h"

namespace pagoda::graph
{
class ParameterInputNode : public Node
{
	public:
	ParameterInputNode();
	~ParameterInputNode() override;

	void Execute(const NodeSet& inNodes, const NodeSet& outNodes) override;
	const std::string& GetParameterName() const;

	const char* const GetNodeType() override;

	private:
	std::string m_parameterName;
};
}  // namespace pagoda::graph
