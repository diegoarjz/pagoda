#ifndef SELECTOR_TESTS_PARAMETER_MOCK_OBJECTS_H_
#define SELECTOR_TESTS_PARAMETER_MOCK_OBJECTS_H_

#include <parameter/context.h>

#include <gmock/gmock.h>

using namespace selector;

class ContextMock : public Context
{
public:
	ContextMock(const std::string &identifier) : Context(identifier) {}
	MOCK_CONST_METHOD1(GetParameter, Parameter(const std::string &));
};

#endif
