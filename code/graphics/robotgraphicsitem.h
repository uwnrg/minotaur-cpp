#ifndef ROBOTGRAPHICSITEM_H
#define ROBOTGRAPHICSITEM_H

#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class RobotGraphicsItem : public QGraphicsEllipseItem {
public:
	RobotGraphicsItem();
	~RobotGraphicsItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
};

#endif // ROBOTGRAPHICSITEM_H
