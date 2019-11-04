#ifndef SELECTOR_PARAMETER_VARIABLE_RESOLVER_H_
#define SELECTOR_PARAMETER_VARIABLE_RESOLVER_H_

#include "parameter.h"
#include "variable.h"

namespace selector
{
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;

/**
 * Resolves a \c DynamicValueBasePtr value given a \c Variable.
 * Walks through the hierarchy of \c DynamicValueBasePtr within a \c DynamicValueBasePtr
 */
class VariableResolver
{
public:
	VariableResolver(const Variable &v);
	~VariableResolver();

	DynamicValueBasePtr Resolve(DynamicValueBasePtr parameterizable);

private:
	Variable m_variable;
};
}  // namespace selector

#endif
