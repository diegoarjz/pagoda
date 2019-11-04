#include "node.h"

#include "../dynamic_value/type_info.h"
#include "common/assertions.h"
#include "parameter/parameter.h"

namespace selector
{
const TypeInfoPtr Node::s_typeInfo = std::make_shared<TypeInfo>("Node");

Node::Node() : DynamicValueBase(s_typeInfo), ClassBase("Node"), m_nodeName(""), m_nodeId(0) {}
Node::~Node() {}

uint32_t Node::GetId() const { return m_nodeId; }
void Node::SetId(uint32_t nodeId) { m_nodeId = nodeId; }

void Node::SetName(const std::string &name) { m_nodeName = name; }
const std::string &Node::GetName() const { return m_nodeName; }

void Node::SetExecutionArguments(const std::unordered_map<std::string, DynamicValueBasePtr> &arguments)
{
	for (const auto &arg : arguments)
	{
		RegisterMember(arg.first, arg.second);
	}
}

void Node::SetExpressionVariables() { throw std::runtime_error("Unimplemented"); /*m_context->UpdateExpressions();*/ }

std::string Node::ToString() const { return "<Node>"; }

void Node::AcceptVisitor(ValueVisitorBase &visitor) { throw std::runtime_error("Unimplemented"); }

}  // namespace selector
