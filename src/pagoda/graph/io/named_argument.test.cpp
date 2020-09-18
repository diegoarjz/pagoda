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

/* *****************
 * Named Argument  *
 * ****************/
class NamedArgumentTest : public ::testing::Test
{
	protected:
	void SetUp() {}

	void TearDown() {}
};

TEST_F(NamedArgumentTest, when_constructing_with_name_type_and_value_should_store_them)
{
	auto n = std::make_shared<NamedArgument>("parameter", NamedArgument::ArgumentType::String, "value");
	EXPECT_EQ(n->GetName(), "parameter");
	EXPECT_EQ(n->GetArgumentType(), NamedArgument::ArgumentType::String);
	EXPECT_EQ(n->GetArgumentValue(), "value");
}

TEST_F(NamedArgumentTest, when_setting_name_type_and_value_should_store_them)
{
	auto n = std::make_shared<NamedArgument>("parameter", NamedArgument::ArgumentType::String, "value");
	n->SetName("itsName");
	n->SetArgumentType(NamedArgument::ArgumentType::Float);
	n->SetArgumentValue("itsValue");
	EXPECT_EQ(n->GetName(), "itsName");
	EXPECT_EQ(n->GetArgumentType(), NamedArgument::ArgumentType::Float);
	EXPECT_EQ(n->GetArgumentValue(), "itsValue");
}

