#pragma once

#include "query.h"

#include <pagoda/common/debug/assertions.h>

#include <memory>
#include <vector>

namespace pagoda::graph::query
{
class Or : public Query
{
	public:
	Or(Graph &graph, QueryHandle_t queryHandle);
	Or(Graph &graph, NodeSet &nodeSet);
	Or(Graph &graph, QueryHandle_t queryHandle, QueryContainer_t &&queries);
	Or(Graph &graph, NodeSet &nodeSet, QueryContainer_t &&queries);

	~Or() override;

	bool Matches(NodePtr n) override;

	template<class T>
	void AddQuery(T q)
	{
		m_queries.push_back(std::make_shared<T>(q));
	}

	private:
	std::vector<std::shared_ptr<Query>> m_queries;
};

template<class LHS, class RHS>
Or operator|(LHS &&lhs, RHS &&rhs)
{
	auto lhsGraph = lhs.GetGraph();
	auto rhsGraph = rhs.GetGraph();
	DBG_ASSERT(lhsGraph != nullptr || rhsGraph != nullptr);
	DBG_ASSERT(lhsGraph == rhsGraph || rhsGraph == nullptr || lhsGraph == nullptr);
	return Or(*lhs.GetGraph(), lhs.GetQueryHandle(), {std::make_shared<LHS>(lhs), std::make_shared<RHS>(rhs)});
}
}  // namespace pagoda::graph::query

