#include "solenoid.h"

MovementExecutor::MovementExecutor(Solenoid *solenoid)
    : m_solenoid(solenoid) {}

MovementExecutor::MovementExecutor(Solenoid *solenoid, const std::string &port)
    : m_solenoid(solenoid) {
    m_stream.open(port);
    if (!m_stream.is_open()) {
        Logger::log("Failed to connect to serial port: " + port, Logger::FATAL);
        m_stream.close();
    }
}

void MovementExecutor::queue_movement(Vector2i dir) {
    m_buffer.push(dir);
}

void MovementExecutor::execute_movement() {
    if (m_buffer.empty()) {
        return;
    }
    m_mutex.lock();
    Vector2i next_movement = m_buffer.front();
    m_buffer.pop();
    m_mutex.unlock();
}

Solenoid::Solenoid(const QString &serial_port) : Controller(1, 1) {
    // TODO: change actuator setup to be used for general serial set up
    if (serial_port.isEmpty()) {
        // TODO: provide error if this does not succeed
        m_stream.open("/dev/ttyACM0");        // manually changed to match port indicated in arduino IDE
    } else {
        m_stream.open(serial_port.toStdString());
    }
}

Solenoid::~Solenoid() {
    m_stream.close();
}

void Solenoid::move(Vector2i dir, int timer) {
    // TODO: user timer for movement control
    bool success = true;
#ifndef NDEBUG
    Logger::log("Moving Solenoid controller", Logger::DEBUG);
    Logger::log("Attempting move (" + std::to_string(dir.x_comp) + ", " + std::to_string(dir.y_comp) + ")",
                Logger::DEBUG);
#endif
    try {
        m_stream << vectorToBinary(dir);
        m_stream.flush();
    } catch (...) {
        success = false;
#ifndef NDEBUG
        Logger::log("Unexpected exception", Logger::DEBUG);
#endif
    }

    if (success) {
        Logger::log("Moved " + dir.toString() + " in " + std::to_string(timer) + " milliseconds.", Logger::INFO);
    } else {
        Logger::log("The movement " + dir.toString() + " could not be completed.", Logger::FATAL);
    }
}

int MovementExecutor::vector_to_bin(Vector2i dir) {
    int direction = 0b0;

    switch (dir.x_comp) {
        case 1:
            direction = direction | Direction::UP;
            break;
        case -1:
            direction = direction | Direction::DOWN;
            break;
        default:
            break;
    }

    switch (dir.y_comp) {
        case 1:
            direction = direction | Direction::RIGHT;
            break;
        case -1:
            direction = direction | Direction::LEFT;
            break;
        default:
            break;
    }

    return direction;
}
