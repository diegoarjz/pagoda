#include "topology.h"

#include "query.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/node_set.h>

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/instrument/profiler.h>

#include <boost/container_hash/hash.hpp>

#include <stack>

namespace pagoda::graph::query
{
QueryTopology::QueryTopology(Query* startQuery) : m_queries{startQuery} {}

void QueryTopology::AddDownstreamQuery(Query* upstream, Query* downstream)
{
	if (m_queries.find(upstream) == m_queries.end()) {
		// TODO: Exception / Error
		return;
	}
	m_downstreamAdjacencies[upstream].insert(downstream);
	m_upstreamAdjacencies[downstream].insert(upstream);
	m_queries.insert(downstream);
}

void QueryTopology::AddUpstreamQuery(Query* downstream, Query* upstream)
{
	if (m_queries.find(downstream) == m_queries.end()) {
		// TODO: Exception / Error
		return;
	}
	m_downstreamAdjacencies[upstream].insert(downstream);
	m_upstreamAdjacencies[downstream].insert(upstream);
	m_queries.insert(upstream);
}

const std::unordered_set<Query*>& QueryTopology::GetQueries() const { return m_queries; }

namespace
{
struct MatchHasher
{
	using Match_t = typename QueryTopology::Match_t;

	std::size_t operator()(const Match_t& match) const
	{
		std::size_t hash = 0;
		for (const auto& m : match) {
			boost::hash_combine(hash, m.first);
			boost::hash_combine(hash, m.second);
		}
		return hash;
	}

	bool operator()(const Match_t& lhs, const Match_t& rhs) const
	{
		if (lhs.size() != rhs.size()) {
			return false;
		}
		for (const auto& l : lhs) {
			auto r = rhs.find(l.first);
			if (r == rhs.end()) {
				return false;
			}
			if (l.second != r->second) {
				return false;
			}
		}
		return true;
	}
};

struct MatchTree
{
	MatchTree(Query* q, NodePtr n, MatchTree* p = nullptr) : m_query(q), m_node(n), m_parent(p) {}

	void CreateChild(Query* q, NodePtr n)
	{
		if (m_availableQueries.find(q) != m_availableQueries.end()) {
			auto& child = m_childMatches.emplace_back(q, n, this);
			child.m_availableQueries = m_availableQueries;
			child.m_availableQueries.erase(q);
			DBG_ASSERT(child.m_availableQueries.size() == m_availableQueries.size() - 1);
		}
	}

	void CollectMatches(std::unordered_set<typename QueryTopology::Match_t, MatchHasher, MatchHasher>& matches)
	{
		if (IsComplete() && m_childMatches.empty()) {
			QueryTopology::Match_t match;
			auto currentMatch = this;
			while (currentMatch->m_parent != nullptr) {
				match[currentMatch->m_query] = currentMatch->m_node;
				currentMatch = currentMatch->m_parent;
			}
			matches.insert(match);
		}
		for (auto& m : m_childMatches) {
			m.CollectMatches(matches);
		}
	}

	bool IsComplete() { return m_availableQueries.empty(); }

	std::unordered_set<Query*> GetVisitedQueries() const
	{
		std::unordered_set<Query*> visitedQueries;
		auto currentMatch = this;
		while (currentMatch->m_parent != nullptr) {
			if (currentMatch->m_query != nullptr) {
				visitedQueries.insert(currentMatch->m_query);
			}
			currentMatch = currentMatch->m_parent;
		}
		return visitedQueries;
	}

	void SetAvailableQueries(const std::unordered_set<Query*>& queries) { m_availableQueries = queries; }
	const std::unordered_set<Query*>& GetAvailableQueries() const { return m_availableQueries; }

	Query* m_query;
	NodePtr m_node;
	MatchTree* m_parent;
	std::vector<MatchTree> m_childMatches;
	std::unordered_set<Query*> m_availableQueries;
};

void processMatches(MatchTree* currentMatch, const std::unordered_set<Query*>& queries, const NodeSet& nodes)
{
	for (auto q : queries) {
		for (auto& n : nodes) {
			if (q->Matches(n)) {
				currentMatch->CreateChild(q, n);
			}
		}
	}
}

}  // namespace

void QueryTopology::Match(Graph* graph, std::vector<Match_t>& matches)
{
	START_PROFILE;

	MatchTree rootMatch(nullptr, nullptr);
	rootMatch.SetAvailableQueries(m_queries);
	std::stack<MatchTree*> matchStack;
	std::unordered_set<Match_t, MatchHasher, MatchHasher> matchSet;

	// Initialization
	for (auto q : m_queries) {
		NodeSet nodeSet;
		q->SetQueryHandle(nodeSet);
		q->SetGraph(graph);
		graph->ExecuteQuery(*q);

		rootMatch.m_childMatches.reserve(nodeSet.size());
		for (auto& n : nodeSet) {
			rootMatch.CreateChild(q, n);
		}
	}
	for (auto& m : rootMatch.m_childMatches) {
		matchStack.push(&m);
	}

	while (!matchStack.empty()) {
		MatchTree* match = matchStack.top();
		matchStack.pop();

		auto node = match->m_node;

		bool matchIsComplete = match->IsComplete();
		if (matchIsComplete) {
			continue;
		}

		std::unordered_set<Query*> downstream;
		std::unordered_set<Query*> upstream;

		GetDownstreamQueriesOf(match->m_query, std::inserter(downstream, downstream.end()));
		GetUpstreamQueriesOf(match->m_query, std::inserter(upstream, upstream.end()));
		NodeSet outputNodes = graph->GetNodeOutputNodes(node->GetName());
		NodeSet inputNodes = graph->GetNodeInputNodes(node->GetName());

		processMatches(match, downstream, outputNodes);
		processMatches(match, upstream, inputNodes);

		for (auto& m : match->m_childMatches) {
			matchStack.push(&m);
		}
	}
	rootMatch.CollectMatches(matchSet);
	std::copy(matchSet.begin(), matchSet.end(), std::back_inserter(matches));
}
}  // namespace pagoda::graph::query
