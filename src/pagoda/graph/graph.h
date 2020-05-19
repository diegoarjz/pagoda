#pragma once
#ifndef PAGODA_PROCEDURAL_GRAH_GRAPH_H_
#define PAGODA_PROCEDURAL_GRAH_GRAPH_H_

#include "node_set.h"
#include "scheduler.h"

#include <functional>
#include <list>
#include <memory>
#include <string>
#include <unordered_set>

namespace pagoda::graph
{
class Node;
using NodePtr = std::shared_ptr<Node>;

class NodeFactory;
using NodeFactoryPtr = std::shared_ptr<NodeFactory>;

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
	using SchedulerFactoryFunction_t = std::function<std::unique_ptr<IScheduler>(Graph &)>;

	static void SetSchedulerFactory(const SchedulerFactoryFunction_t &factoryFunction);

	static SchedulerFactoryFunction_t GetSchedulerFactory();

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
	NodePtr CreateNode()
	{
		return CreateNode(N::name);
	}

	/**
	 * Creates and adds a \c Node for this \c Graph.
	 *
	 * @return An instance of a \c Node.
	 */
	NodePtr CreateNode(const std::string &nodeType);

	/**
	 * Adds a \c Node object to this \c Graph.
	 *
	 * The \c Graph will take ownership of the \c Node.
	 */
	void AddNode(NodePtr node);

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
	void DestroyNode(NodePtr node);

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
	EdgeCreated CreateEdge(NodePtr sourceNode, NodePtr targetNode);

	/**
	 * Destroys an edge between two \c Node objects in this \c Graph.
	 *
	 * @param [in] sourceNode The source \c Node.
	 * @param [in] targetNode The target \c Node.
	 * @return Returns true if unlinking the nodes was successful, false otherwise.
	 */
	EdgeDestroyed DestroyEdge(NodePtr sourceNode, NodePtr targetNode);

	/**
	 * Returns all the \c Node in this \c Graph.
	 */
	NodeSet<Node> GetGraphNodes();

	/**
	 * Returns the number of \c Node in this \c Graph.
	 */
	std::size_t GetNodeCount() const;

	/**
	 * Returns the input nodes of this \c Graph.
	 */
	NodeSet<Node> GetGraphInputNodes();

	/**
	 * Returns the output nodes of this \c Graph.
	 */
	NodeSet<Node> GetGraphOutputNodes();

	/**
	 * Returns all \c Node objects that share an edge with \p node.
	 *
	 * @param [in] node The node.
	 */
	NodeSet<Node> GetNodesAdjacentTo(NodePtr node);

	/**
	 * Returns all input \c Node objects of \p node.
	 *
	 * @param [in] node The node.
	 */
	NodeSet<Node> GetNodeInputNodes(NodePtr node);

	/**
	 * Returns all output \c Node objects of \p node.
	 *
	 * @param [in] node The node.
	 */
	NodeSet<Node> GetNodeOutputNodes(NodePtr node);

	/**
	 * Sets the \c IScheduler for this \c Graph to \p scheduler.
	 */
	void SetScheduler(std::unique_ptr<IScheduler> scheduler);

	/**
	 * Executes the \c Graph using the defined \c IScheduler.
	 */
	void Execute();

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;

	static SchedulerFactoryFunction_t s_schedulerFactoryFunction;
};  // class Graph

using GraphPtr = std::shared_ptr<Graph>;

}  // namespace pagoda::graph

#endif
