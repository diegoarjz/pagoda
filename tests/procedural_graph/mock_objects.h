#ifndef SELECTOR_TESTS_PROCEDURAL_GRAPH_MOCK_OBJECTS_H_
#define SELECTOR_TESTS_PROCEDURAL_GRAPH_MOCK_OBJECTS_H_

#include <procedural_graph/node_execution.h>
#include <procedural_graph/node_factory.h>

#include <gmock/gmock.h>

using namespace selector;

class MockNodeFactory : public NodeFactory
{
public:
	MOCK_CONST_METHOD2(Create, NodePtr(NodeType, const std::string&));
};

class MockNodeExecution : public NodeExecution
{
public:
	MOCK_METHOD2(Execute, NodeExecutionResultPtr(NodePtr, GraphExecutionContextPtr));
};
#endif
