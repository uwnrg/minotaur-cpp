#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "controller.h"
#include "../utility/vector2i.h"

class SimulatorScene;

// Simulated controller, which directs inputs to an on-screen robot
class Simulator : public Controller {
public:
    Simulator(int t_invert_x = 1, int t_invert_y = 1);
    void move(Vector2i dir, int timer);
    // Get (x,y) position of the robot
    Vector2i *getRobotPos();

private:
    Vector2i robot_pos;
};

#endif // SIMULATOR_H
