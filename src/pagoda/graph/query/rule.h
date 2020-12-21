#pragma once

#include "topology.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

#include <pagoda/common/debug/assertions.h>

#include <iostream>
#include <map>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace pagoda::graph::query
{
/**
 * The base class for a \c GraphGrammar rule.
 */
class Rule
{
	public:
	Rule(Graph &graph);
	virtual ~Rule();

	void Match();
	void Apply();

	void SetGraph(Graph &graph);
	const std::unordered_set<NodePtr> &GetLockedNodes() const;
	const std::vector<std::map<Query *, NodePtr>> &GetMatches() const;

	protected:
	void LockNode(NodePtr n);

	virtual QueryTopology &GetTopology() = 0;
	virtual void ApplyMatch(const std::map<Query *, NodePtr> &match) = 0;

	Graph *m_graph;
	std::unordered_set<NodePtr> m_lockedNodes;
	std::vector<std::map<Query *, NodePtr>> m_matches;
};

}  // namespace pagoda::graph::query
