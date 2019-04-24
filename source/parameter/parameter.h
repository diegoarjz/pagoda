#ifndef SELECTOR_PARAMETER_PARAMETER_H_
#define SELECTOR_PARAMETER_PARAMETER_H_

#include "expression.h"

#include <string>
#include <variant>

namespace selector
{
using Parameter = std::variant<float, std::string>;
}  // namespace selector

#endif
