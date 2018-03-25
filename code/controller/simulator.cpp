#include "simulator.h"
#include "../simulator/globalsim.h"

Simulator::Simulator(GlobalSim *sim) :
    Controller(false, false) {
    connect(this, &Simulator::move_right, sim, &GlobalSim::robot_right);
    connect(this, &Simulator::move_left, sim, &GlobalSim::robot_left);
    connect(this, &Simulator::move_down, sim, &GlobalSim::robot_down);
    connect(this, &Simulator::move_up, sim, &GlobalSim::robot_up);
}

void Simulator::__move_delegate(Vector2i dir, int timer) {
    // Update the robot position
#ifndef NDEBUG
    debug() << "Moved " << dir << " in " << timer << " sec";
#endif
    robot_pos += dir;
    if (dir.x() > 0) {
        Q_EMIT move_right();
    } else if (dir.x() < 0) {
        Q_EMIT move_left();
    } else if (dir.y() > 0) {
        Q_EMIT move_down();
    } else if (dir.y() < 0) {
        Q_EMIT move_up();
    }
}

Vector2i *Simulator::getRobotPos() {
    return &robot_pos;
}
