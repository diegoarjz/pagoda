#include "linear.h"

#include <pagoda/graph/graph.h>

namespace pagoda::graph::traversal
{
Linear::Linear(Graph& graph) : Traversal(graph), m_iterator(GetNodes().begin()) {}
Linear::~Linear() {}

NodePtr Linear::Get() { return *m_iterator; }
bool Linear::Advance()
{
	++m_iterator;
	return HasNext();
}
bool Linear::HasNext() { return m_iterator != GetNodes().end(); }

}  // namespace pagoda::graph::traversal
