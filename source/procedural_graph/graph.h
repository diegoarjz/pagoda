#pragma once
#ifndef SELECTOR_PROCEDURAL_GRAH_GRAPH_H_
#define SELECTOR_PROCEDURAL_GRAH_GRAPH_H_

#include "node_type.h"

#include <list>
#include <memory>
#include <string>
#include <unordered_set>

namespace selector
{
class Node;
class NodeFactory;

using NodePtr = std::shared_ptr<Node>;

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
	 *
	 * @param [in] nodeFactory The \c NodeFactory responsible for
	 *                         instantiating \c Node objects for this
	 *                         \c Graph.
	 */
	Graph(std::shared_ptr<NodeFactory> nodeFactory);
	~Graph();

	Graph(const Graph &) = delete;
	Graph &operator=(const Graph &) = delete;

	/**
	 * Creates a \c Node object for this \c Graph.
	 *
	 * The graph takes ownership of the created \c Node.
	 * Both parameters are passed as is to the \c NodeFactory with
	 * which the \c Graph was constructed.
	 *
	 * @param [in] type     The node type to create.
	 * @param [in] typeName The name of the type. See \c NodeFactory.
	 * @return An instance of a \c Node or nullptr if unable to create.
	 */
	NodePtr CreateNode(NodeType type, const std::string &name = "");

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
	 * Returns all nodes in the \c Graph that have the corresponding \c NodeType.
	 *
	 * @param [in] nodeType The type of nodes to return.
	 * @return An unordered set of \c Node objects belonging to this \c Graph. All nodes
	 *         are of the type specified in \p nodeType.
	 */
	const std::unordered_set<NodePtr> &GetGraphNodesByType(NodeType nodeType);

	/**
	 * Returns the input nodes of this \c Graph.
	 */
	std::list<NodePtr> GetGraphInputNodes();

	/**
	 * Returns the output nodes of this \c Graph.
	 */
	std::list<NodePtr> GetGraphOutputNodes();

	/**
	 * Returns all \c Node objects that share an edge with \p node.
	 *
	 * @param [in] node The node.
	 */
	std::list<NodePtr> GetNodesLinkedTo(NodePtr node);

	/**
	 * Returns \c Node objects of type \p nodeType that share an edge with \p node.
	 *
	 * @param [in] node     The node.
	 * @param [in] nodeType The type of nodes.
	 */
	std::list<NodePtr> GetNodesLinkedToByType(NodePtr node, NodeType nodeType);

	/**
	 * Returns all input \c Node objects of \p node.
	 *
	 * @param [in] node The node.
	 */
	std::list<NodePtr> GetNodeInputNodes(NodePtr node);

	/**
	 * Returns all input \c Node objects of type \p nodeType of \p node.
	 *
	 * @param [in] node The node.
	 * @param [in] nodeType The type of nodes.
	 */
	std::list<NodePtr> GetNodeInputNodesByType(NodePtr node, NodeType nodeType);

	/**
	 * Returns all output \c Node objects of \p node.
	 *
	 * @param [in] node The node.
	 */
	std::list<NodePtr> GetNodeOutputNodes(NodePtr node);

	/**
	 * Returns all output \c Node objects of type \p nodeType of \p node.
	 *
	 * @param [in] node The node.
	 * @param [in] nodeType The type of nodes.
	 */
	std::list<NodePtr> GetNodeOutputNodesByType(NodePtr node, NodeType nodeType);

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};  // class Graph

using GraphPtr = std::shared_ptr<Graph>;

}  // namespace selector

#endif
