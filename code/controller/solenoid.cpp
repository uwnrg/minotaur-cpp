#include "solenoid.h"
#include <ostream>

Solenoid::Solenoid(const QString& serial_port): Controller(1, 1) {
    // TODO: change actuator setup to be used for general serial set up
    if ( serial_port.isEmpty() ) {
        // TODO: provide error if this does not succeed
        stream.open("/dev/ttyACM0");        // manually changed to match port indicated in arduino IDE
    } else {
        stream.open(serial_port.toStdString());
    }
}

Solenoid::~Solenoid() {
    stream.close();
}

void Solenoid::move(Vector2i dir, int timer) {
    // TODO: user timer for movement control
    bool success = true;
#ifndef NDEBUG
    Logger::log("Attempting move (" + std::to_string(dir.x_comp) + ", " + std::to_string(dir.y_comp) + ")",
                Logger::DEBUG);
#endif
    try {
        stream << vectorToBinary(dir);
        stream.flush();
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

int Solenoid::vectorToBinary(Vector2i dir) {
    int direction = 0b0;

    switch(dir.x_comp) {
        case 1:
            direction = direction | Direction::UP;
            break;
        case -1:
            direction = direction | Direction::DOWN;
            break;
        case 0:
            // DO NOTHING
            break;
        default:
            Logger::log("Invalid direction x component", Logger::FATAL);
    }

    switch(dir.y_comp) {
        case 1:
            direction = direction | Direction::RIGHT;
            break;
        case -1:
            direction = direction | Direction::LEFT;
            break;
        case 0:
            // DO NOTHING
            break;
        default:
            Logger::log("Invalid direction y component", Logger::FATAL);
    }

    return direction;
}
