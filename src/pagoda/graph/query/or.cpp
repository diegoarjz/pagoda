#include "or.h"
#include <algorithm>
#include <iterator>
#include <memory>

namespace pagoda::graph::query
{
Or::Or(Graph &graph, QueryHandle_t queryHandle) : Query(graph, queryHandle) {}
Or::Or(Graph &graph, NodeSet &nodeSet) : Query(graph, nodeSet) {}
Or::Or(Graph &graph, QueryHandle_t queryHandle, QueryContainer_t &&queries)
  : Query(graph, queryHandle), m_queries(std::move(queries))
{
}
Or::Or(Graph &graph, NodeSet &nodeSet, QueryContainer_t &&queries)
  : Query(graph, nodeSet), m_queries(std::move(queries))
{
}

Or::~Or() {}

bool Or::matches(NodePtr n)
{
	return std::any_of(m_queries.begin(), m_queries.end(), [&](std::shared_ptr<Query> q) {
		q->SetGraph(m_graph);
		return q->Matches(n);
	});
}

void Or::AppendToString(std::stringstream &os, uint32_t indent) const
{
	os << "Or[" << std::endl;
	for (auto q : m_queries) {
		q->AppendToString(os, indent + 1);
		os << std::endl;
	}
	os << "]";
}
}  // namespace pagoda::graph::query

