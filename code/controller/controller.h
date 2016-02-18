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
    Controller(Mode);
    void move(Dir);
    void change_mode (Mode);
    ~Controller();
private:
    Mode m_mode;
};

#endif // CONTROLLER_H
