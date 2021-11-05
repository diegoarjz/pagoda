#include "writer.h"

#include "graph_reader_grammar.h"

#include "graph_definition_node.h"

#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>
#include <pagoda/graph/parameter_input_node.h>
#include <pagoda/graph/parameter_node.h>
#include <pagoda/graph/query/topology.h>
#include <pagoda/graph/query/type.h>
#include <pagoda/graph/traversal/forward.h>

#include <pagoda/objects/parameter.h>

#include <pagoda/common/debug/assertions.h>

#include <pagoda/graph/graph.h>

using namespace pagoda::graph;
using namespace pagoda::graph::traversal;
using namespace pagoda::dynamic;

namespace pagoda::graph::io
{
struct GraphWriter::Impl
{
	Impl(GraphPtr graph) : m_graph{graph}
	{
	}

	void Write(std::ostream& out)
	{
		//
		// Go through all OperationNode and write them
		//
		NodeSet operationNodes;
		auto opQuery = query::type<OperationNode>(*m_graph, operationNodes);
		m_graph->ExecuteQuery(opQuery);

		for (const auto& n : operationNodes) {
			auto op = std::dynamic_pointer_cast<OperationNode>(n);
			DBG_ASSERT_MSG(op != nullptr, "op should be an operation node");
			out << op->GetName() << " = " << op->GetOperation()->GetOperationName()
			    << " {" << std::endl;
			std::vector<std::string> parameters;
			op->ForEachParameter([&parameters](objects::ParameterBasePtr p) {
				// We don't write the operation name because it's implied in the node
				// creation
				if (p->GetName() == "operation") {
					return;
				}
				std::stringstream ss;
				ss << p->GetName() << ": ";
				if (p->HasExpression()) {
					ss << "$<" << p->GetExpression() << ">$";
				} else {
					ss << p->ToString();
				}
				parameters.emplace_back(ss.str());
			});
			for (auto i = 0u; i < parameters.size(); ++i) {
				out << "  " << parameters[i];
				if (i + 1 != parameters.size()) {
					out << ",";
				}
				out << "\n";
			}
			out << "}" << std::endl;
		}

		//
		// Write all node links
		//
		auto upstreamOp = query::type<OperationNode>();
		auto outInterface = query::type<OutputInterfaceNode>();
		auto inputInterface = query::type<InputInterfaceNode>();
		auto downstreamOp = query::type<OperationNode>();
		query::QueryTopology q(&upstreamOp);
		q.AddDownstreamQuery(&upstreamOp, &outInterface);
		q.AddDownstreamQuery(&outInterface, &inputInterface);
		q.AddDownstreamQuery(&inputInterface, &downstreamOp);

		std::vector<query::QueryTopology::Match_t> matches;
		q.Match(m_graph.get(), matches);

		for (auto& m : matches) {
			auto upstreamNode = m[&upstreamOp];
			auto outInterfaceNode =
			  std::dynamic_pointer_cast<OutputInterfaceNode>(m[&outInterface]);
			auto inInterfaceNode =
			  std::dynamic_pointer_cast<InputInterfaceNode>(m[&inputInterface]);
			auto downstreamNode = m[&downstreamOp];

			out << upstreamNode->GetName() << ">"
			    << outInterfaceNode->GetInterfaceName() << " -> "
			    << inInterfaceNode->GetInterfaceName() << "<"
			    << downstreamNode->GetName() << ";" << std::endl;
		}
	}

	GraphPtr m_graph;
};

GraphWriter::GraphWriter(GraphPtr graph)
  : m_implementation{std::make_unique<GraphWriter::Impl>(graph)}
{
}
GraphWriter::~GraphWriter()
{
}

void GraphWriter::Write(std::ostream& outStream)
{
	m_implementation->Write(outStream);
}
}  // namespace pagoda::graph::io
