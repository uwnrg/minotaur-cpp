#include "simulator.h"
#include "../graphics/simulatorscene.h"

Simulator::Simulator(int t_invert_x, int t_invert_y) :
        Controller(t_invert_x, t_invert_y) {
    m_simulator_window = std::shared_ptr<SimulatorWindow>(new SimulatorWindow(std::shared_ptr<Simulator>(this)));
}

void Simulator::move(Vector2i dir, int timer) {
    // Update the robot position
    robot_pos += dir * 3;
    Logger::log("Moved " + dir.to_string(), Logger::INFO);

    // Update the graphics scene
    m_simulator_window->getSimulatorScene()->update();
}

void Simulator::reset() {
    robot_pos = Vector2i();
}

void Simulator::start() {
    reset();
    if (!m_simulator_window->isVisible()) m_simulator_window->show();
}

void Simulator::stop() {
    reset();
    if (m_simulator_window->isVisible()) m_simulator_window->hide();
}

Vector2i *Simulator::getRobotPos() {
    return &robot_pos;
}

