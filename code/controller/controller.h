#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <unordered_map>
#include "../utility/logger.h"
#include "../utility/vector2i.h"

#define STEP_TIME    10

class Controller : public QObject {
Q_OBJECT

public:
    enum Mode {
        EMMA,
        SAM,

        NUM_MODES
    };

    enum Type {
        ACTUATOR,
        SIMULATOR,
        SOLENOID,

        NUM_DELGATES
    };

    enum Dir {
        UP,        // y > 0
        DOWN,    // y < 0
        RIGHT,    // x > 0
        LEFT,    // x < 0

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

    void move(Dir dir, int timer = STEP_TIME);

    virtual void move(Vector2i dir, int timer = STEP_TIME) = 0;

    void keyPressed(int key);

    void keyReleased(int key);

    bool isKeyDown(int key);

protected:
    typedef typename std::unordered_map<int, bool> key_map;
    typedef typename std::pair<int, bool> key_press;

    Controller(int t_invert_x, int t_invert_y);

    key_map m_keyMap{50};
    int m_invert_x, m_invert_y; // +1 for no inversion in the axis, -1 otherwise
};

#endif // CONTROLLER_H
