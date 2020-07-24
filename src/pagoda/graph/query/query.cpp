#include "query.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

namespace pagoda::graph::query
{
Query::Query(QueryHandle_t queryHandle) : m_queryHandle(queryHandle) {}
Query::Query(NodeSet& nodeSet) : m_queryHandle([&nodeSet](NodePtr n) { nodeSet.insert(n); }) {}

void Query::Start(Graph* graph) { m_graph = graph; }
bool Query::Matches(NodePtr n) { return true; }
void Query::AddNode(NodePtr n) { m_queryHandle(n); }

const std::string& Query::GetQueryHash() const
{
	static const std::string hash("query");
	return hash;
}
}  // namespace pagoda::graph::query
