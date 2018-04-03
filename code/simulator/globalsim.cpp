#include "globalsim.h"
#include "../utility/random.h"
#include "../utility/rect.h"
#include "../utility/algorithm.h"

GlobalSim::GlobalSim() :
    m_robot(0, 0),
    m_object(40, 0) {}

double GlobalSim::get_di() const {
    return rng::randf<DELTA_PREF_MIN, DELTA_PREF_MAX>();
}

double GlobalSim::get_dk() const {
    return rng::randf<-DELTA_ERR, DELTA_ERR>();
}

void GlobalSim::robot_reset() {
    m_robot = {0, 0};
    check_collide();
}

void GlobalSim::robot_right() {
    // Right is +X
    m_robot.x() += get_di();
    m_robot.y() += get_dk();
    check_collide();
}

void GlobalSim::robot_left() {
    // Left is -X
    m_robot.x() -= get_di();
    m_robot.y() += get_dk();
    check_collide();
}

void GlobalSim::robot_down() {
    // Down is +Y
    m_robot.y() += get_di();
    m_robot.x() += get_dk();
    check_collide();
}

void GlobalSim::robot_up() {
    // Up is -Y
    m_robot.y() -= get_di();
    m_robot.x() += get_dk();
    check_collide();
}

vector2d &GlobalSim::robot() {
    return m_robot;
}

vector2d &GlobalSim::object() {
    return m_object;
}

void GlobalSim::check_collide() {
    rect2d robot(m_robot.x() - WIDTH / 2, m_robot.y() - WIDTH / 2, WIDTH, WIDTH);
    rect2d object(m_object.x() - WIDTH * 3 / 4, m_object.y() - WIDTH / 2, WIDTH * 3 / 2, WIDTH);
    if (algo::aabb_collide(robot, object)) {
        vector2d resolve = algo::resolve_aabb_collide(object, robot);
        double pref = rng::randf<0, RESOLVE_PREF>();
        double err = rng::randf<-RESOLVE_ERR, RESOLVE_ERR>();
        if (resolve.x() > 0) {
            resolve.x() += pref;
            resolve.y() += err;
        } else if (resolve.x() < 0) {
            resolve.x() -= pref;
            resolve.y() += err;
        } else if (resolve.y() > 0) {
            resolve.y() += pref;
            resolve.x() += err;
        } else if (resolve.y() < 0) {
            resolve.y() -= pref;
            resolve.x() += err;
        }
        m_object += resolve;
        check_collide();
    } else {
        return;
    }
}
