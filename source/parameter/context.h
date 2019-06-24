#ifndef SELECTOR_PARAMETER_CONTEXT_H_
#define SELECTOR_PARAMETER_CONTEXT_H_

#include "common/profiler.h"
#include "expression.h"
#include "parameter.h"
#include "parameterizable.h"

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
class Context : public IParameterizable, public std::enable_shared_from_this<Context>
{
public:
	/**
	 * Creates an instance of a \c Context with the given \p name.
	 */
	explicit Context(const std::string &name);

	virtual ~Context();

	/**
	 * Sets this \c Context parent to \p new_parent.
	 */
	bool SetParent(std::shared_ptr<Context> new_parent);
	/**
	 * Returns the name of this \c Context.
	 */
	const std::string &Name() const { return m_contextName; }
	/**
	 * Returns a map from context name to all children instances of \c Context.
	 */
	const std::unordered_map<std::string, std::shared_ptr<Context>> &Children() { return m_childrenContexts; }
	/**
	 * Returns the parent \c Context.
	 */
	std::shared_ptr<Context> Parent() { return m_parentContext.lock(); }
	/**
	 * Returns the children \c Context with the given \p context_name.
	 */
	std::shared_ptr<Context> GetSubContext(const std::string &context_name);

	/**
	 * Returns a \c Parameter based on its \p parameterName.
	 */
	Parameter GetParameter(const std::string &parameterName) override;
	/**
	 * Sets the \c Parameter with \p parameterName to the value given by \p parameter.
	 */
	void SetParameter(const std::string &parameterName, const Parameter &parameter) override;
	/**
	 * Returns the list of names of all exposed \c Parameter.
	 */
	std::unordered_set<std::string> GetParameterNameList() const override;
	/**
	 * Returns all exposed \c Parameter.
	 */
	std::unordered_map<std::string, Parameter> GetParameters() const override;

	/**
	 * Creates, stores and returns a \c Parameter with the given \p param_name and \p initial_value.
	 */
	template<class T>
	Parameter CreateParameter(const std::string &param_name, const T &initial_value)
	{
		Parameter parameter = initial_value;
		m_parameters.emplace(param_name, parameter);

		return parameter;
	}

private:
	std::string m_contextName;
	std::unordered_map<std::string, Parameter> m_parameters;
	std::weak_ptr<Context> m_parentContext;
	std::unordered_map<std::string, std::shared_ptr<Context>> m_childrenContexts;
};  // class Context

}  // namespace selector

#endif
