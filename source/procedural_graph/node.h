#ifndef SELECTOR_PROCEDURAL_GRAPH_NODE_H_
#define SELECTOR_PROCEDURAL_GRAPH_NODE_H_

#include "node_type.h"

#include <parameter/context.h>
#include <procedural_objects/procedural_operation.h>

#include <memory>

namespace selector
{
inline constexpr size_t Index(const NodeType &type) { return static_cast<size_t>(type); }

class NodeExecution;
class NodeExecutionResult;
using NodeExecutionResultPtr = std::shared_ptr<NodeExecutionResult>;
struct GraphExecutionContext;
using GraphExecutionContextPtr = std::shared_ptr<GraphExecutionContext>;

class Node : public std::enable_shared_from_this<Node>
{
public:
	Node(NodeType type);
	virtual ~Node();

	uint32_t GetId() const;
	void SetId(uint32_t nodeId);

	void SetName(const std::string &name);
	const std::string &GetName() const;
	void SetNodeExecution(std::shared_ptr<NodeExecution> execution);
	std::shared_ptr<NodeExecution> GetNodeExecution() const;
	void SetParameterContext(std::shared_ptr<Context> context);
	std::shared_ptr<Context> GetParameterContext() const;
	NodeExecutionResultPtr GetResult() const;

	void Execute(GraphExecutionContextPtr executionContext);

	NodeType GetNodeType() const;

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
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
