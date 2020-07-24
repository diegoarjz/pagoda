#include "input_node.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

namespace pagoda::graph::query
{
InputNode::InputNode(Graph &graph, QueryHandle_t queryHandle) : Query(graph, queryHandle) {}
InputNode::InputNode(Graph &graph, NodeSet &nodeSet) : Query(graph, nodeSet) {}

bool InputNode::Matches(NodePtr n) { return m_graph.GetNodeInputNodes(n->GetName()).size() == 0; }

}  // namespace pagoda::graph::query
