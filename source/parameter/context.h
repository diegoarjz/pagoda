#ifndef SELECTOR_PARAMETER_CONTEXT_H_
#define SELECTOR_PARAMETER_CONTEXT_H_

#include "common/profiler.h"
#include "expression.h"
#include "parameter_identifier.h"

#include <list>
#include <string>
#include <unordered_map>

namespace selector
{
class ParameterBase;
template<class T>
class Parameter;

/**
 * Context class to hold \c Parameter.
 *
 * Contexts can form a hierarchy that can be used to reference parameters.
 */
class Context : public std::enable_shared_from_this<Context>
{
public:
	explicit Context(const ParameterIdentifier &name);

	bool SetParent(std::shared_ptr<Context> new_parent);
	const std::string &Name() const { return m_contextName; }
	const std::unordered_map<std::string, std::shared_ptr<Context>> &Children() { return m_childrenContexts; }
	std::shared_ptr<Context> Parent() { return m_parentContext.lock(); }

	const std::unordered_map<std::string, std::shared_ptr<ParameterBase>> GetParameters() { return m_parameters; }

	template<class T>
	std::shared_ptr<typename T::parameter_t> CreateParameter(
	    const std::string &param_name, const typename T::value_t &initial_value = typename T::value_t())
	{
		START_PROFILE;
		std::shared_ptr<typename T::parameter_t> parameter = CreateAndValidateParameter<T>(param_name);
		if (parameter == nullptr)
		{
			return parameter;
		}
		parameter->SetValue(initial_value);
		return parameter;
	}

	template<class T>
	std::shared_ptr<typename T::parameter_t> CreateParameter(const std::string &param_name,
	                                                         std::shared_ptr<Expression> &expression)
	{
		START_PROFILE;

		std::shared_ptr<typename T::parameter_t> parameter = CreateAndValidateParameter<T>(param_name);
		if (parameter == nullptr)
		{
			return parameter;
		}
		parameter->SetExpression(expression);
		return parameter;
	}

	std::shared_ptr<ParameterBase> GetParameter(const std::string &parameter_name);
	std::shared_ptr<Context> GetSubContext(const std::string &context_name);

	/**
	 * Utility method to retrieve and cast a parameter to the intended type.
	 */
	template<class ParameterType>
	std::shared_ptr<Parameter<ParameterType>> GetParameterAs(const std::string &parameter_name)
	{
		return std::dynamic_pointer_cast<Parameter<ParameterType>>(GetParameter(parameter_name));
	}

private:
	template<class T>
	std::shared_ptr<typename T::parameter_t> CreateAndValidateParameter(const std::string &param_name)
	{
		START_PROFILE;

		// Don't create a parameter if there is a context with the same name
		if (m_childrenContexts.find(param_name) != std::end(m_childrenContexts))
		{
			return nullptr;
		}

		auto parameter = std::make_shared<typename T::parameter_t>(param_name);
		auto inserted = m_parameters.emplace(param_name, parameter);

		if (inserted.second)
		{
			return parameter;
		}
		else
		{
			return nullptr;
		}
	}

	ParameterIdentifier m_contextName;
	std::unordered_map<std::string, std::shared_ptr<ParameterBase>> m_parameters;
	std::weak_ptr<Context> m_parentContext;
	std::unordered_map<std::string, std::shared_ptr<Context>> m_childrenContexts;
};  // class Context

}  // namespace selector

#endif
