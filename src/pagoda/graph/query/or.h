#pragma once

#include "query.h"

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

}  // namespace pagoda::graph::query

