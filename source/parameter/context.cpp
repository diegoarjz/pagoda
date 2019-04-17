#include "context.h"

namespace selector
{
Context::Context(const ParameterIdentifier &name) : m_contextName(name) {}

bool Context::SetParent(std::shared_ptr<Context> new_parent)
{
	// Validate that the new_parent doesn't already have a context with
	// the same name as this.
	if (new_parent != nullptr &&
	    (new_parent->m_childrenContexts.find(m_contextName) != std::end(new_parent->m_childrenContexts) ||
	     new_parent->m_parameters.find(m_contextName) != std::end(new_parent->m_parameters)))
	{
		return false;
	}

	if (m_parentContext.lock() != nullptr)
	{
		m_parentContext.lock()->m_childrenContexts.erase(m_contextName);
	}

	m_parentContext = new_parent;

	if (new_parent != nullptr)
	{
		new_parent->m_childrenContexts.emplace(m_contextName, shared_from_this());
	}

	return true;
}

Parameter Context::GetParameter(const std::string &parameter_name)
{
	throw std::runtime_error("Re-implemented.");
	auto iter = m_parameters.find(parameter_name);

	if (iter == std::end(m_parameters))
	{
		return Parameter{};
	}
	else
	{
		return iter->second;
	}
}

std::shared_ptr<Context> Context::GetSubContext(const std::string &context_name)
{
	// TODO: unit test this function
	auto iter = m_childrenContexts.find(context_name);
	if (iter == m_childrenContexts.end())
	{
		return nullptr;
	}

	return iter->second;
}
}  // namespace selector
