#include <procedural_graph/input_interface_node.h>
#include <procedural_graph/node_set_visitor.h>
#include <procedural_graph/operation_node.h>
#include <procedural_graph/output_interface_node.h>

#include <gtest/gtest.h>

using namespace selector;

class NodeTypeFilterTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		m_inputInterfaceNode = std::make_shared<InputInterfaceNode>();
		m_outputInterfaceNode = std::make_shared<OutputInterfaceNode>();
		m_operationNode = std::make_shared<OperationNode>();

		m_nodeSet.insert(m_inputInterfaceNode);
		m_nodeSet.insert(m_outputInterfaceNode);
		m_nodeSet.insert(m_operationNode);
	}

	std::shared_ptr<InputInterfaceNode> m_inputInterfaceNode;
	std::shared_ptr<OutputInterfaceNode> m_outputInterfaceNode;
	std::shared_ptr<OperationNode> m_operationNode;
	NodeSet<Node> m_nodeSet;
};

TEST_F(NodeTypeFilterTest, when_visiting_a_node_set_should_call_visitor_methods_for_different_node_types)
{
	NodeSet<InputInterfaceNode> nodes;

	node_type_filter<InputInterfaceNode>(m_nodeSet, nodes);

	EXPECT_EQ(nodes.size(), 1);
	EXPECT_EQ(*nodes.begin(), m_inputInterfaceNode);
}
