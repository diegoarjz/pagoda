#include "graph_grammar.h"

#include "rule.h"

#include <pagoda/common/instrument/profiler.h>

#include <pagoda/graph/graph.h>

namespace pagoda::graph::query
{
GraphGrammar::GraphGrammar(Graph &graph) : m_graph(graph) {}

void GraphGrammar::AddRule(std::shared_ptr<Rule> rule) { m_rules.push_back(rule); }

void GraphGrammar::Run()
{
	START_PROFILE;
	for (auto &r : m_rules) {
		r->SetGraph(m_graph);
		r->Match();
		r->Apply();
	}
}
}  // namespace pagoda::graph::query
