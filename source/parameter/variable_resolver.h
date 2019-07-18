#ifndef SELECTOR_PARAMETER_VARIABLE_RESOLVER_H_
#define SELECTOR_PARAMETER_VARIABLE_RESOLVER_H_

#include "parameter.h"
#include "variable.h"

namespace selector
{
/**
 * Resolves a \c Parameter value given a \c Variable.
 * Walks through the hierarchy of \c IParameterizable within a \c Parameter
 */
class VariableResolver
{
public:
	VariableResolver(const Variable &v);
	~VariableResolver();
	
	Parameter Resolve(IParameterizablePtr parameterizable);

private:
	Variable m_variable;
};
}

#endif