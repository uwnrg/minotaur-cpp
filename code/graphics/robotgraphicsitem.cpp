#include "robotgraphicsitem.h"

RobotGraphicsItem::RobotGraphicsItem() : QGraphicsEllipseItem(0, 0, 15, 15) {
}

RobotGraphicsItem::~RobotGraphicsItem() {
}

void RobotGraphicsItem::paint(QPainter *painter,
                              const QStyleOptionGraphicsItem *option,
                              QWidget *widget) {
	painter->setBrush(QBrush(Qt::blue));
	painter->drawEllipse((int) x(), (int) y(), 10, 10);
}