#include "controldelegator.h"

ControlDelegator::ControlDelegator(int t_invert_x, int t_invert_y, Delegate delegate) :
        Controller(t_invert_x, t_invert_y), m_delegate_type(delegate) {
    m_actuator = std::shared_ptr<Actuator>(new Actuator);
    m_simulator = std::shared_ptr<Simulator>(new Simulator);
    if (delegate == ACTUATOR) m_controller = m_actuator;
    else if (delegate == SIMULATOR) m_controller = m_simulator;
}

void ControlDelegator::move(Vector2i dir, int timer) {
    m_controller->move(dir, timer);
}


void ControlDelegator::switchTo(Delegate delegate) {
    Logger::log("Switching to delegate: " + std::to_string(delegate), Logger::DEBUG);
    if (delegate == m_delegate_type) {
        Logger::log("Switching to same type", Logger::DEBUG);
        return;
    }
    m_delegate_type = delegate;
    if (delegate == ACTUATOR) {
        Logger::log("Switching to ACTUATOR", Logger::DEBUG);
        m_simulator->stop();
        m_controller = m_actuator;

    } else if (delegate == SIMULATOR) {
        Logger::log("Switching to SIMULATOR", Logger::DEBUG);
        m_controller = m_simulator;
        m_simulator->start();
    }
}

std::shared_ptr<Actuator> ControlDelegator::getActuator() {
    return m_actuator;
}

std::shared_ptr<Simulator> ControlDelegator::getSimulator() {
    return m_simulator;
}
