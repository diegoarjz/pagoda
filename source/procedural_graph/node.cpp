#include "node.h"

#include "node_execution.h"

#include <iostream>

namespace selector
{
class Node::Impl
{
public:
	Impl(NodeType type) : m_nodeName(""), m_nodeId(0), m_result(nullptr), m_nodeType(type) {}

	uint32_t GetId() const { return m_nodeId; }
	void SetId(uint32_t nodeId) { m_nodeId = nodeId; }

	void SetName(const std::string &name) { m_nodeName = name; }
	const std::string &GetName() const { return m_nodeName; }
	void SetNodeExecution(std::shared_ptr<NodeExecution> execution) { m_execution = execution; }
	std::shared_ptr<NodeExecution> GetNodeExecution() const { return m_execution; }
	void SetParameterContext(std::shared_ptr<Context> context) { m_context = context; }
	std::shared_ptr<Context> GetParameterContext() const { return m_context; }
	NodeExecutionResultPtr GetResult() const { return m_result; }
	NodeType GetNodeType() const { return m_nodeType; }

	void Execute(NodePtr node, GraphExecutionContextPtr executionContext)
	{
		if (m_execution && executionContext)
		{
			m_result = m_execution->Execute(node, executionContext);
		}
	}

private:
	std::string m_nodeName;
	uint32_t m_nodeId;
	std::shared_ptr<NodeExecution> m_execution;
	std::shared_ptr<Context> m_context;
	NodeExecutionResultPtr m_result;
	NodeType m_nodeType;
};

Node::Node(NodeType type) : m_implementation(std::make_unique<Node::Impl>(type)) {}
Node::~Node() { m_implementation = nullptr; }

uint32_t Node::GetId() const { return m_implementation->GetId(); }
void Node::SetId(uint32_t nodeId) { m_implementation->SetId(nodeId); }

void Node::SetName(const std::string &name) { m_implementation->SetName(name); }
const std::string &Node::GetName() const { return m_implementation->GetName(); }
void Node::SetNodeExecution(std::shared_ptr<NodeExecution> execution) { m_implementation->SetNodeExecution(execution); }
std::shared_ptr<NodeExecution> Node::GetNodeExecution() const { return m_implementation->GetNodeExecution(); }

void Node::SetParameterContext(std::shared_ptr<Context> context) { m_implementation->SetParameterContext(context); }
std::shared_ptr<Context> Node::GetParameterContext() const { return m_implementation->GetParameterContext(); }

NodeExecutionResultPtr Node::GetResult() const { return m_implementation->GetResult(); }
NodeType Node::GetNodeType() const { return m_implementation->GetNodeType(); }

void Node::Execute(GraphExecutionContextPtr executionContext)
{
	m_implementation->Execute(shared_from_this(), executionContext);
}

}  // namespace selector
