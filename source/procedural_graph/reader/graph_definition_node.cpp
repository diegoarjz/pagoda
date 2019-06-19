#include "graph_definition_node.h"

namespace selector
{
GraphDefinitionNode::GraphDefinitionNode()
{
}

GraphDefinitionNode::GraphDefinitionNode(const Offset_t &startOffset, const Offset_t &endOffset)
	: AstNode(startOffset, endOffset)
{
}

GraphDefinitionNode::GraphDefinitionNode(const Range_t &range)
	: AstNode(range)
{
}

GraphDefinitionNode::~GraphDefinitionNode()
{
}

void GraphDefinitionNode::AddGraphStatementNode(const GraphStatementNodePtr &n)
{
	m_graphStatements.pusb_back(n);
}

void GraphDefinitionNode::SetGraphStatementNodes(const std::vector<GraphStatementNodePtr> &statements)
{
	m_graphStatements = statements;
}

StatementIterator_t& GraphDefinitionNode::begin()
{
	return m_graphStatements.begin();
}

StatementIterator_t& GraphDefinitionNode::end()
{
	return m_graphStatements.end();
}

}
