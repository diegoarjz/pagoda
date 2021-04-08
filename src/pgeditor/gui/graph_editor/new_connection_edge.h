#pragma once

#include "edge.h"

namespace pgeditor::gui
{
class GraphPortConnection;

/**
 * Represents a new edge between two nodes.
 */
class NewConnectionEdge : public Edge
{
	public:
	NewConnectionEdge(GraphPortConnection* port);
	~NewConnectionEdge() override;

	/**
	 * Sets position of the free end point.
	 */
	void CursorPositionChanged(const QPointF& pos);

	private:
	GraphPortConnection* m_port;
};
}  // namespace pgeditor::gui
