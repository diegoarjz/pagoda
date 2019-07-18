#ifndef SELECTOR_PARAMETER_PARAMETERIZABLE_H_
#define SELECTOR_PARAMETER_PARAMETERIZABLE_H_

#include "parameter.h"
#include "variable.h"

#include <unordered_map>
#include <unordered_set>

namespace selector
{
/**
 * Interface to a class that exposes parameters.
 *
 * Makes no assumption about the parameter storage.
 */
class IParameterizable
{
public:
	/**
	 * Returns a \c Parameter based on its \p parameterName.
	 */
	virtual Parameter GetParameter(const std::string &parameterName) const = 0;
	/**
	 * Sets the \c Parameter with \p parameterName to the value given by \p parameter.
	 */
	virtual void SetParameter(const std::string &parameterName, const Parameter &parameter) = 0;
	/**
	 * Returns the list of names of all exposed \c Parameter.
	 */
	virtual std::unordered_set<std::string> GetParameterNameList() const = 0;
	/**
	 * Returns all exposed \c Parameter.
	 */
	virtual std::unordered_map<std::string, Parameter> GetParameters() const = 0;
	/**
	 * Resolves a \c Variable within the hierarchy of \c IParameterizable.
	 */
	virtual Parameter ResolveVariable(const Variable &v) const = 0;

	/**
	 * Utility method to retrieve and cast a parameter to the intended type.
	 */
	template<class T>
	T GetParameterAs(const std::string &parameter_name)
	{
		return get_parameter_as<T>(GetParameter(parameter_name));
	}
};
}  // namespace selector

#endif
