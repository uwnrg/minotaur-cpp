#include "simulator.h"

Simulator::Simulator() :
    Controller(false, false) {}

void Simulator::__move_delegate(Vector2i dir, int timer) {
    // Update the robot position
#ifndef NDEBUG
    debug() << "Moved " << dir << " in " << timer << " sec";
#endif
    robot_pos += dir;
}

Vector2i *Simulator::getRobotPos() {
    return &robot_pos;
}
