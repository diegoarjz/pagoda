#pragma once

#include "pagoda/graph/node.h"

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

class InputInterfaceNode : public Node
{
	public:
	static const char* name;

	InputInterfaceNode();
	~InputInterfaceNode() override;

	void Execute(const NodeSet& inNodes, const NodeSet& outNodes) override;
	void SetInterfaceName(const std::string& interfaceName);
	const std::string& GetInterfaceName() const;

	const char* const GetNodeType() override;

  /**
   * Stores the InterfaceableNode when connected to it
   * and connects to the interface in an OutputInterfaceNode
   * when connected to it.
   */
  void ConnectedToNode(const NodePtr& node) override;

  /**
   * Clears the InterfaceableNode when disconnected from it
   * and disconnects from the interface in an OutputInterfaceNode
   * when disconnected to it.
   */
  void DisconnectedFromNode(const NodePtr& node) override;

  /**
   * Returns the \c InterfaceableNode this \c InputInterfaceNode is
   * connected to, if any.
   */
  InterfaceableNodePtr GetInterfaceableNode() const;
  /**
   * Returns the actual \c Interface this \c InputInterfaceNode
   * represents in the \c ProceduralOperation if it is connected
   * to an \c InterfaceableNode.
   */
  objects::InterfacePtr GetInterface() const;

	private:
	std::string m_interfaceName;
  InterfaceableNodeWeakPtr m_interfaceableNode;
};  // class OperationExecution

using InputInterfaceNodePtr = std::shared_ptr<InputInterfaceNode>;
}  // namespace pagoda::graph
