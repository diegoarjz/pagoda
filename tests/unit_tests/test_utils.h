#ifndef SELECTOR_TEST_TEST_UTILS_H_
#define SELECTOR_TEST_TEST_UTILS_H_

#include <common/assertions.h>

selector::Fail::FailBehaviour AssertExpected(const char *condition, const char *file, const int line,
                                             const char *message);
struct ExpectAssert
{
	ExpectAssert();
	~ExpectAssert();
	static bool Asserted();
};

#endif
