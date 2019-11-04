#pragma once

#include <stdexcept>
#include <string>

namespace selector
{
class ValueNotFoundException : public std::runtime_error
{
public:
	ValueNotFoundException(const std::string &name);
};
}  // namespace selector
