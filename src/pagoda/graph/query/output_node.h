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

	bool Matches(NodePtr n) override;
};
}  // namespace pagoda::graph::query
