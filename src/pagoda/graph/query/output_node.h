#pragma once

#include "query.h"

namespace pagoda::graph::query
{
class OutputNode : public Query
{
public:
	OutputNode(Graph &graph, QueryHandle_t queryHandle);
	OutputNode(Graph &graph, NodeSet &nodeSet);

	bool Matches(NodePtr n) override;
};
}  // namespace pagoda::graph::query
