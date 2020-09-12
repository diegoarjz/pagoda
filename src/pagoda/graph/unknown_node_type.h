#pragma once

#include <pagoda/common/exception/exception.h>

#include <string>

namespace pagoda::graph
{
class UnknownNodeTypeException : public common::exception::Exception
{
	public:
	UnknownNodeTypeException(const std::string &nodeType);
};
}  // namespace pagoda::graph
