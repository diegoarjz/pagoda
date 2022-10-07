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

    auto outputInterface = upstream->GetOutputInterface(outInterface->GetInterfaceName());
    auto inputInterface = downstream->GetInputInterface(inInterface->GetInterfaceName());

    if (outputInterface == nullptr || inputInterface == nullptr) {
      LOG_ERROR("Unable to find matching input or output interfaces.");
      LOG_ERROR(" Output interface " << upstream->GetName() << "." << outInterface->GetInterfaceName() << " is " << outputInterface.get());
      LOG_ERROR(" Input interface " << downstream->GetName() << "." << inInterface->GetInterfaceName() << " is " << inputInterface.get());
      return;
    }

    if (!inputInterface->Accepts(outputInterface)) {
      LOG_ERROR("Unable to connect interfaces.");
      LOG_ERROR(" Output interface " << upstream->GetName() << "." << outInterface->GetInterfaceName() << " is " << outputInterface.get());
      LOG_ERROR(" Input interface " << downstream->GetName() << "." << inInterface->GetInterfaceName() << " is " << inputInterface.get());
    }

    inputInterface->Connect(outputInterface);
	}

	private:
	Type<OperationNode> m_upstreamNode;
	Type<OutputInterfaceNode> m_outputInterface;
	Type<InputInterfaceNode> m_inputInterface;
	Type<OperationNode> m_downstreamNode;
	QueryTopology m_topology;
};

void DefaultScheduler::Initialize()
{
	traversal::Forward f(m_graph);
	while (f.HasNext()) {
		m_nodeQueue.push(f.Get());
		f.Advance();
	}
	GraphGrammar graphGrammar(m_graph);

	auto linkRule = std::make_shared<OperationLinker>(m_graph);
	graphGrammar.AddRule(linkRule);
	graphGrammar.Run();

  query::Type<OperationNode> operations(m_graph, [](NodePtr n) {
    std::dynamic_pointer_cast<OperationNode>(n)->GetOperation()->Prepare();
  });
  m_graph.ExecuteQuery(operations);
}

bool DefaultScheduler::Step()
{
	START_PROFILE;

	if (m_nodeQueue.empty()) {
		return false;
	}

	auto nextNode = m_nodeQueue.front();
	m_nodeQueue.pop();
	nextNode->Execute(m_graph.GetNodeInputNodes(nextNode->GetName()),
	                  m_graph.GetNodeOutputNodes(nextNode->GetName()));

	return true;
}

void DefaultScheduler::Finalize()
{

  query::Type<OperationNode> operations(m_graph, [](NodePtr n) {
    std::dynamic_pointer_cast<OperationNode>(n)->GetOperation()->Finish();
  });
  m_graph.ExecuteQuery(operations);
}
}  // namespace pagoda::graph
