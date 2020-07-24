#include "input_node.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

namespace pagoda::graph::query
{
InputNode::InputNode(QueryHandle_t queryHandle) : Query(queryHandle) {}
InputNode::InputNode(NodeSet &nodeSet) : Query(nodeSet) {}

bool InputNode::Matches(NodePtr n) { return m_graph->GetNodeInputNodes(n->GetName()).size() == 0; }

}  // namespace pagoda::graph::query
