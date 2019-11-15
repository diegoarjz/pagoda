#include "node.h"

#include "common/assertions.h"
#include "dynamic_value/type_info.h"
#include "dynamic_value/value_not_found.h"
#include "dynamic_value/expression.h"

namespace selector
{
const TypeInfoPtr Node::s_typeInfo = std::make_shared<TypeInfo>("Node");

Node::Node() : BuiltinClass(s_typeInfo), m_nodeName(""), m_nodeId(0) {}
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

void Node::SetExpressionVariables()
{
	for (auto parIter = GetMembersBegin(); parIter != GetMembersEnd(); ++parIter)
	{
		ExpressionPtr e = std::dynamic_pointer_cast<Expression>(parIter->second.m_value);
		if (e != nullptr)
		{
			for (const auto &var : e->GetVariables())
			{
				try
				{
					auto variableIdentifiers = var.GetIdentifiers();
					DynamicValueBasePtr nodeParameter = GetMember(variableIdentifiers.front());
					e->SetVariableValue(variableIdentifiers.front(), nodeParameter);
				}
				catch (ValueNotFoundException &e)
				{
					LOG_TRACE(ProceduralGraph, "Operation parameter %s not found in Node %s", parIter->first.c_str(),
					          GetName().c_str());
				}
			}
		}
	}
}

std::string Node::ToString() const { return "<Node>"; }

void Node::AcceptVisitor(ValueVisitorBase &visitor) { throw std::runtime_error("Unimplemented"); }

}  // namespace selector
