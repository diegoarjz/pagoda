#ifndef SELECTOR_PARAMETER_PARAMETER_EXCEPTION_H_
#define SELECTOR_PARAMETER_PARAMETER_EXCEPTION_H_

#include <exception>
#include <stdexcept>

namespace selector
{
/**
 * Exception thrown whenever for invalid operations with \c Parameter.
 */
class ParameterException : public std::runtime_error
{
public:
	ParameterException(const std::string &msg);
};

}  // namespace selector

#endif
