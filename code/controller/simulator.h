#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "controller.h"
#include "../utility/vector2i.h"
#include "../gui/simulatorwindow.h"

class SimulatorScene;

// Simulated controller, which directs inputs to an on-screen robot
class Simulator : public Controller {
public:
    Simulator(int t_invert_x = 1, int t_invert_y = 1);
    void move(Vector2i dir, int timer);
    // Set pointer to the simulator scene where the robot is drawn
    void setSimulatorScene(SimulatorScene *simulator_scene);
    // Get (x,y) position of the robot
    Vector2i *getRobotPos();

private:
    SimulatorScene* m_simulator_scene;
    Vector2i robot_pos;
};

#endif // SIMULATOR_H
