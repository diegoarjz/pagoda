#ifndef PAGODA_PARAMETER_VARIABLE_RESOLVER_H_
#define PAGODA_PARAMETER_VARIABLE_RESOLVER_H_

#include "variable.h"

#include <memory>

namespace pagoda
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
}  // namespace pagoda

#endif
