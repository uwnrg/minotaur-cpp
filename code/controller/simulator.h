#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "controller.h"
#include "../utility/vector2d.h"

class SimulatorScene;

// Simulated controller, which directs inputs to an on-screen robot
class Simulator : public Controller {
public:
    Simulator();

    void __move_delegate(Vector2i dir, int timer) override;

    // Get (x,y) position of the robot
    Vector2i *getRobotPos();

private:
    Vector2i robot_pos;
};

#endif // SIMULATOR_H
