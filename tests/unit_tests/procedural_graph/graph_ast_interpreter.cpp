#include <procedural_graph/reader/graph_definition_node.h>
#include <procedural_graph/reader/named_argument.h>
#include <procedural_graph/reader/node_definition_node.h>
#include <procedural_graph/reader/node_link_node.h>
#include <procedural_graph/reader/ast_interpreter.h>

#include <procedural_graph/graph.h>

#include <gtest/gtest.h>

using namespace selector;

class AstInterpreterTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_graph = std::make_shared<Graph>();
	}
	
	void TearDown()
	{
	}
	
	GraphPtr m_graph;
};

