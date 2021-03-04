#pragma once

#include "traversal.h"

#include <queue>
#include <set>

namespace pagoda::graph::traversal
{
/**
 * Allows traversing the graph breadth first.
 */
class Forward : public Traversal
{
	public:
	Forward(Graph& graph);
	~Forward() override;

	NodePtr Get() override;
	bool Advance() override;
	bool HasNext() override;

	private:
	std::queue<NodePtr> m_nodesToVisit;  ///< The queue of next nodes to visit.
};
}  // namespace pagoda::graph::traversal
