#pragma once

#include "query.h"

namespace pagoda::graph::query
{
class InputNode : public Query
{
public:
	InputNode(QueryHandle_t queryHandle);
	InputNode(NodeSet &nodeSet);

	bool Matches(NodePtr n) override;
};
}  // namespace pagoda::graph::query
