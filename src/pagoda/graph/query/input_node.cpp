#include "input_node.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

namespace pagoda::graph::query
{
InputNode::InputNode() {}
InputNode::InputNode(Graph &graph, QueryHandle_t queryHandle) : Query(graph, queryHandle) {}
InputNode::InputNode(Graph &graph, NodeSet &nodeSet) : Query(graph, nodeSet) {}

bool InputNode::matches(NodePtr n)
{
	if (m_graph->GetNodeInputNodes(n->GetName()).size() == 0) {
		return true;
	}
	return false;
}

void InputNode::AppendToString(std::stringstream &os, uint32_t indent) const
{
	os << std::string(indent, ' ') << "InputNode";
}
}  // namespace pagoda::graph::query
