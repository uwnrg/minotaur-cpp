#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../utility/logger.h"

#define STEP_TIME	10

struct Vector2i {
	int x_comp = 0;
	int y_comp = 0;
    Vector2i(int xp, int yp) : x_comp(xp), y_comp(yp) {}
};

class Controller
{
public:
    enum Mode {
        EMMA,
        SAM,

        NUM_MODES
    };

    enum Dir {
        UP,		// y > 0
        DOWN,	// y < 0
        RIGHT,	// x > 0
        LEFT,	// x < 0

        NUM_DIRS
    };

    enum Type {
        ACTUATOR,
        SIMULATOR,

        NUM_TYPES
    };

	enum Axis {
		X,
		Y,

		NUM_AXES
	};
    // Common robot functions
	static Vector2i toVec2(Dir);
	void invertAxis(Axis);
	// This is just a wrapper to convert from Dir enum to Vector2i parameter
	void move(Dir dir, int timer = STEP_TIME);
	virtual void move(Vector2i dir, int timer = STEP_TIME);
	
protected:
	Controller(int t_invert_x, int t_invert_y);
	int m_invert_x, m_invert_y; // +1 for no inversion in the axis, -1 otherwise
};

#endif // CONTROLLER_H
