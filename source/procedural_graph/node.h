#ifndef SELECTOR_PROCEDURAL_GRAPH_NODE_H_
#define SELECTOR_PROCEDURAL_GRAPH_NODE_H_

#include <parameter/context.h>
#include <procedural_objects/procedural_operation.h>

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

class Node : public std::enable_shared_from_this<Node>
{
public:
	Node();
	virtual ~Node();

	uint32_t GetId() const;
	void SetId(uint32_t nodeId);

	void SetName(const std::string &name);
	const std::string &GetName() const;
	void SetParameterContext(std::shared_ptr<Context> context);
	std::shared_ptr<Context> GetParameterContext() const;

	virtual void Execute(GraphExecutionContextPtr executionContext, const NodeSet<Node> &inNodes,
	                     const NodeSet<Node> &outNodes) = 0;

	template<class VisitorT>
	void Visit(VisitorT *v)
	{
		visit(v, this);
	}

private:
	template<class VisitorT, class NodeT>
	void visit(VisitorT *visitor, NodeT *node)
	{
		visitor->Visit(node);
	}

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
