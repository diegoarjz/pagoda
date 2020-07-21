#pragma once

#include "query.h"

namespace pagoda::graph::query
{
class OutputNode : public Query
{
public:
	OutputNode(QueryHandle_t queryHandle);
	OutputNode(NodeSet<Node> &nodeSet);

	bool Matches(NodePtr n) override;
};
}
