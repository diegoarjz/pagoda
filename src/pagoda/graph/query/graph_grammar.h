#pragma once

#include <memory>
#include <vector>

namespace pagoda::graph
{
class Graph;
}

namespace pagoda::graph::query
{
class Rule;

/**
 * Applies a set of \c Rule to a \c Graph.
 */
class GraphGrammar
{
	public:
	GraphGrammar(Graph &graph);

	void AddRule(std::shared_ptr<Rule> rule);

	void Run();

	private:
	Graph &m_graph;
	std::vector<std::shared_ptr<Rule>> m_rules;
};
}  // namespace pagoda::graph::query
