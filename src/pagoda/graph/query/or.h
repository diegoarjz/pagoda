#pragma once

#include "query.h"

#include <pagoda/common/debug/assertions.h>

#include <memory>
#include <vector>

namespace pagoda::graph::query
{
/**
 * Allows combining multiple queries with an or operation.
 */
class Or : public Query
{
	public:
	Or(Graph &graph, QueryHandle_t queryHandle);
	Or(Graph &graph, NodeSet &nodeSet);
	Or(Graph &graph, QueryHandle_t queryHandle, QueryContainer_t &&queries);
	Or(Graph &graph, NodeSet &nodeSet, QueryContainer_t &&queries);

	~Or() override;

	template<class T>
	void AddQuery(T q)
	{
		m_subQueries.push_back(std::make_shared<T>(q));
	}

	void AppendToString(std::stringstream &os, uint32_t indent = 0) const override;

	private:
	bool matches(NodePtr n) override;
};

template<class LHS, class RHS>
Or operator|(LHS &&lhs, RHS &&rhs)
{
	auto lhsGraph = lhs.GetGraph();
	auto rhsGraph = rhs.GetGraph();
	DBG_ASSERT(lhsGraph == rhsGraph || rhsGraph == nullptr || lhsGraph == nullptr);
	return Or(*lhs.GetGraph(), lhs.GetQueryHandle(), {std::make_shared<LHS>(lhs), std::make_shared<RHS>(rhs)});
}
}  // namespace pagoda::graph::query

