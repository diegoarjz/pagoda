#pragma once

#include <QGraphicsWidget>

#include <memory>

namespace pagoda::graph
{
class Node;
using NodePtr = std::shared_ptr<Node>;
}  // namespace pagoda::graph

namespace pgeditor::gui
{
class GraphPortConnection;

class GraphPort : public QGraphicsWidget
{
	public:
	GraphPort(pagoda::graph::NodePtr node, QGraphicsWidget *parent);
	virtual ~GraphPort() {}

	pagoda::graph::NodePtr GetNode() const;
	GraphPortConnection *GetPortConnection() const;

	protected:
	pagoda::graph::NodePtr m_node;
	GraphPortConnection *m_connection;
};

}  // namespace pgeditor::gui
