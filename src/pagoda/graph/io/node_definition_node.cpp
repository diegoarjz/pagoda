#include "node_definition_node.h"

#include "ast_node_visitor.h"

namespace pagoda::graph::io
{
NodeDefinitionNode::NodeDefinitionNode() : GraphStatementNode() {}

NodeDefinitionNode::NodeDefinitionNode(const std::string &name, const std::string &nodeType,
                                       const ConstructionArgumentContainer_t &args)
  : m_nodeName(name), m_nodeType(nodeType), m_constructionArguments(args)
{
}

NodeDefinitionNode::NodeDefinitionNode(const AstNode::Offset_t &startOffset, const AstNode::Offset_t &endOffset,
                                       const std::string &name, const std::string &nodeType,
                                       const ConstructionArgumentContainer_t &args)
  : GraphStatementNode(startOffset, endOffset), m_nodeName(name), m_nodeType(nodeType), m_constructionArguments(args)
{
}

NodeDefinitionNode::NodeDefinitionNode(const AstNode::NodeRange_t &nodeRange, const std::string &name,
                                       const std::string &nodeType, const ConstructionArgumentContainer_t &args)
  : GraphStatementNode(nodeRange), m_nodeName(name), m_nodeType(nodeType), m_constructionArguments(args)
{
}

NodeDefinitionNode::~NodeDefinitionNode() {}

const std::string &NodeDefinitionNode::GetNodeName() const { return m_nodeName; }

std::string &NodeDefinitionNode::GetNodeName() { return m_nodeName; }

void NodeDefinitionNode::SetNodeName(const std::string &name) { m_nodeName = name; }

const std::string &NodeDefinitionNode::GetNodeType() const { return m_nodeType; }

std::string &NodeDefinitionNode::GetNodeType() { return m_nodeType; }

void NodeDefinitionNode::SetNodeType(const std::string &type) { m_nodeType = type; }

const NodeDefinitionNode::ConstructionArgumentContainer_t &NodeDefinitionNode::GetConstructionArguments() const
{
	return m_constructionArguments;
}

void NodeDefinitionNode::SetConstructionArguments(const ConstructionArgumentContainer_t &args)
{
	m_constructionArguments = args;
}

void NodeDefinitionNode::AddConstructionArgument(const NamedArgumentPtr &arg)
{
	m_constructionArguments.push_back(arg);
}

const NodeDefinitionNode::ExecutionArgumentContainer_t &NodeDefinitionNode::GetExecutionArguments() const
{
	return m_executionArguments;
}

void NodeDefinitionNode::SetExecutionArguments(const ExecutionArgumentContainer_t &args)
{
	m_executionArguments = args;
}

void NodeDefinitionNode::AddExecutionArgument(const NamedArgumentPtr &arg) { m_executionArguments.push_back(arg); }

void NodeDefinitionNode::AcceptVisitor(AstNodeVisitor *visitor)
{
	visitor->Visit(std::dynamic_pointer_cast<NodeDefinitionNode>(shared_from_this()).get());
}
}  // namespace pagoda::graph::io
