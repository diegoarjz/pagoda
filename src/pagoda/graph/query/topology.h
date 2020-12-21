#pragma once

#include <map>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace pagoda::graph
{
class Graph;
class Node;
using NodePtr = std::shared_ptr<Node>;
}  // namespace pagoda::graph

namespace pagoda::graph::query
{
class Query;

/**
 * Implements a topology of \c Query objects so that complex queries based
 * adjacencies between nodes are possible.
 *
 * This is used in \c Rule.
 */
class QueryTopology
{
	public:
	QueryTopology(Query* startQuery);

	void AddDownstreamQuery(Query* upstream, Query* downstream);
	void AddUpstreamQuery(Query* downstream, Query* upstream);

	template<class T>
	void GetDownstreamQueriesOf(Query* q, T inserter)
	{
		auto iter = m_downstreamAdjacencies.find(q);
		if (iter != m_downstreamAdjacencies.end()) {
			std::copy(iter->second.begin(), iter->second.end(), inserter);
		}
	}

	template<class T>
	void GetUpstreamQueriesOf(Query* q, T inserter)
	{
		auto iter = m_upstreamAdjacencies.find(q);
		if (iter != m_upstreamAdjacencies.end()) {
			std::copy(iter->second.begin(), iter->second.end(), inserter);
		}
	}

	const std::unordered_set<Query*>& GetQueries() const;

	using Match_t = std::map<Query*, NodePtr>;

	void Match(Graph* graph, std::vector<Match_t>& matches);

	private:
	std::unordered_set<Query*> m_queries;
	std::unordered_map<Query*, std::unordered_set<Query*>> m_downstreamAdjacencies;
	std::unordered_map<Query*, std::unordered_set<Query*>> m_upstreamAdjacencies;
};

}  // namespace pagoda::graph::query
