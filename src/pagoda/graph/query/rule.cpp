#include "rule.h"

namespace pagoda::graph::query
{
Rule::Rule(Graph &graph) : m_graph(&graph) {}
Rule::~Rule() {}

void Rule::Match() { GetTopology().Match(m_graph, m_matches); }

void Rule::Apply()
{
	for (const auto &m : m_matches) {
		bool locked = false;
		for (const auto &p : m) {
			if (m_lockedNodes.find(p.second) != m_lockedNodes.end()) {
				locked = true;
				break;
			}
		}
		if (!locked) {
			ApplyMatch(m);
		}
	}
}

void Rule::LockNode(NodePtr n) { m_lockedNodes.insert(n); }

void Rule::SetGraph(Graph &graph) { m_graph = &graph; }

const std::unordered_set<NodePtr> &Rule::GetLockedNodes() const { return m_lockedNodes; }

const std::vector<std::map<Query *, NodePtr>> &Rule::GetMatches() const { return m_matches; }
}  // namespace pagoda::graph::query
