#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../utility/logger.h"

class Controller
{
public:
    enum Mode {
        EMMA,
        SAM,

        NUM_MODES
    };

    enum Dir {
        UP,
        DOWN,
        RIGHT,
        LEFT,

        NUM_DIRS
    };
    // Common robot functions
    virtual void move(Dir) {}
};

#endif // CONTROLLER_H
