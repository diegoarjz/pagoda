#pragma once

#include <QGraphicsWidget>

namespace pgeditor::gui
{
class GraphPortConnection : public QGraphicsWidget
{
	Q_OBJECT
	public:
	enum class NodeSide
	{
		Left,
		Right
	};

	GraphPortConnection(QGraphicsWidget *parent, NodeSide side);
	~GraphPortConnection() override;

	NodeSide GetNodeSide() const;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	signals:
	void PositionChanged(GraphPortConnection *connection, const QPointF &newPostion);

	protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	private:
	NodeSide m_nodeSide;
};

}  // namespace pgeditor::gui
