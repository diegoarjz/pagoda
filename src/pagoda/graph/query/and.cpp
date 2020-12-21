#include "and.h"
#include <algorithm>
#include <iterator>
#include <memory>

namespace pagoda::graph::query
{
And::And(QueryContainer_t &&queries) { m_subQueries.swap(queries); }

And::And(Graph &graph, QueryHandle_t queryHandle) : Query(graph, queryHandle) {}

And::And(Graph &graph, NodeSet &nodeSet) : Query(graph, nodeSet) {}

And::And(Graph &graph, QueryHandle_t queryHandle, QueryContainer_t &&queries) : Query(graph, queryHandle)
{
	m_subQueries.swap(queries);
}
And::And(Graph &graph, NodeSet &nodeSet, QueryContainer_t &&queries) : Query(graph, nodeSet)
{
	m_subQueries.swap(queries);
}

And::~And() {}

bool And::matches(NodePtr n)
{
	bool matches = std::all_of(m_subQueries.begin(), m_subQueries.end(), [&](std::shared_ptr<Query> q) {
		q->SetGraph(m_graph);
		return q->Matches(n);
	});
	return matches;
}

void And::AppendToString(std::stringstream &os, uint32_t indent) const
{
	os << "And[" << std::endl;
	for (auto q : m_subQueries) {
		q->AppendToString(os, indent + 1);
		os << std::endl;
	}
	os << "]";
}

}  // namespace pagoda::graph::query
