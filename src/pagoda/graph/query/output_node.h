#pragma once

#include "query.h"

namespace pagoda::graph::query
{
/**
 * Query condition that matches output nodes on a graph.
 */
class OutputNode : public Query
{
	public:
	OutputNode();
	OutputNode(Graph &graph, QueryHandle_t queryHandle);
	OutputNode(Graph &graph, NodeSet &nodeSet);

	void AppendToString(std::stringstream &os, uint32_t indent = 0) const override;

	private:
	bool matches(NodePtr n) override;
};

template<class... Args>
OutputNode output_node(Args &&... args)
{
	return OutputNode(args...);
}
}  // namespace pagoda::graph::query
