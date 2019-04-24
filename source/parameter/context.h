#ifndef SELECTOR_PARAMETER_CONTEXT_H_
#define SELECTOR_PARAMETER_CONTEXT_H_

#include "common/profiler.h"
#include "expression.h"
#include "parameter.h"

#include <list>
#include <string>
#include <unordered_map>

namespace selector
{
/**
 * Context class to hold \c Parameter.
 *
 * Contexts can form a hierarchy that can be used to reference parameters.
 */
class Context : public std::enable_shared_from_this<Context>
{
public:
	explicit Context(const std::string &name);

	bool SetParent(std::shared_ptr<Context> new_parent);
	const std::string &Name() const { return m_contextName; }
	const std::unordered_map<std::string, std::shared_ptr<Context>> &Children() { return m_childrenContexts; }
	std::shared_ptr<Context> Parent() { return m_parentContext.lock(); }

	const std::unordered_map<std::string, Parameter> GetParameters() { return m_parameters; }

	template<class T>
	Parameter CreateParameter(const std::string &param_name, const T &initial_value)
	{
		Parameter parameter = initial_value;
		m_parameters.emplace(param_name, parameter);

		return parameter;
	}

	Parameter GetParameter(const std::string &parameter_name);
	std::shared_ptr<Context> GetSubContext(const std::string &context_name);

	/**
	 * Utility method to retrieve and cast a parameter to the intended type.
	 */
	template<class T>
	T GetParameterAs(const std::string &parameter_name)
	{
		return std::get<T>(GetParameter(parameter_name));
	}

private:
	std::string m_contextName;
	std::unordered_map<std::string, Parameter> m_parameters;
	std::weak_ptr<Context> m_parentContext;
	std::unordered_map<std::string, std::shared_ptr<Context>> m_childrenContexts;
};  // class Context

}  // namespace selector

#endif
