#include <pagoda/graph/operation_node.h>
#include <pagoda/objects/operation_factory.h>

#include "mock_objects.h"

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::objects;
using namespace pagoda::graph;

class NodeTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_operationFactory = std::make_shared<OperationFactory>();
		node = std::make_shared<OperationNode>(m_operationFactory);
	}

	void TearDown() {}

	std::shared_ptr<Node> node;
	OperationFactoryPtr m_operationFactory;
};

TEST_F(NodeTest, when_setting_node_id_should_be_able_to_get_node_id)
{
	this->node->SetId(123);
	EXPECT_EQ(this->node->GetId(), 123);
}

TEST_F(NodeTest, when_getting_node_id_after_construction_should_return_0) { EXPECT_EQ(this->node->GetId(), 0); }

TEST_F(NodeTest, when_setting_node_name_should_be_able_to_get_node_name)
{
	this->node->SetName("nodeName");
	EXPECT_EQ(this->node->GetName(), "nodeName");
}

TEST_F(NodeTest, when_getting_node_name_after_construction_should_return_empty_name)
{
	EXPECT_EQ(this->node->GetName(), "");
}
