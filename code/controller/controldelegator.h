#ifndef CONTROLDELEGATOR_H
#define CONTROLDELEGATOR_H

#include <memory>
#include "actuator.h"
#include "simulator.h"

class ControlDelegator : public Controller {
public:
    enum Delegate {
        ACTUATOR,
        SIMULATOR,

        NUM_DELGATES
    };

    ControlDelegator(int t_invert_x = 1, int t_invert_y = 1, Delegate delegate = ACTUATOR);

    inline void move (Controller::Dir dir, int timer = STEP_TIME) { Controller::move(dir, timer); }
    void move(Vector2i dir, int timer = STEP_TIME);
    void switchTo(Delegate delegate);

    std::shared_ptr<Actuator> getActuator();
    std::shared_ptr<Simulator> getSimulator();

private:
    Delegate m_delegate_type;
    std::shared_ptr<Controller> m_controller;
    std::shared_ptr<Actuator> m_actuator;
    std::shared_ptr<Simulator> m_simulator;
};

#endif // CONTROLDELEGATOR_H