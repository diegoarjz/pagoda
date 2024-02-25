#pragma once
#ifndef PAGODA_PROCEDURAL_GRAPH_OUTPUT_INTERFACE_EXECUTION_H_
#define PAGODA_PROCEDURAL_GRAPH_OUTPUT_INTERFACE_EXECUTION_H_

#include "pagoda/graph/node.h"

#include <list>

namespace pagoda::objects
{
class ProceduralOperation;
using ProceduralOperationPtr = std::shared_ptr<ProceduralOperation>;
class ProceduralObject;
using ProceduralObjectPtr = std::shared_ptr<ProceduralObject>;
}  // namespace pagoda::objects

namespace pagoda::graph
{
using InterfaceableNodePtr = std::shared_ptr<class InterfaceableNode>;
using InterfaceableNodeWeakPtr = std::weak_ptr<class InterfaceableNode>;

class OutputInterfaceNode : public Node
{
	public:
	static const char* name;

	OutputInterfaceNode();
	~OutputInterfaceNode();

	void Execute(const NodeSet& inNodes, const NodeSet& outNodes) override;

	void SetInterfaceName(const std::string& name);
	const std::string& GetInterfaceName() const;

	const char* const GetNodeType() override;

  /**
   * Stores the OperationNode when connected to it
   * and connects to the interface in an OutputInterfaceNode
   * when connected to it.
   */
  void ConnectedToNode(const NodePtr& node) override;

  /**
   * Clears the OperationNode when disconnected from it
   * and disconnects from the interface in an OutputInterfaceNode
   * when disconnected to it.
   */
  void DisconnectedFromNode(const NodePtr& node) override;

  /**
   * Returns the \c OperationNode this \c OutputInterfaceNode is
   * connected to, if any.
   */
  InterfaceableNodePtr GetInterfaceableNode() const;
  /**
   * Returns the actual \c Interface this \c OutputInterfaceNode
   * represents in the \c ProceduralOperation if it is connected
   * to an \c OperationNode.
   */
  objects::InterfacePtr GetInterface() const;

	private:
	std::string m_interfaceName;
  InterfaceableNodeWeakPtr m_interfaceableNode;
};  // class OutputInterfaceNode

using OutputInterfaceNodePtr = std::shared_ptr<OutputInterfaceNode>;
}  // namespace pagoda::graph

#endif
