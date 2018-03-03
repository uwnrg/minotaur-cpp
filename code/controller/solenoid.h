#ifndef SOLENOID_H
#define SOLENOID_H

#include "controller.h"

#include <fstream>
#include <queue>

#include <QObject>
#include <QSerialPort>

class Solenoid : public Controller {
Q_OBJECT
public:
    explicit Solenoid(const QString &serial_port = "");

    ~Solenoid() override;

    void __move_delegate(Vector2i dir, int timer) override;

    Q_SLOT void readSerial();

    Q_SIGNAL void serialRead(const std::string &msg);

    static char vectorToBinary(Vector2i dir);

private:
    enum Direction {
        UP = 0b1000,
        RIGHT = 0b0100,
        DOWN = 0b0010,
        LEFT = 0b0001
    };

    QSerialPort m_serial;
};

#endif // SOLENOID_H
