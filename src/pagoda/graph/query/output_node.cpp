#include "output_node.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

namespace pagoda::graph::query
{
OutputNode::OutputNode(QueryHandle_t queryHandle) : Query(queryHandle) {}
OutputNode::OutputNode(NodeSet &nodeSet) : Query(nodeSet) {}

bool OutputNode::Matches(NodePtr n) { return m_graph->GetNodeOutputNodes(n->GetName()).size() == 0; }

}  // namespace pagoda::graph::query

