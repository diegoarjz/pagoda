#include <pagoda/graph/graph.h>
#include <pagoda/graph/io/reader.h>
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
		m_reader = std::make_shared<GraphReader>(m_pagoda.GetNodeFactory());
	}

	void TearDown() override
	{
	}

	std::string getAsString()
	{
		std::stringstream out;
		m_writer->Write(out);
		return out.str();
	}

	Pagoda m_pagoda;
	std::shared_ptr<Graph> m_graph;
	std::shared_ptr<GraphWriter> m_writer;
	std::shared_ptr<GraphReader> m_reader;
};

TEST_F(GraphWriterTest, test_empty_graph)
{
	EXPECT_EQ(getAsString(), "");
}

TEST_F(GraphWriterTest, test_single_operation)
{
	m_graph->CreateNode<OperationNode>("op");
	NodePtr opNode =
	  std::dynamic_pointer_cast<OperationNode>(m_graph->GetNode("op"));
	std::dynamic_pointer_cast<OperationNode>(opNode)->SetOperation(
	  m_pagoda.GetOperationFactory()->Create("CreateRectGeometry"));

	EXPECT_EQ(getAsString(), R"(op = CreateRectGeometry {
  plane: "z",
  height: 1.000000,
  width: 1.000000
}
)");

	EXPECT_NE(m_reader->Read(getAsString()), nullptr);
}

TEST_F(GraphWriterTest, test_links)
{
	m_graph->CreateNode<OperationNode>("op");
	m_graph->CreateNode<OperationNode>("op2");
	m_graph->CreateNode<InputInterfaceNode>("in");
	m_graph->CreateNode<OutputInterfaceNode>("out");
	m_graph->CreateEdge("op", "out");
	m_graph->CreateEdge("out", "in");
	m_graph->CreateEdge("in", "op2");
	auto opNode =
	  std::dynamic_pointer_cast<OperationNode>(m_graph->GetNode("op"));
	opNode->SetOperation(
	  m_pagoda.GetOperationFactory()->Create("CreateRectGeometry"));
	auto opNode2 =
	  std::dynamic_pointer_cast<OperationNode>(m_graph->GetNode("op2"));
	opNode2->SetOperation(
	  m_pagoda.GetOperationFactory()->Create("CreateRectGeometry"));

	std::dynamic_pointer_cast<InputInterfaceNode>(m_graph->GetNode("in"))
	  ->SetInterfaceName("in");
	std::dynamic_pointer_cast<OutputInterfaceNode>(m_graph->GetNode("out"))
	  ->SetInterfaceName("out");

	EXPECT_EQ(getAsString(), R"(op = CreateRectGeometry {
  plane: "z",
  height: 1.000000,
  width: 1.000000
}
op2 = CreateRectGeometry {
  plane: "z",
  height: 1.000000,
  width: 1.000000
}
op>out -> in<op2;
)");

	EXPECT_NE(m_reader->Read(getAsString()), nullptr);
}
