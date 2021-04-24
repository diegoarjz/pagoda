#pragma once

#include "edge.h"

namespace pgeditor::gui::graph
{
class GraphPortConnection;
class GraphNode;

class GraphConnector : public Edge
{
	public:
	GraphConnector(GraphPortConnection *out, GraphPortConnection *in);

	GraphNode *GetUpstreamNode() const;
	GraphNode *GetDownstreamNode() const;

	public slots:
	void PortConnectionChanged(GraphPortConnection *connection, const QPointF &newPos);

	private:
	GraphPortConnection *m_out;
	GraphPortConnection *m_in;
};  // class GraphConnector
}  // namespace pgeditor::gui::graph
