#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/node_set_visitor.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;

class NodeTypeFilterTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		m_inputInterfaceNode = std::make_shared<InputInterfaceNode>();
		m_outputInterfaceNode = std::make_shared<OutputInterfaceNode>();
		m_operationNode = std::make_shared<OperationNode>(m_pagoda.GetOperationFactory());

		m_nodeSet.insert(m_inputInterfaceNode);
		m_nodeSet.insert(m_outputInterfaceNode);
		m_nodeSet.insert(m_operationNode);
	}

	std::shared_ptr<InputInterfaceNode> m_inputInterfaceNode;
	std::shared_ptr<OutputInterfaceNode> m_outputInterfaceNode;
	std::shared_ptr<OperationNode> m_operationNode;
	NodeSet<Node> m_nodeSet;
	Pagoda m_pagoda;
};

TEST_F(NodeTypeFilterTest, when_visiting_a_node_set_should_call_visitor_methods_for_different_node_types)
{
	NodeSet<InputInterfaceNode> nodes;

	node_type_filter<InputInterfaceNode>(m_nodeSet, nodes);

	EXPECT_EQ(nodes.size(), 1);
	EXPECT_EQ(*nodes.begin(), m_inputInterfaceNode);
}
