#ifndef PAGODA_PROCEDURAL_GRAPH_NODE_H_
#define PAGODA_PROCEDURAL_GRAPH_NODE_H_

#include "node_set.h"

#include <pagoda/common/factory.h>
#include <pagoda/dynamic_value/builtin_class.h>
#include <pagoda/objects/procedural_operation.h>

#include <memory>
#include <set>

namespace pagoda
{
class Node;
using NodePtr = std::shared_ptr<Node>;

class NodeVisitor;

/**
 * Represents a \c Node in a \c Graph.
 *
 * Each subclass of \c Node is responsible for implementing an execution logic by
 * overriding the Node::Execute() method.
 */
class Node : public BuiltinClass
{
public:
	static const TypeInfoPtr s_typeInfo;

	Node();
	virtual ~Node();

	Node(const Node &n) = delete;
	Node &operator=(const Node &n) = delete;

	/**
	 * Sets the construction arguments from \p arguments.
	 */
	virtual void SetConstructionArguments(const std::unordered_map<std::string, DynamicValueBasePtr> &arguments) = 0;

	/**
	 * Sets the execution arguments from \p arguments.
	 */
	void SetExecutionArguments(const std::unordered_map<std::string, DynamicValueBasePtr> &arguments);

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
	 * Sets all the expression variables.
	 *
	 * Values are fetched from the \c Node's parameter \c Context.
	 */
	virtual void SetExpressionVariables();

	/**
	 * Accepts a \c NodeVisitor.
	 */
	virtual void AcceptNodeVisitor(NodeVisitor *visitor) = 0;

	/**
	 * Executes some operation in the procedural graph.
	 *
	 * Subclasses should override this method to implement some execution logic.
	 *
	 * @param inNodes           The input nodes to the current \c Node, if any.
	 * @param outNodes          The output nodes to the current \c Node, if any.
	 */
	virtual void Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes) = 0;

	std::string ToString() const override;

	void AcceptVisitor(ValueVisitorBase &visitor) override;

private:
	std::string m_nodeName;
	uint32_t m_nodeId;
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
	bool operator()(const NodeWeakPtr &lhs, const NodeWeakPtr &rhs) const { return lhs.lock() == rhs.lock(); }
};

}  // namespace pagoda

#endif
