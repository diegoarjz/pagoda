#ifndef SELECTOR_PROCEDURAL_GRAPH_NODE_H_
#define SELECTOR_PROCEDURAL_GRAPH_NODE_H_

#include "common/factory.h"
#include "parameter/context.h"
#include "procedural_objects/procedural_operation.h"

#include <memory>
#include <unordered_set>

namespace selector
{
struct GraphExecutionContext;
using GraphExecutionContextPtr = std::shared_ptr<GraphExecutionContext>;

class Node;
using NodePtr = std::shared_ptr<Node>;

template<class Node>
using NodeSet = std::unordered_set<std::shared_ptr<Node>>;

/**
 * Represents a \c Node in a \c Graph.
 *
 * Each subclass of \c Node is responsible for implementing an execution logic by
 * overriding the Node::Execute() method.
 */
class Node : public Factory<Node>
{
public:
	Node();
	virtual ~Node();

	Node(const Node &n) = delete;
	Node &operator=(const Node &n) = delete;

	/**
	 * Sets the construction arguments from \p arguments.
	 */
	virtual void SetConstructionArguments(const std::unordered_map<std::string, Parameter> &arguments) = 0;

	/**
	 * Sets the execution arguments from \p arguments.
	 */
	void SetExecutionArguments(const std::unordered_map<std::string, Parameter> &arguments);

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
	 * Sets this \c Node parameter \c Context.
	 */
	void SetParameterContext(std::shared_ptr<Context> context);

	/**
	 * Gets this \c Node parameter \c Context.
	 */
	std::shared_ptr<Context> GetParameterContext() const;

	/**
	 * Sets all the expression variables.
	 *
	 * Values are fetched from the \c Node's parameter \c Context.
	 */
	virtual void SetExpressionVariables();

	/**
	 * Executes some operation in the procedural graph.
	 *
	 * Subclasses should override this method to implement some execution logic.
	 *
	 * @param inNodes           The input nodes to the current \c Node, if any.
	 * @param outNodes          The output nodes to the current \c Node, if any.
	 */
	virtual void Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes) = 0;

private:
	std::string m_nodeName;
	uint32_t m_nodeId;
	std::shared_ptr<Context> m_context;
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

}  // namespace selector

#endif
