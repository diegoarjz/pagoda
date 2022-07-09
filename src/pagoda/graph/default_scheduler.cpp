#include "pagoda/graph/default_scheduler.h"

#include "pagoda/graph/input_interface_node.h"
#include "pagoda/graph/operation_node.h"
#include "pagoda/graph/output_interface_node.h"
#include "pagoda/graph/parameter_node.h"

#include "pagoda/graph/query/adjacent.h"
#include "pagoda/graph/query/and.h"
#include "pagoda/graph/query/graph_grammar.h"
#include "pagoda/graph/query/input_node.h"
#include "pagoda/graph/query/query.h"
#include "pagoda/graph/query/rule.h"
#include "pagoda/graph/query/topology.h"
#include "pagoda/graph/query/type.h"

#include "pagoda/graph/traversal/forward.h"

#include <pagoda/common/exception/exception.h>
#include <pagoda/common/instrument/profiler.h>

#include <ctime>
#include <memory>

using namespace pagoda::objects;

namespace pagoda::graph
{
using namespace query;

DefaultScheduler::DefaultScheduler(Graph &graph) : m_graph(graph)
{
}

/*
class OperationLinker : public Rule
{
	public:
	OperationLinker(Graph &graph) : Rule(graph), m_topology(&m_upstreamNode)
	{
		m_topology.AddDownstreamQuery(&m_upstreamNode, &m_outputInterface);
		m_topology.AddDownstreamQuery(&m_outputInterface, &m_inputInterface);
		m_topology.AddDownstreamQuery(&m_inputInterface, &m_downstreamNode);
	}

	QueryTopology &GetTopology() override
	{
		return m_topology;
	}

	void ApplyMatch(const std::map<Query *, NodePtr> &match) override
	{
		auto downstream =
		  std::dynamic_pointer_cast<OperationNode>(match.at(&m_downstreamNode));
		auto outInterface = std::dynamic_pointer_cast<OutputInterfaceNode>(
		  match.at(&m_outputInterface));
		auto inInterface = std::dynamic_pointer_cast<InputInterfaceNode>(
		  match.at(&m_inputInterface));
		auto upstream =
		  std::dynamic_pointer_cast<OperationNode>(match.at(&m_upstreamNode));

		auto upstreamOp = upstream->GetOperation();
		auto downstreamOp = downstream->GetOperation();

		downstreamOp->LinkInputInterface(inInterface->GetInterfaceName(),
		                                 outInterface->GetInterfaceName(),
		                                 upstreamOp);
	}

	private:
	Type<OperationNode> m_upstreamNode;
	Type<OutputInterfaceNode> m_outputInterface;
	Type<InputInterfaceNode> m_inputInterface;
	Type<OperationNode> m_downstreamNode;
	QueryTopology m_topology;
};
*/

void DefaultScheduler::Initialize()
{
	traversal::Forward f(m_graph);
	while (f.HasNext()) {
		m_nodeQueue.push(f.Get());
		f.Advance();
	}
	/*
	GraphGrammar graphGrammar(m_graph);

	auto linkRule = std::make_shared<OperationLinker>(m_graph);
	graphGrammar.AddRule(linkRule);
	graphGrammar.Run();
	*/

	/*
	// Execute all nodes. I.e., set the parameters
	query::Query nodes(m_graph, [&](NodePtr n) {
	  auto inNodes = m_graph.GetNodeInputNodes(n->GetName());
	  auto outNodes = m_graph.GetNodeOutputNodes(n->GetName());
	  n->Execute(inNodes, outNodes);
	});
	m_graph.ExecuteQuery(nodes);

	// Once we're done with the parameter nodes, we can destroy them
	// so that they aren't treated as input nodes (next step).
	// TODO: this should be handled better and the nodes should not be deleted
	std::list<NodePtr> paramNodes;
	query::Type<ParameterNode> parameterNodes(
	  m_graph, [&](NodePtr n) { paramNodes.push_back(n); });
	m_graph.ExecuteQuery(parameterNodes);
	for (const auto &p : paramNodes) {
	  m_graph.DestroyNode(p->GetName());
	}

	// Create the list of input nodes.
	query::InputNode inputNodes(m_graph, [&](NodePtr n) {
	  auto opNode = std::dynamic_pointer_cast<OperationNode>(n);
	  if (opNode != nullptr) {
	    m_operationQueue.push(opNode->GetOperation().get());
	  }
	});
	m_graph.ExecuteQuery(inputNodes);

	// Set the OnNeedsUpdate handler to push the operation on the queue
	query::Type<OperationNode> operationNodes(m_graph, [&](NodePtr n) {
	  auto op = std::dynamic_pointer_cast<OperationNode>(n)->GetOperation();
	  op->OnNeedsUpdate(
	    [&](ProceduralOperation *op) { m_operationQueue.push(op); });
	});
	m_graph.ExecuteQuery(operationNodes);
	*/
}

bool DefaultScheduler::Step()
{
	START_PROFILE;

	if (m_nodeQueue.empty()) {
		return false;
	}

	/*
	auto nextOperation = m_operationQueue.front();
	m_operationQueue.pop();

	nextOperation->Execute();
	*/

	auto nextNode = m_nodeQueue.front();
	m_nodeQueue.pop();
	nextNode->Execute(m_graph.GetNodeInputNodes(nextNode->GetName()),
	                  m_graph.GetNodeOutputNodes(nextNode->GetName()));

	return true;
}

void DefaultScheduler::Finalize()
{
}
}  // namespace pagoda::graph
