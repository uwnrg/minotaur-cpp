#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "../utility/vector.h"
#include "controller.h"

class SimulatorScene;
class GlobalSim;

// Simulated controller, which directs inputs to an on-screen robot
class Simulator : public Controller {
Q_OBJECT

public:
    Simulator(GlobalSim *sim);

    void __move_delegate(vector2i dir, int timer) override;

    // Get (x,y) position of the robot
    vector2i *getRobotPos();

    Q_SIGNAL void move_right();
    Q_SIGNAL void move_left();
    Q_SIGNAL void move_down();
    Q_SIGNAL void move_up();

private:
    vector2i robot_pos;
};

#endif // SIMULATOR_H
