#include "node.h"

namespace selector
{
Node::Node() : m_nodeName(""), m_nodeId(0), m_context(nullptr) {}
Node::~Node() {}

uint32_t Node::GetId() const { return m_nodeId; }
void Node::SetId(uint32_t nodeId) { m_nodeId = nodeId; }

void Node::SetName(const std::string &name) { m_nodeName = name; }
const std::string &Node::GetName() const { return m_nodeName; }

void Node::SetParameterContext(std::shared_ptr<Context> context) { m_context = context; }
std::shared_ptr<Context> Node::GetParameterContext() const { return m_context; }
}  // namespace selector
