#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../utility/logger.h"

struct Vector2i {
	int x_comp = 0;
	int y_comp = 0;
	Vector2i(int xp, int yp) : x_comp(xp), y_comp(yp) {};
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
	static Vector2i toVec2(Dir);
	void invertAxis(Axis);
	
protected:
	int m_invert_x, m_invert_y;
};

#endif // CONTROLLER_H
