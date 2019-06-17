#include "parameter_exception.h"

namespace selector
{
ParameterException::ParameterException(const std::string &msg) : std::runtime_error(msg) {}

}