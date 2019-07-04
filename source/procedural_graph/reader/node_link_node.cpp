#include "node_link_node.h"

namespace selector
{
NodeLinkNode::NodeLinkNode() : GraphStatementNode() {}

NodeLinkNode::NodeLinkNode(const NodeLinkContainer_t &linkedNodes) : m_linkedNodes(linkedNodes) {}

NodeLinkNode::NodeLinkNode(const AstNode::Offset_t &startOffset, const AstNode::Offset_t &endOffset,
                           const NodeLinkContainer_t &linkedNodes)
    : GraphStatementNode(startOffset, endOffset), m_linkedNodes(linkedNodes)
{
}

NodeLinkNode::NodeLinkNode(const AstNode::NodeRange_t &range, const NodeLinkContainer_t &linkedNodes)
    : GraphStatementNode(range), m_linkedNodes(linkedNodes)
{
}

NodeLinkNode::~NodeLinkNode() {}

const NodeLinkNode::NodeLinkContainer_t &NodeLinkNode::GetLinkedNodes() const { return m_linkedNodes; }

NodeLinkNode::NodeLinkContainer_t &NodeLinkNode::GetLinkedNodes() { return m_linkedNodes; }

void NodeLinkNode::SetLinkedNodes(const NodeLinkContainer_t &linkedNodes) { m_linkedNodes = linkedNodes; }

void NodeLinkNode::AddLinkedNode(const std::string &nodeName) { m_linkedNodes.emplace_back(nodeName); }

NodeLinkNode::NodeLinkContainerIterator_t NodeLinkNode::begin() { return m_linkedNodes.begin(); }

NodeLinkNode::NodeLinkContainerIterator_t NodeLinkNode::end() { return m_linkedNodes.end(); }

void NodeLinkNode::AcceptVisitor(AstNodeVisitor *visitor)
{
	visitor->Visit(std::dynamic_pointer_cast<NodeLinkNode>(shared_from_this()).get());
}
}  // namespace selector
