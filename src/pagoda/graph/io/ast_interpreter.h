#ifndef PAGODA_PROCEDURAL_GRAPH_AST_INTERPRETER_H_
#define PAGODA_PROCEDURAL_GRAPH_AST_INTERPRETER_H_

#include <string>
#include <unordered_map>

#include "ast_node_visitor.h"
#include "pagoda/graph/node_factory.h"

namespace pagoda::graph
{
class Graph;
using GraphPtr = std::shared_ptr<Graph>;
class Node;
using NodePtr = std::shared_ptr<Node>;
}  // namespace pagoda::graph

namespace pagoda::graph::io
{
/**
 * Class to interpret and create a \c Graph from an \c AstNode tree.
 */
class AstInterpreter : public AstNodeVisitor
{
	public:
	AstInterpreter(GraphPtr graph);
	virtual ~AstInterpreter();

	void Visit(GraphDefinitionNode *graphDefinition) override;
	void Visit(NamedArgument *namedArgument) override;
	void Visit(NodeDefinitionNode *nodeDefinition) override;
	void Visit(NodeLinkNode *nodeLink) override;
	void Visit(NodeLinkDefinition *nodeLinkDefinition) override;

	const std::unordered_map<std::string, dynamic::DynamicValueBasePtr> &GetCurrentNamedArguments() const;

	private:
	GraphPtr m_graph;

	std::unordered_map<std::string, dynamic::DynamicValueBasePtr> m_currentNamedParameters;
};
}  // namespace pagoda::graph::io
#endif
