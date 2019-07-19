#ifndef SELECTOR_PROCEDURAL_GRAPH_GRAPH_DOT_EXPORTER_H_
#define SELECTOR_PROCEDURAL_GRAPH_GRAPH_DOT_EXPORTER_H_

#include <iostream>
#include <memory>

namespace selector
{
class Graph;
using GraphPtr = std::shared_ptr<Graph>;

class GraphDotExporter
{
public:
	/**
	 * Defines how the nodes in the graph should be ranked.
	 */
	enum class RankBy
	{
		None,				///< Don't specify any ranking.
		Depth,				///< Rank by depth to a graph input node.
		ExecutionOrder		///< Rank by the execution order.
	};
	
	/**
	 * Defines the rank direction.
	 */
	 enum class RankDirection
	 {
	 	LeftToRight,		///< Displays the nodes from left to right.
	 	TopToBottom			///< Displays the nodes from top to bottom.
	 };

	GraphDotExporter(GraphPtr graph);
	~GraphDotExporter();

	/**
	 * Sets the rank by mode.
	 */
	void SetRankBy(const RankBy &rankBy);
	
	/**
	 * Gets the rank by mode.
	 */
	RankBy GetRankBy() const;
	
	/**
	 * Sets the rank direction mode.
	 */
	void SetRankDirection(const RankDirection &rankDirection);
	
	/**
	 * Gets the rank direction mode.
	 */
	RankDirection GetRankDirection() const;
	
	/**
	 * Defines whether \c Node parameters should be shown or not.
	 */
	void SetShowParameters(bool &show);
	
	/**
	 * Gets whether the \c Node parameters are to be shown or not.
	 */
	 bool GetShowParameters() const;
	 
	 /**
	  * Exports the graph to \p outStream.
	  */
	void Export(std::ostream &outStream);
	
private:

	/**
	 * The information about how to export a \c Node.
	 */
	struct NodeExportInfo
	{
		NodePtr m_node;		///< The \c Node to export.
		uint32_t m_rank;	///< The node's rank.
	};

	/**
	 * Returns the nodes to be exported along with their information.
	 */
	std::vector<NodeExportInfo> GetNodes();

	GraphPtr m_graph;
	RankBy m_rankBy;
	RankDirection m_rankDirection;
	bool m_showParameters;
};
}

#endif
