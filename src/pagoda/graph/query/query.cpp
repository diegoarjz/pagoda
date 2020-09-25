#include "query.h"

#include <pagoda/graph/traversal/linear.h>
#include <pagoda/graph/traversal/traversal.h>

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

#include <sstream>

namespace pagoda::graph::query
{
Query::Query() : m_graph{nullptr} {}
Query::Query(Graph& graph, QueryHandle_t queryHandle) : m_graph(&graph), m_queryHandle(queryHandle) {}
Query::Query(Graph& graph, NodeSet& nodeSet)
  : m_graph(&graph), m_queryHandle([&nodeSet](NodePtr n) { nodeSet.insert(n); })
{
}

Query::~Query() {}

bool Query::Matches(NodePtr n) { return matches(n); }

bool Query::matches(NodePtr n) { return true; }

void Query::AddNode(NodePtr n) { m_queryHandle(n); }

void Query::SetGraph(Graph* graph) { m_graph = graph; }
Graph* Query::GetGraph() const { return m_graph; }
Query::QueryHandle_t& Query::GetQueryHandle() { return m_queryHandle; }

const std::size_t Query::GetQueryHash() const { return 0; }

std::shared_ptr<traversal::Traversal> Query::GetTraversal()
{
	if (m_graph == nullptr) {
		// Can't create a traversal if the graph is not set
		return nullptr;
	}
	if (m_traversal == nullptr) {
		// Default to a linear traversal
		m_traversal = std::make_shared<traversal::Linear>(*m_graph);
	}
	return m_traversal;
}

void Query::SetTraversal(std::shared_ptr<traversal::Traversal> traversal) { m_traversal = traversal; }

std::string Query::ToString() const
{
	std::stringstream os;
	AppendToString(os, 0);
	return os.str();
}

void Query::AppendToString(std::stringstream& os, uint32_t indent) const { os << std::string(indent, ' ') << "Query"; }
}  // namespace pagoda::graph::query
