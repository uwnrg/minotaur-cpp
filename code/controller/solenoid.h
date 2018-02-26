#ifndef SOLENOID_H
#define SOLENOID_H

#include "controller.h"

#include <fstream>
#include <queue>

#include <QObject>
#include <QSerialPort>
#include <QMutex>

class Solenoid : public Controller, public QObject {
Q_OBJECT

public:
    Solenoid(const QString &serial_port = "");

    ~Solenoid();

    void move(Vector2i dir, int timer);

private:

    std::ofstream m_stream;
};

class MovementExecutor : public QObject {
Q_OBJECT

public:
    static int vector_to_bin(Vector2i dir);

    explicit MovementExecutor(Solenoid *solenoid);

    MovementExecutor(Solenoid *solenoid, const std::string &port);

    Q_SLOT void queue_movement(Vector2i vec);

    Q_SLOT void wake();

    Q_SLOT void sleep();

    Q_SLOT void execute_movement();

private:
    enum Direction {
        UP = 0b1000,
        RIGHT = 0b0100,
        DOWN = 0b0010,
        LEFT = 0b0001
    };

    QMutex m_mutex;
    QSerialPort m_serial;
    Solenoid *m_solenoid;
    std::queue<Vector2i> m_buffer;
};

#endif // SOLENOID_H
