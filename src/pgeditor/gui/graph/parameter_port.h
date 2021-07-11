#pragma once

#include <QGraphicsProxyWidget>

#include "graph_port.h"

namespace pgeditor::gui::graph
{
class ParameterPort : public GraphPort
{
	public:
	ParameterPort(GraphNode *node, const std::string &paramName, QGraphicsWidget *parent);
	~ParameterPort() override {}

	QRectF boundingRect() const override;

	private:
	QGraphicsProxyWidget *m_label;
	const std::string m_paramName;
};
}  // namespace pgeditor::gui::graph
