#include "parameter_identifier.h"

#include "common/profiler.h"
#include "parameter_validation.h"

namespace selector
{
bool ParameterIdentifier::IsValidIdentifier(const std::string &identifier)
{
	START_PROFILE;

	return std::regex_match(identifier, ParameterValidation::identifier_regex);
}

std::pair<bool, ParameterIdentifier> ParameterIdentifier::CreateIdentifier(const std::string &identifier)
{
	START_PROFILE;

	if (IsValidIdentifier(identifier))
	{
		return std::make_pair(true, ParameterIdentifier(identifier));
	}
	else
	{
		return std::make_pair(false, ParameterIdentifier(""));
	}
}

ParameterIdentifier::ParameterIdentifier(const ParameterIdentifier &other) : m_identifier(other.m_identifier) {}

ParameterIdentifier &ParameterIdentifier::operator=(const ParameterIdentifier &other)
{
	m_identifier = other.m_identifier;
	return *this;
}

ParameterIdentifier::ParameterIdentifier(const std::string &identifier) : m_identifier(identifier) {}

const std::string &ParameterIdentifier::GetIdentifier() const { return m_identifier; }

}  // namespace selector
