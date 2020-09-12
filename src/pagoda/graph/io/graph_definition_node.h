#ifndef PAGODA_PROCEDURAL_GRAPH_GRAPH_DEFINITION_NODE_H_
#define PAGODA_PROCEDURAL_GRAPH_GRAPH_DEFINITION_NODE_H_

#include "ast_node.h"

#include <vector>

namespace pagoda::graph::io
{
class GraphStatementNode;
using GraphStatementNodePtr = std::shared_ptr<GraphStatementNode>;

class NodeDefinitionNode;
using NodeDefinitionNodePtr = std::shared_ptr<NodeDefinitionNode>;

class NodeLinkNode;
using NodeLinkNodePtr = std::shared_ptr<NodeLinkNode>;

/**
 * Represents an entire graph definition in the graph format.
 */
class GraphDefinitionNode : public AstNode
{
	public:
	/// Type used to store \c GraphStatementNode.
	using StatementContainer_t = std::vector<GraphStatementNodePtr>;
	/// Type used to iterate over \c StatementContainer_t
	using StatementIterator_t = StatementContainer_t::iterator;

	/**
	 * Default constructor.
	 */
	GraphDefinitionNode();
	/**
	 * Constructs a \c GraphDefinitionNode with the given \p startOffset and \p endOffset.
	 */
	GraphDefinitionNode(const AstNode::Offset_t &startOffset, const AstNode::Offset_t &endOffset);
	/**
	 * Constructs a \c GraphDefinitionNode with the given \p range.
	 */
	GraphDefinitionNode(const AstNode::NodeRange_t &range);
	/**
	 * Constructs a \c GraphDefinitionNode with the given \p statements.
	 */
	GraphDefinitionNode(const StatementContainer_t &statements);

	virtual ~GraphDefinitionNode();

	/**
	 * Creates and adds a \c NodeDefinitionNode with the given arguments.
	 * \p args must match one of the constructors in \c NodeDefinitionNode.
	 */
	template<typename... Args>
	NodeDefinitionNodePtr CreateNodeDefinitionNode(Args... args)
	{
		auto node = std::make_shared<NodeDefinitionNode>(args...);
		AddGraphStatementNode(node);
		return node;
	}

	/**
	 * Creates and adds a \c NodeLinkNode with the given arguments.
	 * \p args should match one of the constructors in \c NodeLinkNode.
	 */
	template<typename... Args>
	NodeLinkNodePtr CreateNodeLinkNode(Args... args)
	{
		auto node = std::make_shared<NodeLinkNode>(args...);
		AddGraphStatementNode(node);
		return node;
	}

	/**
	 * Returns the graph statements.
	 */
	const StatementContainer_t &GetGraphStatementNodes() const;
	/**
	 * Returns the graph statements.
	 */
	StatementContainer_t &GetGraphStatements();
	/**
	 * Adds a \c GraphStatementNode to this \c GraphDefinitionNode.
	 */
	void AddGraphStatementNode(const GraphStatementNodePtr &n);

	/**
	 * Sets the graph statements.
	 */
	void SetGraphStatementNodes(const std::vector<GraphStatementNodePtr> &statements);

	/**
	 * Gets an iterator to the beginning of the graph statements.
	 */
	StatementIterator_t begin();
	/**
	 * Gets an iterator to the end of the graph statements.
	 */
	StatementIterator_t end();

	/**
	 * Allows \c AstNodeVisitor to visit this node type.
	 */
	void AcceptVisitor(AstNodeVisitor *visitor) override;

	private:
	/// The graph statements.
	std::vector<GraphStatementNodePtr> m_graphStatements;
};

using GraphDefinitionNodePtr = std::shared_ptr<GraphDefinitionNode>;
}  // namespace pagoda::graph::io

#endif
