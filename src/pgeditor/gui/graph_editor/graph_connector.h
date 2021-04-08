#pragma once

#include "edge.h"

namespace pgeditor::gui
{
class GraphPortConnection;

class GraphConnector : public Edge
{
	public:
	GraphConnector(GraphPortConnection *out, GraphPortConnection *in);

	public slots:
	void PortConnectionChanged(GraphPortConnection *connection, const QPointF &newPos);

	private:
	GraphPortConnection *m_out;
	GraphPortConnection *m_in;
};  // class GraphConnector
}  // namespace pgeditor::gui
