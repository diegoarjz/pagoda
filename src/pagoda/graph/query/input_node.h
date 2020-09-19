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

	bool Matches(NodePtr n) override;
};
}  // namespace pagoda::graph::query
