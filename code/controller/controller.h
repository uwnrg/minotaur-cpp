#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <unordered_map>
#include "../utility/logger.h"
#include "code/utility/vector2d.h"

#define STEP_TIME 10

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

    // Movement
    void move(Dir dir, int timer = STEP_TIME);

    void move(Vector2i dir, int timer = STEP_TIME);

    virtual void __move_delegate(Vector2i dir, int timer) = 0;

    // Key press functions
    void keyPressed(int key);

    void keyReleased(int key);

    bool isKeyDown(int key);

    // Functions and slots to control axis inversion
    void invertAxis(Axis);

    Q_SLOT void invert_x_axis();

    Q_SLOT void invert_y_axis();


protected:
    typedef typename std::unordered_map<int, bool> key_map;
    typedef typename std::pair<int, bool> key_press;

    Controller(bool invert_x, bool invert_y);

private:
    key_map m_keyMap{50};

    // Variables are true if inputs to the axis are inverted
    bool m_invert_x;
    bool m_invert_y;
};

#endif // CONTROLLER_H
