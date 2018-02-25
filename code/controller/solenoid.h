#ifndef SOLENOID_H
#define SOLENOID_H

#include "controller.h"
#include <fstream>
#include <queue>

class Solenoid : public Controller {
Q_OBJECT
public:
    Solenoid(const QString& serial_port = "");
    ~Solenoid();
    void move(Vector2i dir, int timer);
    void queueInstruction(Dir dir);

public Q_SLOTS:
    void executeInstruction();

private:
    enum Direction { UP = 0b1000, RIGHT = 0b0100, DOWN = 0b0010, LEFT = 0b0001 };
    std::ofstream stream;
    int vectorToBinary(Vector2i dir);
    std::queue<Dir> m_directions;
};

#endif // SOLENOID_H
