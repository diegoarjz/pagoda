#pragma once

#include "traversal.h"

#include <queue>
#include <set>

namespace pagoda::graph::traversal
{
class Forward : public Traversal
{
	public:
	Forward(Graph& graph);

	NodePtr Get() override;
	bool Advance() override;
	bool HasNext() override;

	private:
	std::queue<NodePtr> m_nodesToVisit;
};
}  // namespace pagoda::graph::traversal
