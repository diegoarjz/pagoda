#include "parameter_node.h"

namespace selector
{
const char *name = "ParameterNode";

ParameterNode::ParameterNode() {}

ParameterNode~ParameterNode() {}

void ParameterNodeExecute(GraphExecutionContextPtr graph, const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes)
{
    START_PROFILE;
    
    auto parameterContext = GetParameterContext();
    for (const auto& parameter : parameterContext->GetParameters())
    {
        for (const auto& outNode : outNodes)
        {
            DBG_ASSERT_MSG(outNode->GetParameterContext() != nullptr, "Out node doesn't have a ParameterContext set.");
            outNode->GetParameterContext()->SetParameter(parameter.first, parameter.second);
        }
    }
}
}