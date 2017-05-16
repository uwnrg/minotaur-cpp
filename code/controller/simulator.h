#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "controller.h"

class Simulator : public Controller {
public:
	Simulator(int t_invert_x = 1, int t_invert_y = 1);
	void move(Vector2i dir, int timer = STEP_TIME);
};

#endif // SIMULATOR_H
