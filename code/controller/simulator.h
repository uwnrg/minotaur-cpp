#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "controller.h"
#include "../utility/vector2i.h"
#include "../gui/simulatorwindow.h"

class SimulatorScene;

class Simulator : public Controller {
public:
    Simulator(int t_invert_x = 1, int t_invert_y = 1);
    void move(Vector2i dir, int timer);
    void setSimulatorScene(SimulatorScene *simulator_scene);
    Vector2i *getRobotPos();

private:
    SimulatorScene* m_simulator_scene;
    Vector2i robot_pos;
};

#endif // SIMULATOR_H
