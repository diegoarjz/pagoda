#include "output_node.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

namespace pagoda::graph::query
{
OutputNode::OutputNode() {}
OutputNode::OutputNode(Graph &graph, QueryHandle_t queryHandle) : Query(graph, queryHandle) {}
OutputNode::OutputNode(Graph &graph, NodeSet &nodeSet) : Query(graph, nodeSet) {}

bool OutputNode::matches(NodePtr n)
{
	if (m_graph->GetNodeOutputNodes(n->GetName()).size() == 0) {
		return true;
	}
	return false;
}

void OutputNode::AppendToString(std::stringstream &os, uint32_t indent) const
{
	os << std::string(indent, ' ') << "OutputNode";
}

}  // namespace pagoda::graph::query
