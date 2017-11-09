#include "simulator.h"
#include "../graphics/simulatorscene.h"

Simulator::Simulator(int t_invert_x, int t_invert_y) :
        Controller(t_invert_x, t_invert_y) {
}

void Simulator::move(Vector2i dir, int timer) {
    // Update the robot position
#ifndef NDEBUG
    Logger::log("Moved " + dir.toString() + " in " + std::to_string(timer) + " sec", Logger::DEBUG);
#endif
    dir.x_comp *= m_invert_x;
    dir.y_comp *= m_invert_y;
    robot_pos += dir;

    // Update the graphics scene
    m_simulator_scene->update();
}

void Simulator::setSimulatorScene(SimulatorScene *simulator_scene) {
    m_simulator_scene = simulator_scene;
}

Vector2i *Simulator::getRobotPos() {
    return &robot_pos;
}

