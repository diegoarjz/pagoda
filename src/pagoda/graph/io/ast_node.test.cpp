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

/* ***********
 * Ast Node  *
 * **********/
class StubAstNode : public AstNode
{
	public:
	StubAstNode() : AstNode() {}

	StubAstNode(const AstNode::Offset_t &startOffset, const AstNode::Offset_t &endOffset)
	  : AstNode(startOffset, endOffset)
	{
	}

	StubAstNode(const AstNode::NodeRange_t &range) : AstNode(range) {}

	void AcceptVisitor(AstNodeVisitor *visitor) override {}
};

class AstNodeTest : public ::testing::Test
{
	protected:
	void SetUp() {}

	void TearDown() {}
};

TEST_F(AstNodeTest, when_using_the_default_constructor_should_set_the_offsets_to_zero)
{
	StubAstNode n;
	EXPECT_EQ(n.GetStartOffset(), 0);
	EXPECT_EQ(n.GetEndOffset(), 0);
}

TEST_F(AstNodeTest, when_constructing_with_offset_should_set_the_offsets)
{
	StubAstNode n(10, 15);
	EXPECT_EQ(n.GetStartOffset(), 10);
	EXPECT_EQ(n.GetEndOffset(), 15);
}

TEST_F(AstNodeTest, when_constructing_with_range_should_set_the_offsets)
{
	StubAstNode n(std::make_pair(10, 15));
	EXPECT_EQ(n.GetStartOffset(), 10);
	EXPECT_EQ(n.GetEndOffset(), 15);
}

TEST_F(AstNodeTest, when_setting_the_range_should_set_the_offsets)
{
	StubAstNode n(std::make_pair(15, 10));
	n.SetNodeRange(std::make_pair(10, 15));
	EXPECT_EQ(n.GetStartOffset(), 10);
	EXPECT_EQ(n.GetEndOffset(), 15);
}

TEST_F(AstNodeTest, when_getting_the_range_should_get_a_correct_range)
{
	auto range = std::make_pair(std::size_t(15), std::size_t(10));
	StubAstNode n(range);
	EXPECT_EQ(n.GetNodeRange(), range);
}

