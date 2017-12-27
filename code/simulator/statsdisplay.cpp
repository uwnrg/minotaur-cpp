#include <QPainter>

#include "statsdisplay.h"
#include "sam.h"

StatsDisplay::StatsDisplay(RenderSceneBase *render_scene)
        : m_render_scene(render_scene) {}

void StatsDisplay::draw(QPainter *painter, QPaintEvent *, int, float) {
    QString pos, vel, mag;
    const SAMRobot *sam = m_render_scene->sam();
    char format[] = "(%.3g, %.3g)";
    pos.sprintf(format, sam->pos().x(), sam->pos().y());
    vel.sprintf(format, sam->vel().x(), sam->vel().y());
    mag.sprintf(format, sam->mag().x(), sam->mag().y());
    painter->setPen(Qt::white);
    painter->drawText(5, 15, "Position");
    painter->drawText(5, 35, "Velocity");
    painter->drawText(5, 55, "Magnetic");
    painter->drawText(65, 15, pos);
    painter->drawText(65, 35, vel);
    painter->drawText(65, 55, mag);
}
