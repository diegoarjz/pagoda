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

/* ****************
 * Node Link Node *
 * ***************/

class NodeLinkNodeTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		m_linkedNodes = {std::make_shared<NodeLinkDefinition>("n1", "", ""),
		                 std::make_shared<NodeLinkDefinition>("n2", "", ""),
		                 std::make_shared<NodeLinkDefinition>("n3", "", "")};
	}

	void TearDown() {}

	void expectEqualLinkedNodes(const NodeLinkNode::NodeLinkContainer_t &t1, const NodeLinkNode::NodeLinkContainer_t &t2)
	{
		ASSERT_EQ(t1.size(), t2.size());
		auto eq = [&](std::shared_ptr<NodeLinkDefinition> lhs, std::shared_ptr<NodeLinkDefinition> rhs) {
			return lhs->GetInputInterface() == rhs->GetInputInterface() && lhs->GetNodeName() == rhs->GetNodeName() &&
			       lhs->GetOutputInterface() == rhs->GetOutputInterface();
		};

		auto t1Iter = std::begin(t1);
		auto t2Iter = std::begin(t2);
		for (auto i = 0u; i < t1.size(); ++i, ++t1Iter, ++t2Iter) {
			EXPECT_TRUE(eq(*t1Iter, *t2Iter));
		}
	}

	NodeLinkNode::NodeLinkContainer_t m_linkedNodes;
};

TEST_F(NodeLinkNodeTest, when_constructing_with_offsets_and_linked_nodes_should_store_them)
{
	auto n = std::make_shared<NodeLinkNode>(0, 1, m_linkedNodes);
	expectEqualLinkedNodes(n->GetLinkedNodes(), m_linkedNodes);
}

TEST_F(NodeLinkNodeTest, when_constructing_with_range_and_linked_nodes_should_store_them)
{
	auto n = std::make_shared<NodeLinkNode>(std::make_pair(std::size_t(0), std::size_t(1)), m_linkedNodes);
	expectEqualLinkedNodes(n->GetLinkedNodes(), m_linkedNodes);
}

TEST_F(NodeLinkNodeTest, when_setting_linked_nodes_should_store_them)
{
	auto n = std::make_shared<NodeLinkNode>(0, 1, NodeLinkNode::NodeLinkContainer_t{});
	n->SetLinkedNodes(m_linkedNodes);
	expectEqualLinkedNodes(n->GetLinkedNodes(), m_linkedNodes);
}

TEST_F(NodeLinkNodeTest, when_adding_a_linked_node_should_store_it)
{
	auto n = std::make_shared<NodeLinkNode>(0, 1, NodeLinkNode::NodeLinkContainer_t{});
	for (auto l : m_linkedNodes) {
		n->AddLinkedNode(l->GetNodeName());
	}
	expectEqualLinkedNodes(n->GetLinkedNodes(), m_linkedNodes);
}

TEST_F(NodeLinkNodeTest, when_iterating_over_should_access_the_linked_nodes_in_order)
{
	auto n = std::make_shared<NodeLinkNode>(0, 1, m_linkedNodes);
	NodeLinkNode::NodeLinkContainer_t c(n->begin(), n->end());  // does the same as iterating
	expectEqualLinkedNodes(c, m_linkedNodes);
}

