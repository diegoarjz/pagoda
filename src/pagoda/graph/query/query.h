#pragma once

#include <pagoda/graph/node_set.h>

#include <functional>
#include <memory>
#include <sstream>

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
	/// Type definition for a container of \c Query
	using QueryContainer_t = std::vector<std::shared_ptr<Query>>;
	/// Function type to handle each \c Node that matches a \c Query.
	using QueryHandle_t = std::function<void(NodePtr)>;

	/**
	 * Default constructor.
	 * Should only be used to create sub queries.
	 */
	Query();

	/**
	 * Constructor with a custom \c QueryHandle_t.
	 */
	Query(Graph& graph, QueryHandle_t queryHandle);
	/**
	 * Specialized constructor that adds results in the \p nodeSet \c NodeSet.
	 */
	Query(Graph& graph, NodeSet& nodeSet);

	virtual ~Query();

	/**
	 * Checks whether or not the given \c Node \p n passes the condition.
	 */
	bool Matches(NodePtr n);

	void SetGraph(Graph* graph);
	Graph* GetGraph() const;
	QueryHandle_t& GetQueryHandle();

	const std::size_t GetQueryHash() const;

	std::string ToString() const;
	virtual void AppendToString(std::stringstream& os, uint32_t indent = 0) const;

	protected:
	Graph* m_graph;
	QueryHandle_t m_queryHandle;

	private:
	void AddNode(NodePtr n);

	virtual bool matches(NodePtr n);

	friend class pagoda::graph::Graph;
	friend class pagoda::graph::NodeSet;
};

template<class... Args>
Query all(Args&&... args)
{
	return Query(args...);
}
}  // namespace pagoda::graph::query
