#pragma once

#include <memory>
#include "query.h"

namespace pagoda::graph::query
{
/**
 * Query condition that matches nodes of a given C++ type.
 */
template<class T>
class Type : public Query
{
	public:
	Type() {}
	Type(Graph &graph, QueryHandle_t queryHandle) : Query(graph, queryHandle) {}
	Type(Graph &graph, NodeSet &nodeSet) : Query(graph, nodeSet) {}

	bool Matches(NodePtr n) override { return std::dynamic_pointer_cast<T>(n) != nullptr; }
};
}  // namespace pagoda::graph::query
