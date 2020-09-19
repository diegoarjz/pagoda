#pragma once

#include <pagoda/graph/node_set.h>

#include <functional>
#include <memory>

namespace pagoda::graph
{
class Graph;
class Node;
using NodePtr = std::shared_ptr<Node>;
}  // namespace pagoda::graph

namespace pagoda::graph::query
{
/**
 * Base class to perform queries on a \c Graph.
 *
 * \code{.cpp}
 * NodeSet result;
 * Query q(result);
 * graph->ExecuteQuery(q);
 * \endcode
 *
 * or
 *
 * \code{.cpp}
 * std::vector<NodePtr> result;
 * Query q([&result](NodePtr n) { result.push_back(n); });
 * graph->ExecuteQuery(q);
 * \endcode
 */
class Query
{
	public:
	/// Function type to handle each \c Node that matches a \c Query.
	using QueryHandle_t = std::function<void(NodePtr)>;

	/**
	 * Constructor with a custom \c QueryHandle_t.
	 */
	Query(Graph& graph, QueryHandle_t queryHandle);
	/**
	 * Specialized constructor that adds results in the \p nodeSet \c NodeSet.
	 */
	Query(Graph& graph, NodeSet& nodeSet);

	/**
	 * Checks whether or not the given \c Node \p n passes the condition.
	 */
	virtual bool Matches(NodePtr n);

	const std::string& GetQueryHash() const;

	protected:
	Graph& m_graph;
	QueryHandle_t m_queryHandle;

	private:
	void AddNode(NodePtr n);

	friend class pagoda::graph::Graph;
	friend class pagoda::graph::NodeSet;
};
}  // namespace pagoda::graph::query
