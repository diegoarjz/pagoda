#include "graph_port.h"

namespace pgeditor::gui
{
GraphPort::GraphPort(pagoda::graph::NodePtr node, QGraphicsWidget *parent) : QGraphicsWidget(parent), m_node(node) {}
GraphPortConnection *GraphPort::GetPortConnection() const { return m_connection; }

pagoda::graph::NodePtr GraphPort::GetNode() const { return m_node; }
}  // namespace pgeditor::gui
