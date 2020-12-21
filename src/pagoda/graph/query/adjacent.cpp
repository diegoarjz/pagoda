#include "adjacent.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

#include <iostream>

namespace pagoda::graph::query
{
Adjacent::Adjacent(std::shared_ptr<Query> &&query, int32_t direction) : m_direction(direction)
{
	m_subQueries.push_back(std::move(query));
}

Adjacent::Adjacent(Graph &graph, QueryHandle_t queryHandle, std::shared_ptr<Query> &&query, int32_t direction)
  : Query(graph, queryHandle), m_direction(direction)
{
	m_subQueries.push_back(std::move(query));
}

Adjacent::Adjacent(Graph &graph, NodeSet &nodeSet, std::shared_ptr<Query> &&query, int32_t direction)
  : Query(graph, nodeSet), m_direction(direction)
{
	m_subQueries.push_back(std::move(query));
}

Adjacent::~Adjacent() {}

void Adjacent::AppendToString(std::stringstream &os, uint32_t indent) const
{
	os << std::string(indent, ' ');
	if (m_direction == 0) {
		os << "Adjacent(";
	} else if (m_direction > 0) {
		os << "Downstream(";
	} else if (m_direction < 0) {
		os << "Upstream(";
	}
	os << m_direction << ")[";
	m_subQueries[0]->AppendToString(os, 0);
	os << "]";
}

namespace
{
void collectOutNodes(Graph *graph, NodePtr n, int32_t depth, std::insert_iterator<NodeSet> nodes)
{
	if (depth < 0) {
		return;
	}
	for (auto node : graph->GetNodeOutputNodes(n->GetName())) {
		nodes = node;
		collectOutNodes(graph, node, depth - 1, nodes);
	}
}
void collectInNodes(Graph *graph, NodePtr n, int32_t depth, std::insert_iterator<NodeSet> nodes)
{
	if (depth > 0) {
		return;
	}
	for (auto node : graph->GetNodeInputNodes(n->GetName())) {
		nodes = node;
		collectInNodes(graph, node, depth + 1, nodes);
	}
}
}  // namespace

bool Adjacent::matches(NodePtr n)
{
	m_subQueries[0]->SetGraph(m_graph);
	NodeSet nodes;
	collectOutNodes(m_graph, n, std::max(0, m_direction - 1), std::inserter(nodes, nodes.end()));
	collectInNodes(m_graph, n, std::min(0, m_direction + 1), std::inserter(nodes, nodes.end()));

	for (auto adjacent : nodes) {
		if (m_subQueries[0]->Matches(adjacent)) {
			return true;
		}
	}
	return false;
}
}  // namespace pagoda::graph::query
