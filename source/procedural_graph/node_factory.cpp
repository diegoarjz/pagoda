#include "node_factory.h"

#include "input_interface_execution.h"
#include "operation_execution.h"
#include "output_interface_execution.h"

#include "parameter/context.h"

namespace selector
{
class NodeFactory::Impl
{
public:
	NodePtr Create(NodeType nodeType, const std::string &nodeName = "") const
	{
		auto node = std::make_shared<Node>(nodeType);

		switch (nodeType)
		{
			case NodeType::InputInterface:
				node->SetNodeExecution(std::make_shared<InputInterfaceExecution>());
				break;
			case NodeType::OutputInterface:
				node->SetNodeExecution(std::make_shared<OutputInterfaceExecution>());
				break;
			case NodeType::Operation:
				node->SetNodeExecution(std::make_shared<OperationExecution>());
				break;
			default:
				return nullptr;
		};

		return node;
	}
};

NodeFactory::NodeFactory() : m_implementation(std::make_unique<Impl>()) {}
NodeFactory::~NodeFactory() {}

NodePtr NodeFactory::Create(NodeType nodeType, const std::string &nodeName) const
{
	return m_implementation->Create(nodeType, nodeName);
}
}  // namespace selector
