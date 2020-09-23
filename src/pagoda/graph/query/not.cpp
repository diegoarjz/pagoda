#include "not.h"
#include <algorithm>
#include <iterator>
#include <memory>

namespace pagoda::graph::query
{
Not::Not(std::shared_ptr<Query> &&query) : m_query(std::move(query)) {}
Not::Not(Graph &graph, QueryHandle_t queryHandle) : Query(graph, queryHandle) {}
Not::Not(Graph &graph, NodeSet &nodeSet) : Query(graph, nodeSet) {}
Not::Not(Graph &graph, QueryHandle_t queryHandle, std::shared_ptr<Query> &&query)
  : Query(graph, queryHandle), m_query(std::move(query))
{
}
Not::Not(Graph &graph, NodeSet &nodeSet, std::shared_ptr<Query> &&query)
  : Query(graph, nodeSet), m_query(std::move(query))
{
}

Not::~Not() {}

bool Not::Matches(NodePtr n) { return !m_query->Matches(n); }
}  // namespace pagoda::graph::query

