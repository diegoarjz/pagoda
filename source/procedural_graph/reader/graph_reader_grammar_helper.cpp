#include "graph_reader_grammar_helper.h"

#include "graph_definition_node.h"
#include "graph_statement_node.h"
#include "named_argument.h"
#include "node_definition_node.h"
#include "node_link_node.h"

namespace selector
{
namespace grammar_helpers
{
NamedArgumentPtr CreateStringNamedArgument(std::string &name, std::string &str)
{
	return std::make_shared<NamedArgument>(name, NamedArgument::ArgumentType::String, str);
}

NamedArgumentPtr CreateFloatNamedArgument(std::string &name, float &f)
{
	return std::make_shared<NamedArgument>(name, NamedArgument::ArgumentType::Float, std::to_string(f));
}

NamedArgumentPtr CreateIntegerNamedArgument(std::string &name, int &f)
{
	return std::make_shared<NamedArgument>(name, NamedArgument::ArgumentType::Integer, std::to_string(f));
}

NamedArgumentPtr CreateExpressionNamedArgument(std::string &name, std::string &expression)
{
	return std::make_shared<NamedArgument>(name, NamedArgument::ArgumentType::Expression, expression);
}

NodeDefinitionNodePtr CreateNodeDefinition(std::string &name, std::string &type,
                                           std::vector<NamedArgumentPtr> &constructionArgs)
{
	return std::make_shared<NodeDefinitionNode>(name, type, constructionArgs);
}

NodeDefinitionNodePtr SetExecutionArguments(NodeDefinitionNodePtr nodeDefinition,
                                            std::vector<NamedArgumentPtr> &executionArgs)
{
	nodeDefinition->SetExecutionArguments(executionArgs);
	return nodeDefinition;
}

NodeLinkNodePtr CreateNodeLink() { return std::make_shared<NodeLinkNode>(); }

void AddLinkedNode(NodeLinkNodePtr linkedNodes, std::string &nodeName) { linkedNodes->AddLinkedNode(nodeName); }

GraphDefinitionNodePtr CreateGraphDefinition() { return std::make_shared<GraphDefinitionNode>(); }

void AddNodeDefinition(GraphDefinitionNodePtr graph, NodeDefinitionNodePtr statement)
{
	graph->AddGraphStatementNode(statement);
}

void AddNodeLinks(GraphDefinitionNodePtr graph, NodeLinkNodePtr statement) { graph->AddGraphStatementNode(statement); }
}  // namespace grammar_helpers
}  // namespace selector
