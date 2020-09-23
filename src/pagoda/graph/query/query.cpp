#include "query.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

namespace pagoda::graph::query
{
Query::Query() : m_graph{nullptr} {}
Query::Query(Graph& graph, QueryHandle_t queryHandle) : m_graph(&graph), m_queryHandle(queryHandle) {}
Query::Query(Graph& graph, NodeSet& nodeSet)
  : m_graph(&graph), m_queryHandle([&nodeSet](NodePtr n) { nodeSet.insert(n); })
{
}

Query::~Query() {}

bool Query::Matches(NodePtr n) { return true; }
void Query::AddNode(NodePtr n) { m_queryHandle(n); }

void Query::SetGraph(Graph* graph) { m_graph = graph; }
Graph* Query::GetGraph() const { return m_graph; }
Query::QueryHandle_t& Query::GetQueryHandle() { return m_queryHandle; }

const std::string& Query::GetQueryHash() const
{
	static const std::string hash("query");
	return hash;
}
}  // namespace pagoda::graph::query
