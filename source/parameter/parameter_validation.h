#ifndef SELECTOR_PARAMETER_PARAMETER_VALIDATION_H_
#define SELECTOR_PARAMETER_PARAMETER_VALIDATION_H_

#include <regex>
#include <string>

namespace selector
{
class ParameterValidation
{
public:
	static const std::regex identifier_regex;
	static const std::regex path_regex;
	static const std::regex variable_regex;
};  // class ParameterValidation

}  // namespace selector
#endif
