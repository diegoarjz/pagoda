#pragma once

#include "traversal.h"

namespace pagoda::graph::traversal
{
class Linear : public Traversal
{
	public:
	Linear(Graph& graph);

	NodePtr Get() override;
	bool Advance() override;
	bool HasNext() override;

	private:
	NodeSet::iterator m_iterator;
};
}  // namespace pagoda::graph::traversal
