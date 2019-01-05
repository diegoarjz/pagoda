#include "parameter_validation.h"

namespace selector
{
#define IDENTIFIER_REGEX "[_a-zA-Z][_a-zA-Z0-9]*"
#define PATH_VARIABLE "(/|\\.{1,2}/)?(\\.{1,2}/|" IDENTIFIER_REGEX "/)*(" IDENTIFIER_REGEX ")"
#define PATH_REGEX "^" PATH_VARIABLE "$"
#define VARIABLE_REGEX "\\$\\{(" PATH_VARIABLE ")\\}"

const std::regex ParameterValidation::identifier_regex(IDENTIFIER_REGEX);
const std::regex ParameterValidation::path_regex(PATH_REGEX);
const std::regex ParameterValidation::variable_regex(VARIABLE_REGEX);

}  // namespace selector
