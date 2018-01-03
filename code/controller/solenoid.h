#ifndef SOLENOID_H
#define SOLENOID_H

#include "controller.h"
#include <fstream>

class Solenoid : public Controller {
public:
    Solenoid(const QString& serial_port = "");
    ~Solenoid();
    void move(Vector2i dir, int timer);
private:
    enum Direction { UP = 0b1000, RIGHT = 0b0100, DOWN = 0b0010, LEFT = 0b0001 };
    std::ofstream stream;
    int vectorToBinary(Vector2i dir);
};

#endif // SOLENOID_H
