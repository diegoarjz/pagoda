#pragma once

#include "query.h"

namespace pagoda::graph::query
{
/**
 * Query condition that matches input nodes on a graph.
 */
class InputNode : public Query
{
	public:
	InputNode();
	InputNode(Graph &graph, QueryHandle_t queryHandle);
	InputNode(Graph &graph, NodeSet &nodeSet);

	void AppendToString(std::stringstream &os, uint32_t indent = 0) const override;

	private:
	bool matches(NodePtr n) override;
};

template<class... Args>
InputNode input_node(Args &&... args)
{
	return InputNode(args...);
}
}  // namespace pagoda::graph::query
