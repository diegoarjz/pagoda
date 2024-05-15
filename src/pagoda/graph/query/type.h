#pragma once

#include <memory>
#include "query.h"

#include "pagoda/graph/node.h"

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

	void AppendToString(std::stringstream &os, uint32_t indent) const override
	{
		os << std::string(indent, ' ') << "Type<" << T::name << ">";
	}

	private:
	bool matches(NodePtr n) override { return std::strcmp(T::name, n->GetNodeType()) == 0; }
};

template<class T, class... Args>
Type<T> type(Args &&... args)
{
	return Type<T>(args...);
}
}  // namespace pagoda::graph::query
