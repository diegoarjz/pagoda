#ifndef SELECTOR_PROCEDURAL_GRAPH_NODE_LINK_NODE_H_
#define SELECTOR_PROCEDURAL_GRAPH_NODE_LINK_NODE_H_

#include "graph_statement_node.h"

#include <list>

namespace selector
{
/**
 * Represents node links in the graph format.
 */
class NodeLinkNode : public GraphStatementNode
{
public:
	/// Storage type for linked nodes.
    using NodeLinkContainer_t = std::list<std::string>;
    /// Iterator over linked nodes.
    using NodeLinkContainerIterator_t = NodeLinkContainer_t::iterator;
    
    /**
     * Constructs an empty list of linked nodes.
     */
    NodeLinkNode();
 
 	/**
 	 * Constructs a \c NodeLinkNode using the \p linkedNodes.
 	 */
    NodeLinkNode(const Offset_t &startOffset, const Offset_t &endOffset, const NodeLinkContainer_t &linkedNodes);
 
 	/**
 	 * Constructs a \c NodeLinkNode using the \p linkedNodes.
 	 */
    NodeLinkNode(const Range_t &range, const NodeLinkContainer_t &linkedNodes);
    
    virtual ~NodeLinkNode();
    
    /**
     * Returns the linked nodes.
     */
    const NodeLinkContainer_t& GetLinkedNodes() const;
    
    /**
     * Returns the linked nodes.
     */
    NodeLinkContainer_t& GetLinkedNodes();
    
    /**
     * Sets the linked nodes.
     */
    void SetLinkedNodes(const NodeLinkContainer_t &linkedNodes);
    
    /**
     * Adds a linked node.
     */
    void AddLinkedNode(const std::string &nodeName);
    
    /**
     * Returns an iterator to the beginning of the linked nodes.
     */
    NodeLinkContainerIterator_t& begin() const;
    
    /**
     * Returns an iterator to the end of the linked nodes.
     */
    NodeLinkContainerIterator_t& end() const;
private:
    NodeLinkContainer_t m_linkedNodes; ///< List of linked nodes.
};
}

#endif