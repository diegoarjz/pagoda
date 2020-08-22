#pragma once

#include "query.h"
#include <memory>

namespace pagoda::graph::query
{
template<class T>
class Type : public Query
{
public:
	Type(Graph &graph, QueryHandle_t queryHandle) : Query(graph, queryHandle) {}
	Type(Graph &graph, NodeSet &nodeSet) : Query(graph, nodeSet) {}

	bool Matches(NodePtr n) override
	{
        return std::dynamic_pointer_cast<T>(n) != nullptr;
	}
};
}  // namespace pagoda::graph::query
