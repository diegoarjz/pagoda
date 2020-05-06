#include "assertions.h"

#include <cstdarg>
#include <cstdio>

namespace pagoda
{
typename Fail::FailBehaviour DefaultHandler(const char *condition, const char *file, const int line,
                                            const char *message)
{
	std::printf("%s:%d - ASSERT FAILED -\n", file, line);

	if (condition != nullptr)
	{
		std::printf("\tCondition: %s\n", condition);
	}
	if (message != nullptr)
	{
		std::printf("\t%s\n", message);
	}

	return Fail::FailBehaviour::Halt;
}

Fail::FailHandler Fail::sFailHandler = DefaultHandler;

void Fail::SetFailHandler(const FailHandler &handler) { sFailHandler = handler; }

typename Fail::FailHandler Fail::GetFailHandler() { return sFailHandler; }

typename Fail::FailBehaviour Fail::ReportFail(const char *condition, const char *file, const int line,
                                              const char *message, ...)
{
	const char *final_message = nullptr;
	if (message != nullptr)
	{
		char messageBuffer[1024];
		{
			va_list args;
			va_start(args, message);
			vsnprintf(messageBuffer, 1024, message, args);
			va_end(args);
		}

		final_message = messageBuffer;
	}
	return sFailHandler(condition, file, line, final_message);
}

}  // namespace pagoda
