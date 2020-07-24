#pragma once

#include "query.h"

namespace pagoda::graph::query
{
class InputNode : public Query
{
public:
	InputNode(Graph &graph, QueryHandle_t queryHandle);
	InputNode(Graph &graph, NodeSet &nodeSet);

	bool Matches(NodePtr n) override;
};
}  // namespace pagoda::graph::query
