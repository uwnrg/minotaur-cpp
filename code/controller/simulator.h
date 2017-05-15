#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "controller.h"

#define STEP_TIME 10

class Simulator : public Controller {
public:
    void move(Dir dir, int time = STEP_TIME);
    void move(Vector2i dir, int time = STEP_TIME);
};

#endif // SIMULATOR_H
