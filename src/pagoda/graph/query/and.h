#pragma once

#include "query.h"

#include <pagoda/common/debug/assertions.h>

#include <memory>
#include <vector>

namespace pagoda::graph::query
{
class And : public Query
{
	public:
	And(QueryContainer_t &&queries);
	And(Graph &graph, QueryHandle_t queryHandle);
	And(Graph &graph, NodeSet &nodeSet);
	And(Graph &graph, QueryHandle_t queryHandle, QueryContainer_t &&queries);
	And(Graph &graph, NodeSet &nodeSet, QueryContainer_t &&queries);

	~And() override;

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
And operator&(LHS &&lhs, RHS &&rhs)
{
	auto lhsGraph = lhs.GetGraph();
	auto rhsGraph = rhs.GetGraph();
	DBG_ASSERT(lhsGraph != nullptr || rhsGraph != nullptr);
	DBG_ASSERT(lhsGraph == rhsGraph || rhsGraph == nullptr || lhsGraph == nullptr);
	return And(*lhs.GetGraph(), lhs.GetQueryHandle(), {std::make_shared<LHS>(lhs), std::make_shared<RHS>(rhs)});
}

}  // namespace pagoda::graph::query
