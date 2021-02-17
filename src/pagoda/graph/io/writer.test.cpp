#include <pagoda/graph/graph.h>
#include <pagoda/graph/io/writer.h>

#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>

#include <pagoda/dynamic/float_value.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::dynamic;
using namespace pagoda::graph;
using namespace pagoda::graph::io;

class GraphWriterTest : public ::testing::Test
{
	protected:
	void SetUp() override
	{
		m_graph = m_pagoda.CreateGraph();
		m_writer = std::make_shared<GraphWriter>(m_graph);
	}

	void TearDown() override {}

	std::string getAsString()
	{
		std::stringstream out;
		m_writer->Write(out);
		return out.str();
	}

	Pagoda m_pagoda;
	std::shared_ptr<Graph> m_graph;
	std::shared_ptr<GraphWriter> m_writer;
};

TEST_F(GraphWriterTest, test_empty_graph) { EXPECT_EQ(getAsString(), ""); }

TEST_F(GraphWriterTest, test_single_operation)
{
	m_graph->CreateNode<OperationNode>("op");
	auto opNode = std::dynamic_pointer_cast<OperationNode>(m_graph->GetNode("op"));
	opNode->SetOperation(m_pagoda.GetOperationFactory()->Create("CreateRectGeometry"));
	opNode->SetExecutionArguments({{"parameter", std::make_shared<FloatValue>(123.0f)}});

	EXPECT_EQ(getAsString(), R"(op = CreateRectGeometry{
  parameter : 123.000000
}
)");
}

TEST_F(GraphWriterTest, test_links)
{
	m_graph->CreateNode<OperationNode>("op");
	m_graph->CreateNode<InputInterfaceNode>("in");
	m_graph->CreateNode<OutputInterfaceNode>("out");
	m_graph->CreateEdge("in", "op");
	m_graph->CreateEdge("op", "out");
	auto opNode = std::dynamic_pointer_cast<OperationNode>(m_graph->GetNode("op"));
	opNode->SetOperation(m_pagoda.GetOperationFactory()->Create("CreateRectGeometry"));
	std::dynamic_pointer_cast<InputInterfaceNode>(m_graph->GetNode("in"))->SetInterfaceName("in");
	std::dynamic_pointer_cast<OutputInterfaceNode>(m_graph->GetNode("out"))->SetInterfaceName("out");

	EXPECT_EQ(getAsString(), R"(in = InputInterface(interface: "in"){
}
op = CreateRectGeometry{
}
out = OutputInterface(interface: "out"){
}
in -> op;
op -> out;
)");
}
