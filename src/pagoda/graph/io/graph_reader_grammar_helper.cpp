#include "graph_reader_grammar_helper.h"

#include "graph_definition_node.h"
#include "graph_statement_node.h"
#include "named_argument.h"
#include "node_definition_node.h"
#include "node_link_definition.h"
#include "node_link_node.h"

#include <iostream>
#include <memory>

namespace pagoda::graph::io
{
namespace grammar_helpers
{
NamedArgumentPtr CreateStringNamedArgument(std::string &name, std::string &str)
{
	return std::make_shared<NamedArgument>(
	  name, NamedArgument::ArgumentType::String, "\"" + str + "\"");
}

NamedArgumentPtr CreateFloatNamedArgument(std::string &name, float &f)
{
	return std::make_shared<NamedArgument>(
	  name, NamedArgument::ArgumentType::Float, std::to_string(f));
}

NamedArgumentPtr CreateIntegerNamedArgument(std::string &name, int &f)
{
	return std::make_shared<NamedArgument>(
	  name, NamedArgument::ArgumentType::Integer, std::to_string(f));
}

NamedArgumentPtr CreateBoolNamedArgument(std::string &name, bool &f)
{
	return std::make_shared<NamedArgument>(
	  name, NamedArgument::ArgumentType::Boolean, f ? "true" : "false");
}

NamedArgumentPtr CreateExpressionNamedArgument(std::string &name,
                                               std::string &expression)
{
	return std::make_shared<NamedArgument>(
	  name, NamedArgument::ArgumentType::Expression, expression);
}

NamedArgumentPtr CreateCompoundNamedArgument(std::string &name,
                                             std::string &value)
{
	return std::make_shared<NamedArgument>(
	  name, NamedArgument::ArgumentType::Compound, value);
}

NodeDefinitionNodePtr CreateNodeDefinition(
  std::string &name, std::string &type,
  std::vector<NamedArgumentPtr> &executionArgs)
{
	return std::make_shared<NodeDefinitionNode>(name, type, executionArgs);
}

NodeDefinitionNodePtr CreateOperationDefinition(
  std::string &name, std::string &operation,
  std::vector<NamedArgumentPtr> &executionArgs)
{
	// We need to add the operation parameter first so the operation is created
	// before the rest of the arguments.
	executionArgs.insert(executionArgs.begin(),
	                     std::make_shared<NamedArgument>(
	                       "operation", NamedArgument::ArgumentType::String,
	                       "\"" + operation + "\""));
	auto node =
	  std::make_shared<NodeDefinitionNode>(name, "Operation", executionArgs);
	return node;
}

NodeDefinitionNodePtr SetExecutionArguments(
  NodeDefinitionNodePtr nodeDefinition,
  std::vector<NamedArgumentPtr> &executionArgs)
{
	nodeDefinition->SetExecutionArguments(executionArgs);
	return nodeDefinition;
}

NodeLinkDefinitionPtr CreateLinkDefinition()
{
	return std::make_shared<NodeLinkDefinition>();
}
void SetInputInterface(NodeLinkDefinitionPtr n, const std::string &name)
{
	n->SetInputInterface(name);
}
void SetNodeName(NodeLinkDefinitionPtr n, const std::string &name)
{
	n->SetNodeName(name);
}
void SetOutputInterface(NodeLinkDefinitionPtr n, const std::string &name)
{
	n->SetOutputInterface(name);
}

NodeLinkNodePtr CreateNodeLink()
{
	return std::make_shared<NodeLinkNode>();
}

void AddLinkedNode(NodeLinkNodePtr linkedNodes,
                   NodeLinkDefinitionPtr &linkDefinition)
{
	linkedNodes->AddLinkedNode(linkDefinition);
}

GraphDefinitionNodePtr CreateGraphDefinition()
{
	return std::make_shared<GraphDefinitionNode>();
}

void AddNodeDefinition(GraphDefinitionNodePtr graph,
                       NodeDefinitionNodePtr statement)
{
	graph->AddGraphStatementNode(statement);
}

void AddNodeLinks(GraphDefinitionNodePtr graph, NodeLinkNodePtr statement)
{
	graph->AddGraphStatementNode(statement);
}
}  // namespace grammar_helpers
}  // namespace pagoda::graph::io
