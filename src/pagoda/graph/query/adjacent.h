#pragma once

#include "query.h"

namespace pagoda::graph::query
{
/**
 * Queries if adjacent/upstream/downstream nodes verify a given condition.
 */
class Adjacent : public Query
{
	public:
	Adjacent(std::shared_ptr<Query> &&query = nullptr, int32_t direction = 0);
	Adjacent(Graph &graph, QueryHandle_t queryHandle, std::shared_ptr<Query> &&query = nullptr, int32_t direction = 0);
	Adjacent(Graph &graph, NodeSet &nodeSet, std::shared_ptr<Query> &&query = nullptr, int32_t direction = 0);

	~Adjacent() override;

	void AppendToString(std::stringstream &os, uint32_t indent = 0) const override;

	private:
	bool matches(NodePtr n) override;

	int32_t m_direction;
};

template<class Q>
Adjacent downstream(Q query)
{
	return Adjacent(std::make_shared<Q>(query), 1);
}

template<class Q>
Adjacent upstream(Q query)
{
	return Adjacent(std::make_shared<Q>(query), -1);
}

}  // namespace pagoda::graph::query
