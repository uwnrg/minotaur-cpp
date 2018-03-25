#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "controller.h"
#include "../utility/vector2d.h"

class SimulatorScene;
class GlobalSim;

// Simulated controller, which directs inputs to an on-screen robot
class Simulator : public Controller {
Q_OBJECT

public:
    Simulator(GlobalSim *sim);

    void __move_delegate(Vector2i dir, int timer) override;

    // Get (x,y) position of the robot
    Vector2i *getRobotPos();

    Q_SIGNAL void move_right();
    Q_SIGNAL void move_left();
    Q_SIGNAL void move_down();
    Q_SIGNAL void move_up();

private:
    Vector2i robot_pos;
};

#endif // SIMULATOR_H
