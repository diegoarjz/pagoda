#include "and.h"
#include <algorithm>
#include <iterator>
#include <memory>

namespace pagoda::graph::query
{
And::And(Graph &graph, QueryHandle_t queryHandle) : Query(graph, queryHandle) {}
And::And(Graph &graph, NodeSet &nodeSet) : Query(graph, nodeSet) {}
And::And(Graph &graph, QueryHandle_t queryHandle, QueryContainer_t &&queries)
  : Query(graph, queryHandle), m_queries(std::move(queries))
{
}
And::And(Graph &graph, NodeSet &nodeSet, QueryContainer_t &&queries)
  : Query(graph, nodeSet), m_queries(std::move(queries))
{
}

And::~And() {}

bool And::Matches(NodePtr n)
{
	return std::all_of(m_queries.begin(), m_queries.end(), [&](std::shared_ptr<Query> q) {
		q->SetGraph(m_graph);
		return q->Matches(n);
	});
}
}  // namespace pagoda::graph::query
