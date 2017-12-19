#include "sam.h"
#include "simulatorSolenoid.h"

SAMRobot::SAMRobot(RenderSceneBase *scene, float mu_s, float mass, float length)
        : m_renderScene(scene),
          m_fric(mu_s),
          m_mass(mass),
          m_len(length),
          m_pos(0, 0),
          m_vel(0, 0) {
}

void SAMRobot::draw(QPainter *painter, QPaintEvent *, int elapsed, float scale) {
    // Update phase
    float dt = elapsed / 1000.0f;
    const std::vector<SimulatorSolenoid> *solenoids = m_renderScene->solenoids();
    m_mag.setX(0);
    m_mag.setY(0);
    for (auto &solenoid : *solenoids) {
        m_mag += solenoid.fieldAt(m_pos);
    }
    float vel = hypotf(m_vel.x(), m_vel.y());
    if (vel > 0) {
        float sfric = G_FIELD * m_mass * m_fric;
        vector2f vfric(-sfric * m_vel.x() / vel, -sfric * m_vel.y() / vel);
        bool xdir = m_vel.x() > 0;
        bool ydir = m_vel.y() > 0;
        m_vel += dt * (m_mag + vfric);
        // Discrete time means we need to check if friction
        // causes a change in direction, that it stops SAM
        if (xdir != (m_vel.x() > 0) && ydir != (m_vel.y() > 0)) {
            m_vel.setX(0);
            m_vel.setY(0);
        }
    } else {
        m_vel += dt * m_mag;
    }

    m_pos += dt * m_vel;
    // Render phase
    float theta = atan2f(m_vel.y(), m_vel.x());
    float l2 = m_len * scale / 2.0f;
    float lx = l2 * cosf(theta);
    float ly = l2 * sinf(theta);
    vector2f center = m_renderScene->center();
    float tx = m_pos.x() * scale + center.x();
    float ty = -m_pos.y() * scale + center.y();
    const pointf vertices[4] = {
            pointf(tx + lx, ty + ly),
            pointf(tx - ly, ty + lx),
            pointf(tx - lx, ty - ly),
            pointf(tx + ly, ty - lx)
    };
    painter->setPen(Qt::green);
    painter->drawPolygon(vertices, 4);
}

const typename SAMRobot::vector2f &
SAMRobot::pos() const {
    return m_pos;
}

const typename SAMRobot::vector2f &
SAMRobot::vel() const {
    return m_vel;
}

const typename SAMRobot::vector2f &
SAMRobot::mag() const {
    return m_mag;
}

void SAMRobot::stop() {
    m_vel.setX(0);
    m_vel.setY(0);
}

void SAMRobot::reset() {
    m_pos.setX(0);
    m_pos.setY(0);
    stop();
}
