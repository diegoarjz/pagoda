#include "graph_dot_exporter.h"

#include "breadth_first_node_visitor.h"
#include "execution_queue.h"
#include "graph.h"
#include "node.h"

namespace pagoda::graph
{
GraphDotExporter::GraphDotExporter(GraphPtr graph)
    : m_graph(graph), m_rankBy(RankBy::None), m_rankDirection(RankDirection::LeftToRight), m_showParameters(false)
{
}

GraphDotExporter::~GraphDotExporter() {}

void GraphDotExporter::GraphDotExporter::SetRankBy(const RankBy &rankBy) { m_rankBy = rankBy; }

GraphDotExporter::RankBy GraphDotExporter::GetRankBy() const { return m_rankBy; }

void GraphDotExporter::SetRankDirection(const RankDirection &rankDirection) { m_rankDirection = rankDirection; }

GraphDotExporter::RankDirection GraphDotExporter::GetRankDirection() const { return m_rankDirection; }

void GraphDotExporter::SetShowParameters(const bool &show) { m_showParameters = show; }

bool GraphDotExporter::GetShowParameters() const { return m_showParameters; }

void GraphDotExporter::Export(std::ostream &outStream)
{
	auto nodes = GetNodes();

	outStream << "digraph {\n";
	outStream << "\trankdir=" << (m_rankDirection == RankDirection::LeftToRight ? "LR" : "TB") << ";\n";
	outStream << "\tnode [shape=plaintext fontname=\"Sans serif\" fontsize=\"8\"];\n";

	for (auto n : nodes)
	{
		auto node = n.m_node;
		// TODO: Rank
		outStream << "\t" << node->GetName() << " [ label=<\n";
		outStream << "<table border=\"1\" cellborder=\"0\" cellspacing=\"1\">\n";
		outStream << "<tr><td align=\"left\"><b>" << node->GetName() << "</b></td></tr>\n";

		if (m_showParameters)
		{
			for (auto parIter = node->GetMembersBegin(); parIter != node->GetMembersEnd(); ++parIter)
			{
				outStream << "<tr><td>" << parIter->first << "</td></tr>\n";
			}
		}

		outStream << "</table>>]\n\n";

		auto outNodes = m_graph->GetNodeOutputNodes(node);
		std::string sourceNodeName = node->GetName();

		for (auto outNode : outNodes)
		{
			std::string targetNodeName = outNode->GetName();
			outStream << "\t" << sourceNodeName << " -> " << targetNodeName << ";" << std::endl;
		}
	}

	outStream << "}";
}

std::vector<GraphDotExporter::NodeExportInfo> GraphDotExporter::GetNodes()
{
	std::vector<NodeExportInfo> nodes;
	nodes.reserve(m_graph->GetNodeCount());

	switch (m_rankBy)
	{
		case RankBy::None:
		{
			auto allNodes = m_graph->GetGraphNodes();
			std::transform(allNodes.begin(), allNodes.end(), std::back_inserter(nodes), [](const NodePtr &n) {
				return NodeExportInfo{n, 0};
			});
			break;
		}
		case RankBy::Depth:
		{
			std::unordered_map<NodePtr, uint32_t> depths;
			auto delegate = [&depths, this](const NodePtr &n) {
				auto thisNodeDepth = depths.emplace(n, 0).first->second;
				for (auto outNode : this->m_graph->GetNodeOutputNodes(n))
				{
					if (depths[outNode] <= thisNodeDepth)
					{
						depths[outNode] = thisNodeDepth + 1;
					}
				}
			};

			BreadthFirstNodeVisitor<decltype(delegate)> visitor(*m_graph, delegate);
			visitor.Visit();

			std::transform(depths.begin(), depths.end(), std::back_inserter(nodes),
			               [](const std::pair<NodePtr, uint32_t> &n) {
				               return NodeExportInfo{n.first, n.second};
			               });

			break;
		}
		case RankBy::ExecutionOrder:
		{
			ExecutionQueue q(*m_graph);
			NodePtr n;
			uint32_t rank = 0;
			auto inserter = std::back_inserter(nodes);

			while ((n = q.GetNextNode()) != nullptr)
			{
				inserter = NodeExportInfo{n, rank++};
			}

			break;
		}
	};

	std::sort(nodes.begin(), nodes.end(), [](const NodeExportInfo &n1, const NodeExportInfo &n2) {
		return n1.m_node->GetId() < n2.m_node->GetId();
	});

	return nodes;
}
}  // namespace pagoda::graph

