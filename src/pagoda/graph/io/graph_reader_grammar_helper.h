#ifndef PAGODA_PROCEDURAL_GRAPH_GRAPH_READER_GRAMMAR_HELPER_H_
#define PAGODA_PROCEDURAL_GRAPH_GRAPH_READER_GRAMMAR_HELPER_H_

#include <memory>
#include <vector>

namespace pagoda::graph::io
{
class NamedArgument;
using NamedArgumentPtr = std::shared_ptr<NamedArgument>;
class NodeDefinitionNode;
using NodeDefinitionNodePtr = std::shared_ptr<NodeDefinitionNode>;
class NodeLinkNode;
using NodeLinkNodePtr = std::shared_ptr<NodeLinkNode>;
class GraphDefinitionNode;
using GraphDefinitionNodePtr = std::shared_ptr<GraphDefinitionNode>;

namespace grammar_helpers
{
NamedArgumentPtr CreateStringNamedArgument(std::string &name, std::string &str);
NamedArgumentPtr CreateFloatNamedArgument(std::string &name, float &f);
NamedArgumentPtr CreateIntegerNamedArgument(std::string &name, int &f);
NamedArgumentPtr CreateExpressionNamedArgument(std::string &name, std::string &expression);

NodeDefinitionNodePtr CreateNodeDefinition(std::string &name, std::string &type,
                                           std::vector<NamedArgumentPtr> &constructionArgs);

NodeDefinitionNodePtr SetExecutionArguments(NodeDefinitionNodePtr nodeDefinition,
                                            std::vector<NamedArgumentPtr> &executionArgs);

NodeLinkNodePtr CreateNodeLink();

void AddLinkedNode(NodeLinkNodePtr linkedNodes, std::string &nodeName);

GraphDefinitionNodePtr CreateGraphDefinition();

void AddNodeDefinition(GraphDefinitionNodePtr graph, NodeDefinitionNodePtr statement);
void AddNodeLinks(GraphDefinitionNodePtr graph, NodeLinkNodePtr statement);
}  // namespace grammar_helpers
}  // namespace pagoda::graph::io

#endif
