#include "parameter_node.h"

#include "common/assertions.h"
#include "common/profiler.h"

namespace selector
{
const char *ParameterNode::name = "Parameter";

ParameterNode::ParameterNode() {}

ParameterNode::~ParameterNode() {}

void ParameterNode::SetConstructionArguments(
    const std::unordered_map<std::string, DynamicValueBasePtr> &constructionArgs)
{
}

void ParameterNode::Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes)
{
	START_PROFILE;

	for (const auto &parameter : GetMembers())
	{
		for (const auto &outNode : outNodes)
		{
			outNode->SetMember(parameter.first, parameter.second);
		}
	}
}
}  // namespace selector
