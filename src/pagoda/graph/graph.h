#pragma once

#include "node_set.h"
#include "scheduler.h"
#include "traversal/traversal.h"

#include <functional>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace pagoda::dynamic
{
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;
}  // namespace pagoda::dynamic

namespace pagoda::objects
{
class NewParameterCallback;
}

namespace pagoda::graph
{
class Node;
using NodePtr = std::shared_ptr<Node>;

class NodeFactory;
using NodeFactoryPtr = std::shared_ptr<NodeFactory>;

class ConstructionArgumentCallback;

namespace query
{
class Query;
}

/**
 * @brief Represents a procedural graph.
 *
 * A procedural graph is a directed graph that details the execution of
 * the operations and the flow of procedural items through the operations.
 *
 * All sorts of concepts in a procedural graph are represented by nodes.
 * Operations nodes, interfaces between procedural operations, etc. The
 * reasoning for this approach is that it allows a higher level of
 * flexibility and extensibility while defining procedural graph.
 * Links between nodes do represent the flow of procedural items but
 * different nodes might process the items in different ways. Some
 * may create new geometry with procedural operations. Other might
 * filter the flow of items through the nodes.
 */
class Graph
{
	public:
	using SchedulerFactoryFunction_t =
	  std::function<std::unique_ptr<IScheduler>(Graph &)>;

	static void SetSchedulerFactory(
	  const SchedulerFactoryFunction_t &factoryFunction);

	static SchedulerFactoryFunction_t GetSchedulerFactory();

	using NodeIdentifier_t = std::string;

	/**
	 * Result of calling CreateEdge.
	 */
	enum class EdgeCreated
	{
		Created,    ///< Edge created successfully.
		EdgeExists  ///< Edge not created since edge already exists.
	};

	/**
	 * Result of calling DestroyEdge.
	 */
	enum class EdgeDestroyed
	{
		Destroyed,       ///< Edge destroyed successfully.
		EdgeDoesntExist  ///< Edge not destroyed since it didn't exist.
	};

	/**
	 * Constructs a \c Graph.
	 */
	Graph(NodeFactoryPtr nodeFactory);
	~Graph();

	Graph(const Graph &) = delete;
	Graph &operator=(const Graph &) = delete;

	/**
	 * Creates and adds \c Node object for this \c Graph.
	 *
	 * @return An instance of a \c Node.
	 */
	template<class N>
	NodeIdentifier_t CreateNode()
	{
		return CreateNode(N::name, N::name);
	}

	/**
	 * Creates and adds \c Node object for this \c Graph.
	 *
	 * @return An instance of a \c Node.
	 */
	template<class N>
	NodeIdentifier_t CreateNode(const NodeIdentifier_t &nodeName)
	{
		return CreateNode(N::name, nodeName);
	}

	/**
	 * Creates and adds a \c Node for this \c Graph.
	 *
	 * @return An instance of a \c Node.
	 */
	NodeIdentifier_t CreateNode(const std::string &nodeType);

	/**
	 * Creates and adds a \c Node with the given \c nodeType and \c nodeName.
	 */
	NodeIdentifier_t CreateNode(const std::string &nodeType,
	                            const NodeIdentifier_t &nodeName);

	/**
	 * Destroys a \c Node object from this \c Graph.
	 *
	 * The \c Node is removed from the \c Graph, removing all links with
	 * other \c Node objects.
	 *
	 * The \c Graph ceases to have ownership of the \c Node to be destroyed.
	 *
	 * @param [in] node The Node to be destroyed and removed from this graph.
	 */
	void DestroyNode(const NodeIdentifier_t &node);

	/**
	 * Returns a \c Node given its \p name.
	 */
	NodePtr GetNode(const NodeIdentifier_t &name) const;

	/**
	 * Creates an edge between two \c Node objects in this \c Graph.
	 *
	 * There can only be at most one edge connecting two \c Nodes. This method
	 * checks if an edge already exists and returns false if it does.
	 *
	 * @param [in] sourceNode The source \c Node.
	 * @param [in] targetNode The target \c Node.
	 * @return Returns true if linking both nodes was successful, false otherwise.
	 */
	EdgeCreated CreateEdge(const NodeIdentifier_t &sourceNode,
	                       const NodeIdentifier_t &targetNode);

	/**
	 * Destroys an edge between two \c Node objects in this \c Graph.
	 *
	 * @param [in] sourceNode The source \c Node.
	 * @param [in] targetNode The target \c Node.
	 * @return Returns true if unlinking the nodes was successful, false
	 * otherwise.
	 */
	EdgeDestroyed DestroyEdge(const NodeIdentifier_t &sourceNode,
	                          const NodeIdentifier_t &targetNode);

	/**
	 * Returns the number of \c Node in this \c Graph.
	 */
	std::size_t GetNodeCount() const;

	/**
	 * Returns all \c Node objects that share an edge with \p node.
	 *
	 * @param [in] node The node.
	 */
	NodeSet GetNodesAdjacentTo(const NodeIdentifier_t &node);

	/**
	 * Returns all input \c Node objects of \p node.
	 *
	 * @param [in] node The node.
	 */
	NodeSet GetNodeInputNodes(const NodeIdentifier_t &node);

	/**
	 * Returns all output \c Node objects of \p node.
	 *
	 * @param [in] node The node.
	 */
	NodeSet GetNodeOutputNodes(const NodeIdentifier_t &node);

	/**
	 * Sets the \c IScheduler for this \c Graph to \p scheduler.
	 */
	void SetScheduler(std::unique_ptr<IScheduler> scheduler);

	/**
	 * Executes the \c Graph using the defined \c IScheduler.
	 */
	void Execute();

	/**
	 * Executes a query on the \c Graph collecting the \c Nodes.
	 */
	void ExecuteQuery(query::Query &q);

	void SetNodeConstructionParameters(const NodeIdentifier_t &nodeName,
	                                   ConstructionArgumentCallback *cb);

	void SetNodeExecutionParameters(const NodeIdentifier_t &nodeName,
	                                objects::NewParameterCallback *cb);

	private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;

	static SchedulerFactoryFunction_t s_schedulerFactoryFunction;

	NodeSet &getNodes();

	friend class traversal::Traversal;
};  // class Graph

using GraphPtr = std::shared_ptr<Graph>;

}  // namespace pagoda::graph
