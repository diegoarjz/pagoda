#include "parameter_node.h"

#include "common/assertions.h"

namespace selector
{
const char *ParameterNode::name = "ParameterNode";

ParameterNode::ParameterNode() {}

ParameterNode::~ParameterNode() {}

void ParameterNode::SetConstructionArguments(const std::unordered_map<std::string, Parameter> &constructionArgs) {}

void ParameterNode::Execute(GraphExecutionContextPtr ctx, const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes)
{
	START_PROFILE;

	auto parameterContext = GetParameterContext();
	for (const auto &parameter : parameterContext->GetParameters())
	{
		for (const auto &outNode : outNodes)
		{
			DBG_ASSERT_MSG(outNode->GetParameterContext() != nullptr, "Out node doesn't have a ParameterContext set.");
			outNode->GetParameterContext()->SetParameter(parameter.first, parameter.second);
		}
	}
}
}  // namespace selector
