#pragma once
#ifndef SELECTOR_PROCEDURAL_GRAPH_NODE_FACTORY_H_
#define SELECTOR_PROCEDURAL_GRAPH_NODE_FACTORY_H_

#include "node.h"
#include "node_type.h"

namespace selector
{
/** Provides a way of instantiating objects of \c Node.
 *
 * Built-in node types (defined in \c NodeType enumeration) can be directly instantiated by passing the corresponding
 * enumeration value to \c NodeFactory::Create.
 *
 * To instantiate custom node types (as in sub-classes of \c Node), users should first subclass \c NodeFactory to handle
 * the creation logic of the custom node type in \c NodeFactory::Create based on its name. Instantiation is then
 * achieved by passing \c NodeType::Custom to \c NodeFactory::Create and the respective node name.
 */
class NodeFactory
{
public:
	NodeFactory();
	virtual ~NodeFactory();

	/**
	 * @brief Creates and returns a smart pointer to a \c Node (\c NodePtr) with the defined in \p nodeType.
	 *
	 * If \p nodeType is \c NodeType::Custom, then \p nodeName is the name of the custom node to create and subclasses
	 * must handle its instantiation.
	 *
	 * In other words, consider that the client code wants to instantiate a node of type CustomNode which derives from
	 * \c Node. In this case, \p nodeType should be set to \c NodeType::Custom and \p nodeName should be set to
	 * CustomNode. Client code must also provide a subclass of \c NodeFactory that knows how to instantiate the
	 * CustomNode from the provided name.
	 *
	 * @param [in] nodeType      The type of node to create.
	 * @param [in] nodeTypeName  The custom node type name. Valid if \p nodeType is \c NodeType::Custom.
	 * @return                   A node with the given type or nullptr if unable to create the node.
	 */
	virtual NodePtr Create(NodeType nodeType, const std::string &nodeName = "") const;

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace selector
#endif
