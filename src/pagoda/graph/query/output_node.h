#pragma once

#include "query.h"

namespace pagoda::graph::query
{
class OutputNode : public Query
{
public:
	OutputNode(QueryHandle_t queryHandle);
	OutputNode(NodeSet &nodeSet);

	bool Matches(NodePtr n) override;
};
}  // namespace pagoda::graph::query
