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
    void reset();
    void start();
    void stop();
    Vector2i *getRobotPos();

private:
    std::shared_ptr<SimulatorWindow> m_simulator_window;
    Vector2i robot_pos;
};

#endif // SIMULATOR_H
