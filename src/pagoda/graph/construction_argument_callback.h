#pragma once

#include <string>

namespace pagoda::graph
{
class ConstructionArgumentCallback
{
	public:
	virtual void StringArgument(const char* const name, std::string& arg, const char* const label) = 0;
};
}  // namespace pagoda::graph
