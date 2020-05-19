#include "parameter_node.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

#include "node_visitor.h"

using namespace pagoda::dynamic;

namespace pagoda
{
const char *ParameterNode::name = "Parameter";

ParameterNode::ParameterNode() {}

ParameterNode::~ParameterNode() {}

void ParameterNode::SetConstructionArguments(
    const std::unordered_map<std::string, DynamicValueBasePtr> &constructionArgs)
{
}

void ParameterNode::AcceptNodeVisitor(NodeVisitor *visitor)
{
	visitor->Visit(std::dynamic_pointer_cast<ParameterNode>(shared_from_this()));
}

void ParameterNode::Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes)
{
	START_PROFILE;
	LOG_TRACE(ProceduralGraph, "Executing ParameterNode " << GetName() << "(" << GetId() << ")");

	for (auto parIter = GetMembersBegin(); parIter != GetMembersEnd(); ++parIter)
	{
		for (const auto &outNode : outNodes)
		{
			outNode->RegisterOrSetMember(parIter->first, parIter->second.m_value);
		}
	}
}
}  // namespace pagoda
