#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../utility/logger.h"
#include "../utility/vector.h"
#include <unordered_map>

class Controller : public QObject {
Q_OBJECT

public:
    enum Type {
        SIMULATOR,
        SOLENOID
    };

    enum Dir {
        UP,    // -Y
        DOWN,  // +Y
        RIGHT, // +X
        LEFT   // -X
    };

    enum Axis {
        X,
        Y
    };

    enum {
        STEP_TIME = 10,
        NUM_KEYS = 50
    };

    // Common robot functions
    virtual vector2i to_vector2i(Dir dir);

    // Movement
    void move(Dir dir, int timer = STEP_TIME);

    void move(vector2i dir, int timer = STEP_TIME);

    virtual void __move_delegate(vector2i dir, int timer) = 0;

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
    key_map m_keyMap{NUM_KEYS};

    // Variables are true if inputs to the axis are inverted
    bool m_invert_x;
    bool m_invert_y;
};

#endif // CONTROLLER_H
