#include "node_link_node.h"

namespace selector
{
NodeLinkNode::NodeLinkNode()
	: AstNode()
{
}

NodeLinkNode::NodeLinkNode(const Offset_t &startOffset, const Offset_t &endOffset, const NodeLinkContainer_t &linkedNodes)
	: AstNode(startOffset, endOffset),
	  m_linkedNodes(linkedNodes)
{
}
	
NodeLinkNode::NodeLinkNode(const Range_t &range, const NodeLinkContainer_t &linkedNodes)
	: AstNode(range),
	  m_linkedNodes(linkedNodes)
{
}

NodeLinkNode::~NodeLinkNode()
{
}

const NodeLinkContainer_t& NodeLinkNode::GetLinkedNodes() const
{
	return m_linkedNodes;
}

NodeLinkContainer_t& NodeLinkNode::GetLinkedNodes()
{
	return m_linkedNodes;
}

void NodeLinkNode::SetLinkedNodes(const NodeLinkContainer_t &linkedNodes)
{
	m_linkedNodes = linkedNodes;
}

void NodeLinkNode::AddLinkedNode(const std::string &nodeName)
{
	m_linkedNodes.emplace_back(nodeName);
}

NodeLinkContainerIterator_t& NodeLinkNode::begin() const
{
	return m_linkedNodes.begin();
}

NodeLinkContainerIterator_t& NodeLinkNode::end() const
{
	return m_linkedNodes.end();
}
}