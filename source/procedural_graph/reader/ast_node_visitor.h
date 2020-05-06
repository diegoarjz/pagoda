#ifndef PAGODA_PROCEDURAL_GRAPH_AST_NODE_VISITOR_H_
#define PAGODA_PROCEDURAL_GRAPH_AST_NODE_VISITOR_H_

namespace pagoda
{
class GraphDefinitionNode;
class NamedArgument;
class NodeDefinitionNode;
class NodeLinkNode;
/**
 * Implements logic to iterate over an \c AstNode tree.
 */
class AstNodeVisitor
{
public:
	virtual void Visit(GraphDefinitionNode *graphDefinition) = 0;
	virtual void Visit(NamedArgument *namedArgument) = 0;
	virtual void Visit(NodeDefinitionNode *nodeDefinition) = 0;
	virtual void Visit(NodeLinkNode *nodeLink) = 0;
};
}  // namespace pagoda

#endif
