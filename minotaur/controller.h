#ifndef CONTROLLER_H
#define CONTROLLER_H

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

class Controller
{

public:
    virtual Controller(Mode);
    virtual void move(Dir);
    virtual change_mode (Mode);
    virtual ~Controller();
private:
    Mode m_mode;
};

#endif // CONTROLLER_H
