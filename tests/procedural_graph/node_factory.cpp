#include <procedural_graph/node_factory.h>
#include <procedural_graph/node_type.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(NodeFactory, when_creating_with_node_type_enum_should_return_the_same_type_of_node)
{
	std::shared_ptr<NodeFactory> factory = std::make_shared<NodeFactory>();

	for (uint32_t i = 0; i < static_cast<uint32_t>(NodeType::MAX); ++i)
	{
		NodePtr node = factory->Create(static_cast<NodeType>(i));
		EXPECT_EQ(node->GetNodeType(), static_cast<NodeType>(i));
	}
}
