#ifndef SELECTOR_PROCEDURAL_GRAPH_GRAPH_STATEMENT_NODE_H_
#define SELECTOR_PROCEDURAL_GRAPH_GRAPH_STATEMENT_NODE_H_

#include "AstNode.h"

namespace selector
{
/**
 * Represents a statement in the graph format.
 * Statements can be, for example, a node definition or node links.
 */
class GraphStatementNode : public AstNode
{
public:
private:
};

using GraphStatementNodePtr = std::shared_ptr<GraphStatementNode>;
}

#endif