#include <pagoda/graph/io/graph_definition_node.h>
#include <pagoda/graph/io/named_argument.h>
#include <pagoda/graph/io/node_definition_node.h>
#include <pagoda/graph/io/node_link_definition.h>
#include <pagoda/graph/io/node_link_node.h>
#include <memory>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::io;

/* ************************
 * Graph Definition Node  *
 * ***********************/
class GraphDefinitionNodeTest : public ::testing::Test
{
	protected:
	void SetUp() { m_statements = {std::make_shared<NodeDefinitionNode>(), std::make_shared<NodeLinkNode>()}; }
	void TearDown() {}

	void expectEqualGraphStatements(const std::vector<GraphStatementNodePtr> &actual,
	                                const std::vector<GraphStatementNodePtr> &expected)
	{
		ASSERT_EQ(actual.size(), expected.size());
		for (auto i = 0u; i < actual.size(); ++i) {
			EXPECT_EQ(actual[i], expected[i]);
		}
	}

	std::vector<GraphStatementNodePtr> m_statements;
};

TEST_F(GraphDefinitionNodeTest, when_constructing_with_statements_should_set_the_graph_statements)
{
	auto g = std::make_shared<GraphDefinitionNode>(m_statements);
	expectEqualGraphStatements(g->GetGraphStatementNodes(), m_statements);
}

TEST_F(GraphDefinitionNodeTest, when_adding_statements_should_store_the_graph_statements)
{
	auto g = std::make_shared<GraphDefinitionNode>();
	for (const auto &s : m_statements) {
		g->AddGraphStatementNode(s);
	}
	expectEqualGraphStatements(g->GetGraphStatementNodes(), m_statements);
}

TEST_F(GraphDefinitionNodeTest, when_setting_statements_should_store_the_graph_statements)
{
	auto g = std::make_shared<GraphDefinitionNode>();
	g->SetGraphStatementNodes(m_statements);
	expectEqualGraphStatements(g->GetGraphStatementNodes(), m_statements);
}

TEST_F(GraphDefinitionNodeTest, when_creating_a_node_definition_node_should_return_and_store_it)
{
	auto g = std::make_shared<GraphDefinitionNode>();
	auto n = g->CreateNodeDefinitionNode("name", "nodeType", std::vector<NamedArgumentPtr>{});
	expectEqualGraphStatements(g->GetGraphStatementNodes(), {n});
}

TEST_F(GraphDefinitionNodeTest, when_creating_a_node_link_node_should_return_and_store_it)
{
	auto g = std::make_shared<GraphDefinitionNode>();
	auto n = g->CreateNodeLinkNode();
	expectEqualGraphStatements(g->GetGraphStatementNodes(), {n});
}

TEST_F(GraphDefinitionNodeTest, when_iterating_over_a_graph_definition_node_should_iterate_over_the_stored_statemets)
{
	auto g = std::make_shared<GraphDefinitionNode>(m_statements);
	std::vector<GraphStatementNodePtr> statementNodes(g->begin(), g->end());  // Does the same thing as iterating
	expectEqualGraphStatements(statementNodes, m_statements);
}

