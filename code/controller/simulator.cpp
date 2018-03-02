#include "simulator.h"

Simulator::Simulator() :
    Controller(true, false) {}

void Simulator::move(Vector2i dir, int timer) {
    // Update the robot position
#ifndef NDEBUG
    debug() << "Moved " << dir << " in " + std::to_string(timer) + " sec";
#endif
    dir.y() *= m_invert_x ? -1 : 1;
    dir.x() *= m_invert_y ? -1 : 1;
    robot_pos += dir;
}

Vector2i *Simulator::getRobotPos() {
    return &robot_pos;
}

