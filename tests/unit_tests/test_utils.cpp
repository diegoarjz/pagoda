#include "test_utils.h"

bool g_asserted = false;
selector::Fail::FailBehaviour AssertExpected(const char *condition, const char *file, const int line,
                                             const char *message)
{
	g_asserted = true;
	return selector::Fail::FailBehaviour::Continue;
}

ExpectAssert::ExpectAssert()
{
	g_asserted = false;
	selector::Fail::SetFailHandler(AssertExpected);
}

ExpectAssert::~ExpectAssert()
{
	g_asserted = false;
	selector::Fail::SetFailHandler(selector::DefaultHandler);
}

bool ExpectAssert::Asserted() { return g_asserted; }
