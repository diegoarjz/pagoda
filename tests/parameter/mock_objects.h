#ifndef SELECTOR_TESTS_PARAMETER_MOCK_OBJECTS_H_
#define SELECTOR_TESTS_PARAMETER_MOCK_OBJECTS_H_

#include <parameter/context.h>

#include <gmock/gmock.h>

using namespace selector;

class ContextMock : public Context
{
public:
	ContextMock(const std::string &identifier) : Context(identifier) {}
	MOCK_METHOD1(GetParameter, std::shared_ptr<Parameter>(const std::string &));
};

#endif
