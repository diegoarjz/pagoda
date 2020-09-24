#pragma once

#include "query.h"

#include <pagoda/common/debug/assertions.h>

#include <memory>
#include <vector>

namespace pagoda::graph::query
{
class Not : public Query
{
	public:
	Not(std::shared_ptr<Query> &&query);
	Not(Graph &graph, QueryHandle_t queryHandle);
	Not(Graph &graph, NodeSet &nodeSet);
	Not(Graph &graph, QueryHandle_t queryHandle, std::shared_ptr<Query> &&query);
	Not(Graph &graph, NodeSet &nodeSet, std::shared_ptr<Query> &&query);

	~Not() override;

	void AppendToString(std::stringstream &os, uint32_t indent = 0) const override;

	private:
	bool matches(NodePtr n) override;

	std::shared_ptr<Query> m_query;
};

template<class Q>
Not operator~(Q &&q)
{
	return Not(*q.GetGraph(), q.GetQueryHandle(), std::make_shared<Q>(q));
}

}  // namespace pagoda::graph::query

