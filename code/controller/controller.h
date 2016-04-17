#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../utility/logger.h"

struct Vector2i {
	int x = 0;
	int y = 0;
	// TODO: Add ostream operator overloading
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

	enum Axis {
		X,
		Y,

		NUM_AXES
	};
    // Common robot functions
	void move(Dir);
	void invertAxis(Axis);
	virtual void move(Vector2i, int time = 0) { }

protected:
	int m_invert_x, m_invert_y;
};

#endif // CONTROLLER_H
