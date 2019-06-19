#include "node_definition_node.h"

namespace selector
{
NodeDefinitionNode::NodeDefinitionNode(const std::string &name, const ConstructionArgumentContainer_t &args)
	: m_nodeName(name),
	  m_constructionArguments(args)
{
}

NodeDefinitionNode::NodeDefinitionNode(const NodeOffset_t &startOffset, const NodeOffset_t &endOffset, const std::string &name, const ConstructionArgumentContainer_t &args)
	: AstNode(startOffset, endOffset),
	  m_nodeName(name),
	  m_constructionArguments(args)
{
}

NodeDefinitionNode::NodeDefinitionNode(const NodeRange_t &nodeRange, const std::string &name, const ConstructionArgumentContainer_t &args)
	: AstNode(nodeRange),
	  m_nodeName(name),
	  m_constructionArguments(args)
{
}

NodeDefinitionNode::~NodeDefinitionNode()
{
}

const std::string& NodeDefinitionNode::GetNodeName() const
{
	return m_nodeName;
}

std::string& NodeDefinitionNode::GetNodeName()
{
	return m_nodeName;
}

void NodeDefinitionNode::SetNodeName(const std::string &name)
{
	m_nodeName = name;
}

const NodeDefinitionNode::ConstructionArgumentContainer_t& GetConstructionArguments() const
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

const ExecutionArgumentContainer_t& NodeDefinitionNode::GetExecutionArguments()  const
{
	return m_executionArguments;
}

void NodeDefinitionNode::SetExecutionArguments(const ExecutionArgumentContainer_t &args)
{
	return m_executionArguments;
}
	
void NodeDefinitionNode::AddExecutionArgument(const NamedArgumentPtr &arg)
{
	m_executionArguments.push_back(arg);
}
}