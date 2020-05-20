#include "graph_definition_node.h"

#include "ast_node_visitor.h"

namespace pagoda::graph::io
{
GraphDefinitionNode::GraphDefinitionNode() {}

GraphDefinitionNode::GraphDefinitionNode(const AstNode::Offset_t &startOffset, const AstNode::Offset_t &endOffset)
    : AstNode(startOffset, endOffset)
{
}

GraphDefinitionNode::GraphDefinitionNode(const AstNode::NodeRange_t &range) : AstNode(range) {}

GraphDefinitionNode::GraphDefinitionNode(const GraphDefinitionNode::StatementContainer_t &statements)
    : m_graphStatements(statements)
{
}

GraphDefinitionNode::~GraphDefinitionNode() {}

const GraphDefinitionNode::StatementContainer_t &GraphDefinitionNode::GetGraphStatementNodes() const
{
	return m_graphStatements;
}

GraphDefinitionNode::StatementContainer_t &GraphDefinitionNode::GetGraphStatements() { return m_graphStatements; }

void GraphDefinitionNode::AddGraphStatementNode(const GraphStatementNodePtr &n) { m_graphStatements.push_back(n); }

void GraphDefinitionNode::SetGraphStatementNodes(const std::vector<GraphStatementNodePtr> &statements)
{
	m_graphStatements = statements;
}

GraphDefinitionNode::StatementIterator_t GraphDefinitionNode::begin() { return m_graphStatements.begin(); }

GraphDefinitionNode::StatementIterator_t GraphDefinitionNode::end() { return m_graphStatements.end(); }

void GraphDefinitionNode::AcceptVisitor(AstNodeVisitor *visitor)
{
	visitor->Visit(std::dynamic_pointer_cast<GraphDefinitionNode>(shared_from_this()).get());
}

}  // namespace pagoda::graph::io
