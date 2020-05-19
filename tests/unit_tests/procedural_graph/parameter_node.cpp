#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/procedural_graph/parameter_node.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::dynamic;

TEST(ParameterNode, when_executing_a_parameter_node_should_propagate_the_parameters_to_its_outnodes)
{
	auto p = std::make_shared<ParameterNode>();
	NodeSet<Node> outNodes{std::make_shared<ParameterNode>(), std::make_shared<ParameterNode>()};
	NodeSet<Node> emptyNodeSet;

	p->RegisterMember("a", std::make_shared<FloatValue>(123.0f));
	p->Execute(emptyNodeSet, outNodes);

	for (auto &n : outNodes)
	{
		EXPECT_EQ(get_value_as<float>(*(n->GetMember("a"))), 123.0f);
	}
}

TEST(ParameterNode, when_executing_a_parameter_node_should_overwrite_parameters_in_out_nodes)
{
	auto p = std::make_shared<ParameterNode>();
	p->RegisterMember("a", std::make_shared<FloatValue>(123.0f));

	auto p2 = std::make_shared<ParameterNode>();
	p2->RegisterMember("a", std::make_shared<FloatValue>(0.0f));

	NodeSet<Node> emptyNodeSet;
	NodeSet<Node> outNodes{p2};

	p->Execute(emptyNodeSet, outNodes);

	EXPECT_EQ(get_value_as<float>(*(p2->GetMember("a"))), 123.0f);
}
