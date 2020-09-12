#pragma once

#include <string>

namespace pagoda::common::exception
{
class Exception
{
	public:
	Exception(const std::string exceptionMessage);

	const std::string& What() const;

	private:
	const std::string m_message;
};
}  // namespace pagoda::common::exception
