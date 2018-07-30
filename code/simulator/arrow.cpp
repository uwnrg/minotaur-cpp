#include "arrow.h"
#include "renderscenebase.h"
#include <QPainter>

Arrow::Arrow(RenderSceneBase *scene, const vector2f &P, const vector2f &B)
        : m_renderScene(scene),
          m_pos(P),
          m_mag(B) {
}

void Arrow::draw(QPainter *painter, QPaintEvent *, int, float scale) {
    vector2f center = m_renderScene->center();
    // Transform onto graphics plane
    m_pos.setX(m_pos.x() * scale + center.x());
    m_pos.setY(-m_pos.y() * scale + center.y());
    m_mag.setY(-m_mag.y());
    float a = sqrtf(m_mag.x() * m_mag.x() + m_mag.y() * m_mag.y());
    float xhat = m_mag.x() / a;
    float yhat = m_mag.y() / a;
    float x = ARR_LEN * xhat;
    float y = ARR_LEN * yhat;
    float x2 = x / 2.0f;
    float y2 = y / 2.0f;
    float ex = m_pos.x() + x2;
    float ey = m_pos.y() + y2;
    float sx = m_pos.x() - x2;
    float sy = m_pos.y() - y2;
    float tx = ex - ARR_Y * xhat;
    float ty = ey - ARR_Y * yhat;
    pointf vertices[3] = {
            pointf(ex, ey),
            pointf(tx + ARR_X * yhat, ty - ARR_X * xhat),
            pointf(tx - ARR_X * yhat, ty + ARR_X * xhat)
    };
    painter->setPen(Qt::white);
    painter->drawLine(pointf(ex, ey), pointf(sx, sy));
    painter->drawPolygon(vertices, 3);
}
