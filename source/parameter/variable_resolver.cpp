#include "variable_resolver.h"

namespace selector
{
VariableResolver::VariableResolver(const Variable &v) : m_variable(v)
{
}

VariableResolver::~VariableResolver()
{
}

Parameter VariableResolver::Resolve(IParameterizablePtr parameterizable)
{
	const auto& variableIdentifiers = m_variable.GetIdentifiers();
	auto identifierIter = variableIdentifiers.begin();
	
	Parameter currentParameter = parameterizable;
	for (std::size_t i = 0; i < variableIdentifiers.size() - 1; ++i, ++identifierIter)
	{
		currentParameter = std::get<IParameterizablePtr>(currentParameter)->GetParameter(*identifierIter);
	}
	
	return std::get<IParameterizablePtr>(currentParameter)->GetParameter(*identifierIter);
}
}