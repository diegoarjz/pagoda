#include "rule.h"

namespace pagoda::graph::query
{
RuleBase::RuleBase(Graph &graph, Query &query) : m_graph(&graph), m_query(&query) {}
RuleBase::~RuleBase() {}

const std::unordered_set<NodePtr> &RuleBase::GetLockedNodes() const { return m_lockedNodes; }
}  // namespace pagoda::graph::query
