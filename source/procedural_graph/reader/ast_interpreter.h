#ifndef SELECTOR_PROCEDURAL_GRAPH_AST_INTERPRETER_H_
#define SELECTOR_PROCEDURAL_GRAPH_AST_INTERPRETER_H_

#include <string>
#include <unordered_map>

#include "ast_node_visitor.h"
#include "parameter/parameter.h"

namespace selector
{
class Graph;
using GraphPtr = std::shared_ptr<Graph>;
class Node;
using NodePtr = std::shared_ptr<Node>;

/**
 * Class to interpret and create a \c Graph from an \c AstNode tree.
 */
class AstInterpreter : public AstNodeVisitor
{
public:
	AstInterpreter(GraphPtr graph);

	void Visit(GraphDefinitionNode *graphDefinition) override;
	void Visit(NamedArgument *namedArgument) override;
	void Visit(NodeDefinitionNode *nodeDefinition) override;
	void Visit(NodeLinkNode *nodeLink) override;

	const std::unordered_map<std::string, NodePtr> &GetNodeTable() const;
	const std::unordered_map<std::string, Parameter> &GetCurrentNamedArguments() const;

private:
	GraphPtr m_graph;

	std::unordered_map<std::string, NodePtr> m_nodeTable;
	std::unordered_map<std::string, Parameter> m_currentNamedParameters;
	uint32_t m_nextNodeId;
};
}  // namespace selector
#endif
