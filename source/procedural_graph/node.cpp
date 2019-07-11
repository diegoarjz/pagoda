#include "node.h"

#include "common/assertions.h"
#include "parameter/parameter.h"

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

void Node::SetExecutionArguments(const std::unordered_map<std::string, Parameter> &arguments)
{
	DBG_ASSERT_MSG(m_context != nullptr, "m_context is not set on Node");
	for (const auto &arg : arguments)
	{
		m_context->SetParameter(arg.first, arg.second);
	}
}

void Node::SetExpressionVariables()
{
	for (auto &p : m_context->GetParameters())
	{
		// TODO: Should not rely on the exception
		try
		{
			auto e = get_parameter_as<ExpressionPtr>(p.second);
			for (const auto &var : e->GetVariables())
			{
				auto parameter = m_context->GetParameter(var);
				e->SetVariableValue(var, parameter);
			}
		}
		catch (...)
		{
			continue;
		}
	}
}

}  // namespace selector
