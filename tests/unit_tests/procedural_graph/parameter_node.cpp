#include <parameter/context.h>
#include <parameter/parameter.h>
#include <procedural_graph/parameter_node.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(ParameterNode, when_executing_a_parameter_node_should_propagate_the_parameters_to_its_outnodes)
{
	auto p = std::make_shared<ParameterNode>();
	p->SetParameterContext(std::make_shared<Context>("ctx"));
	NodeSet<Node> outNodes{std::make_shared<ParameterNode>(), std::make_shared<ParameterNode>()};
	NodeSet<Node> emptyNodeSet;

	for (auto &n : outNodes)
	{
		n->SetParameterContext(std::make_shared<Context>("ctx"));
	}

	p->GetParameterContext()->SetParameter("a", 123.0f);
	p->GetParameterContext()->SetParameter("b", 0.0f);

	p->Execute(emptyNodeSet, outNodes);

	for (auto &n : outNodes)
	{
		EXPECT_EQ(n->GetParameterContext()->GetParameterAs<float>("a"), 123.0f);
		EXPECT_EQ(n->GetParameterContext()->GetParameterAs<float>("b"), 0.0f);
	}
}

TEST(ParameterNode, when_executing_a_parameter_node_should_overwrite_parameters_in_out_nodes)
{
	auto p = std::make_shared<ParameterNode>();
	p->SetParameterContext(std::make_shared<Context>("ctx"));
	p->GetParameterContext()->SetParameter("a", 123.0f);

	auto p2 = std::make_shared<ParameterNode>();
	p2->SetParameterContext(std::make_shared<Context>("ctx"));
	p2->GetParameterContext()->SetParameter("a", 0.0f);

	NodeSet<Node> outNodes{p2};
	NodeSet<Node> emptyNodeSet;

	p->Execute(emptyNodeSet, outNodes);

	EXPECT_EQ(p2->GetParameterContext()->GetParameterAs<float>("a"), 123.0f);
}
