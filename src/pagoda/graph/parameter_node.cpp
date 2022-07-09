#include "pagoda/graph/parameter_node.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

using namespace pagoda::dynamic;

namespace pagoda::graph
{
const char *ParameterNode::name = "Parameter";

ParameterNode::ParameterNode()
{
}

ParameterNode::~ParameterNode()
{
}

void ParameterNode::Execute(const NodeSet &inNodes, const NodeSet &outNodes)
{
	START_PROFILE;
	LOG_TRACE(ProceduralGraph,
	          "Executing ParameterNode " << GetName() << "(" << GetId() << ")");

	for (auto parIter = GetMembersBegin(); parIter != GetMembersEnd();
	     ++parIter) {
		for (const auto &outNode : outNodes) {
			outNode->RegisterOrSetMember(parIter->first, parIter->second.m_value);
		}
	}
}

const char *const ParameterNode::GetNodeType()
{
	static const char *const sNodeType = "Parameter";
	return sNodeType;
}

}  // namespace pagoda::graph
