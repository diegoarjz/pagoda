#include <pagoda/graph/io/node_link_definition.h>
#include <memory>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::io;

TEST(NodeLinkDefinitionTest, should_store_the_right_node_names)
{
	NodeLinkDefinition n{"node", "in", "out"};
	EXPECT_EQ(n.GetNodeName(), "node");
	EXPECT_EQ(n.GetInputInterface(), "in");
	EXPECT_EQ(n.GetOutputInterface(), "out");
}

TEST(NodeLinkDefinitionTest, should_return_the_right_in_out_nodes)
{
	NodeLinkDefinition n{"node", "in", "out"};
	EXPECT_EQ(n.GetInNode(), "node_in");
	EXPECT_EQ(n.GetOutNode(), "node_out");

	NodeLinkDefinition n2{"node", "", "out"};
	EXPECT_EQ(n2.GetInNode(), "node");
	EXPECT_EQ(n2.GetOutNode(), "node_out");

	NodeLinkDefinition n3{"node", "in", ""};
	EXPECT_EQ(n3.GetInNode(), "node_in");
	EXPECT_EQ(n3.GetOutNode(), "node");

	NodeLinkDefinition n4{"node", "", ""};
	EXPECT_EQ(n4.GetInNode(), "node");
	EXPECT_EQ(n4.GetOutNode(), "node");
}
