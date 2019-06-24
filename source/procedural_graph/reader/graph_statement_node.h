#ifndef SELECTOR_PROCEDURAL_GRAPH_GRAPH_STATEMENT_NODE_H_
#define SELECTOR_PROCEDURAL_GRAPH_GRAPH_STATEMENT_NODE_H_

#include "ast_node.h"

namespace selector
{
/**
 * Represents a statement in the graph format.
 * Statements can be, for example, a node definition or node links.
 */
class GraphStatementNode : public AstNode
{
public:
	GraphStatementNode() : AstNode() {}

	GraphStatementNode(const AstNode::Offset_t &startOffset, const AstNode::Offset_t &endOffset)
	    : AstNode(startOffset, endOffset)
	{
	}

	GraphStatementNode(const AstNode::NodeRange_t &range) : AstNode(range) {}

private:
};

using GraphStatementNodePtr = std::shared_ptr<GraphStatementNode>;
}  // namespace selector

#endif
