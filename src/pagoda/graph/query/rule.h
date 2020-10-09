#pragma once

#include "query.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

#include <memory>
#include <unordered_set>

namespace pagoda::graph::query
{
/**
 * The base class for a graph grammar rule.
 */
class RuleBase
{
	public:
	RuleBase(Graph &graph, Query &query);
	virtual ~RuleBase();

	const std::unordered_set<NodePtr> &GetLockedNodes() const;

	protected:
	Graph *m_graph;
	Query *m_query;
	std::unordered_set<NodePtr> m_lockedNodes;
};

/**
 * Concrete class for a graph grammar rule.
 *
 * Executes a \c Query in a \c Graph, deferring full matches of type T to an
 * external function for node/edges replacement.
 */
template<class T>
class Rule : public RuleBase
{
	public:
	Rule(Graph &graph, Query &query, T &fullMatch) : RuleBase(graph, query), m_fullMatch(fullMatch) {}

	void Run(std::function<void(T &)> match)
	{
		m_query->SetGraph(m_graph);
		m_query->SetQueryHandle([&](NodePtr n) {
			m_fullMatch.GetLockedNodes(std::inserter(m_lockedNodes, std::end(m_lockedNodes)));
			match(m_fullMatch);
		});
		m_graph->ExecuteQuery(*m_query);
	}

	private:
	T &m_fullMatch;
};
}  // namespace pagoda::graph::query
