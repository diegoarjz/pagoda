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


void DefaultScheduler::Initialize()
{
	traversal::Forward f(m_graph);
	while (f.HasNext()) {
		m_nodeQueue.push(f.Get());
		f.Advance();
	}

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
