#include "globalsim.h"
#include "../utility/random.h"

GlobalSim::GlobalSim() :
    m_robot(0, 0) {}

double GlobalSim::get_di() const {
    return rng::randf<DELTA_PREF_MIN, DELTA_PREF_MAX>();
}

double GlobalSim::get_dk() const {
    return rng::randf<-DELTA_ERR, DELTA_ERR>();
}

void GlobalSim::robot_reset() {
    m_robot = {0, 0};
}

void GlobalSim::robot_right() {
    // Right is +X
    m_robot.x() += get_di();
    m_robot.y() += get_dk();
}

void GlobalSim::robot_left() {
    // Left is -X
    m_robot.x() -= get_di();
    m_robot.y() += get_dk();
}

void GlobalSim::robot_down() {
    // Down is +Y
    m_robot.y() += get_di();
    m_robot.x() += get_dk();
}

void GlobalSim::robot_up() {
    // Up is -Y
    m_robot.y() -= get_di();
    m_robot.x() += get_dk();
}

vector2d<double> &GlobalSim::robot() {
    return m_robot;
}
