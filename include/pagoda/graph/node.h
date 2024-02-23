#pragma once

#include "pagoda/graph/node_set.h"

#include <pagoda/common/factory.h>
#include <pagoda/dynamic/builtin_class.h>
#include <pagoda/objects/procedural_operation.h>
#include <pagoda/objects/parameter.h>
#include <pagoda/objects/interface.h>

#include <memory>
#include <map>
#include <set>

namespace pagoda::objects
{
class NewParameterCallback;
}  // namespace pagoda::objects

namespace pagoda::graph
{
using NodePtr = std::shared_ptr<class Node>;
using GraphPtr = std::shared_ptr<class Graph>;

/**
 * Represents a \c Node in a \c Graph.
 *
 * Each subclass of \c Node is responsible for implementing an execution logic
 * by overriding the Node::Execute() method.
 */
class Node : public dynamic::BuiltinClass
{
	public:
	static const dynamic::TypeInfoPtr s_typeInfo;

	Node();
	virtual ~Node();

	Node(const Node &n) = delete;
	Node &operator=(const Node &n) = delete;

	/**
	 * Gets the id from this \c Node.
	 */
	uint32_t GetId() const;
	/**
	 * Sets the id of this \c Node.
	 */
	void SetId(uint32_t nodeId);

	/**
	 * Sets this \c Node name.
	 */
	void SetName(const std::string &name);
	/**
	 * Gets this \c Node name.
	 */
	const std::string &GetName() const;

  /**
   * Called by \c Graph when creating the \c Node.
   * Subclasses can override to perform more tasks,
   * including creating new nodes.
   */
  virtual void AttachToGraph(Graph* graph);

  /**
   * Get the \c this \c Node is attached to.
   */
  Graph* GetGraph() const { return m_graph; }

  /**
   * Sets the \c Node position in the graph.
   * This is stored in the `posX` and a `posY`
   * parameters.
   */
  void SetPos(const math::Vec2F pos);
  /**
   * Returns the \c Node position in the graph.
   */
  math::Vec2F GetPos() const;


	/**
	 * Executes some operation in the procedural graph.
	 *
	 * Subclasses should override this method to implement some execution logic.
	 *
	 * @param inNodes           The input nodes to the current \c Node, if any.
	 * @param outNodes          The output nodes to the current \c Node, if any.
	 */
	virtual void Execute(const NodeSet &inNodes, const NodeSet &outNodes) = 0;

	std::string ToString() const override;

	virtual const char *const GetNodeType() = 0;

	void AcceptVisitor(dynamic::ValueVisitorBase &visitor) override;

	objects::ParameterBasePtr GetParameter(const std::string &name) const;
	void ForEachParameter(std::function<void(objects::ParameterBasePtr)> f) const;

  /**
   * Calls \a cb on each input and output interface.
   */
  virtual void Interfaces(objects::InterfaceCallback* cb) {}
  /**
   * Calls \b cb on each input interface.
   */
  virtual void InputInterfaces(objects::InterfaceCallback* cb) {}
  /**
   * Calls \b cb on each output interface.
   */
  virtual void OutputInterfaces(objects::InterfaceCallback* cb) {}

	protected:
	std::map<std::string, objects::ParameterBasePtr> m_parameters;

	private:
  Graph* m_graph{nullptr};
	std::string m_nodeName;
	uint32_t m_nodeId;
  float m_posX;
  float m_posY;
};  // class Node

using NodePtr = std::shared_ptr<Node>;
using NodeWeakPtr = std::weak_ptr<Node>;

struct NodeWeakPtrHasher
{
	size_t operator()(const NodeWeakPtr &ptr) const
	{
		auto sh_ptr = ptr.lock();
		return std::hash<NodePtr>()(sh_ptr);
	}
};

struct NodeWeakPtrEqual
{
	bool operator()(const NodeWeakPtr &lhs, const NodeWeakPtr &rhs) const
	{
		return lhs.lock() == rhs.lock();
	}
};

}  // namespace pagoda::graph

