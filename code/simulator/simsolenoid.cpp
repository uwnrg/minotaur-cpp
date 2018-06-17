#include "simsolenoid.h"

SimulatorSolenoid::SimulatorSolenoid(
    const RenderSceneBase *scene,
    const vector2f &P,
    float theta,
    float n,
    float R,
    float l,
    float mu_rel) :
    m_renderScene(scene),
    m_pos(P),
    m_theta(theta),
    m_N(n),
    m_radius(R),
    m_len(l),
    m_mu(mu_rel),
    m_I(0.0f) {}

void SimulatorSolenoid::setCurrent(float i) {
    m_I = i;
}

float SimulatorSolenoid::getCurrent() {
    return m_I;
}

inline float SimulatorSolenoid::getA(float x, float y) const {
    return powf((powf(x, 2.0f) + powf(y, 2.0f)), -1.5f);
}

void SimulatorSolenoid::draw(QPainter *painter, QPaintEvent *, int, float scale) {
    float s = sinf(m_theta);
    float c = cosf(m_theta);
    float l2 = m_len * scale / 2.0f;
    float dx1 = l2 * s;
    float dy1 = l2 * c;
    float dx2 = m_radius * scale / 2.0f * c;
    float dy2 = m_radius * scale / 2.0f * s;
    vector2f center = m_renderScene->center();
    float nx = m_pos.x() * scale + center.x() - dx1;
    float ny = -m_pos.y() * scale + center.y() - dy1;
    float sx = m_pos.x() * scale + center.x() + dx1;
    float sy = -m_pos.y() * scale + center.y() + dy1;
    const pointf vertices[4] = {
        pointf(nx - dx2, ny + dy2),
        pointf(nx + dx2, ny - dy2),
        pointf(sx + dx2, sy - dy2),
        pointf(sx - dx2, sy + dy2)
    };
    painter->setPen(Qt::magenta);
    painter->drawPolygon(vertices, 4);
}

typename SimulatorSolenoid::vector2f
SimulatorSolenoid::fieldAt(const vector2f &Q) const {
    vector2f Qp = rotate(translate(Q, m_pos), -m_theta);
    float k = MU_0F * m_mu * m_I * m_N * powf(m_radius, 2.0f) / 4.0f;
    float l2 = m_len / 2.0f;
    float yp1 = Qp.y() - l2;
    float yp2 = Qp.y() + l2;
    float a1 = getA(Qp.x(), yp1);
    float a2 = getA(Qp.x(), yp2);
    float Bx = k * Qp.x() * (a1 - a2);
    float By = k * (yp1 * a1 - yp2 * a2);
    return rotate(vector2f(Bx, By), m_theta);
}
