#ifndef SELECTOR_PARAMETER_PARAMETER_IDENTIFIER_H_
#define SELECTOR_PARAMETER_PARAMETER_IDENTIFIER_H_

#include <regex>

namespace selector
{
class ParameterIdentifier
{
public:
	static std::pair<bool, ParameterIdentifier> CreateIdentifier(const std::string &identifier);
	static bool IsValidIdentifier(const std::string &identifier);

	ParameterIdentifier(const ParameterIdentifier &other);
	ParameterIdentifier &operator=(const ParameterIdentifier &other);

	const std::string &GetIdentifier() const;

private:
	explicit ParameterIdentifier(const std::string &identifier);

	std::string m_identifier;

};  // class ParameterIdentifier

}  // namespace selector

#endif
