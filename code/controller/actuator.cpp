#include <thread>
#include <chrono>
#include <future>

#include "actuator.h"
#include "../utility/logger.h"

Actuator::Actuator(const QString& serial_port, 
    const PortSettings& settings, QextSerialPort::QueryMode mode) :
    Controller(1, 1), m_serial_port(new QextSerialPort(serial_port, settings, mode)) {

    //on successful connection
    if (m_serial_port->lastError() == 0) {
        Logger::log(serial_port.toStdString() + " successfully opened!", Logger::INFO);
        m_serial_port->flush();
        // Send a renumbering request for the devices
        resetDeviceNumber();
    }
    else {
        Logger::log("ERROR: " + serial_port.toStdString() + " could not be opened! " + m_serial_port->errorString().toStdString(), Logger::ERROR);
    }
}

// Copy constructor
Actuator::Actuator(const Actuator& other) :
    Controller(other.m_invert_x, other.m_invert_y),
    m_x_device(other.m_x_device), m_y_device(other.m_y_device),
    m_serial_port(other.m_serial_port) {}

char* const Actuator::convertDataToBytes(long int data) {
    if (data < 0) {
        data = intPow(BYTE_RANGE, 4) + data;
    }

    char result[DATA_SIZE];

    for (int i = DATA_SIZE - 1; i >= 0; --i) {
        int temp = intPow(BYTE_RANGE, i);
        result[i] = data / temp;
        data = data - temp * result[i];
    }

    return result;
}

void Actuator::resetDeviceNumber() {
    char instr[] = { 0, ZaberCmd::RENUMBER, 0, 0, 0, 0 };

    m_serial_port->write(instr);

    m_x_device = 0;
    m_y_device = 1;
}

int const Actuator::intPow(int x, int p) {
    if (p == 0) return 1;
    if (p == 1) return x;

    int tmp = intPow(x, p / 2);
    if (p % 2 == 0) return tmp * tmp;
    else return x * tmp * tmp;
}

int Actuator::setSerPort(const QString& serial_port) {
    if (m_serial_port->portName() == serial_port) {
        return 0;
    }
    //close the current port
    if (m_serial_port->isOpen()) {
        m_serial_port->flush();
        m_serial_port->close();
    }

    m_serial_port->setPortName(serial_port);
    if (m_serial_port->lastError() != 0) {
        Logger::log(m_serial_port->errorString().toStdString(), Logger::ERROR);
        return -1;
    }
    return 0;
}

int Actuator::changeSettings(const PortSettings& settings) {
    m_serial_port->setBaudRate(settings.BaudRate);
    m_serial_port->setDataBits(settings.DataBits);
    m_serial_port->setParity(settings.Parity);
    m_serial_port->setStopBits(settings.StopBits);
    m_serial_port->setFlowControl(settings.FlowControl);
    m_serial_port->setTimeout(settings.Timeout_Millisec);

    if (m_serial_port->lastError() != 0) {
        Logger::log(m_serial_port->errorString().toStdString(), Logger::ERROR);
        return -1;
    }
    return 0;
}

void Actuator::switchDevices() {
    unsigned char temp = m_x_device;
    m_x_device = m_y_device;
    m_y_device = temp;
}

Actuator::~Actuator() {
    delete m_serial_port;
}

void Actuator::move(Dir dir, int t) {
    move(Controller::toVec2(dir), t);
}

void Actuator::move(Vector2i dir, int t) {
    bool success = true;
    try {
        std::future<void> x_thread(std::async(&Actuator::moveActuator, this, m_x_device, dir.x_comp, t));
        std::future<void> y_thread(std::async(&Actuator::moveActuator, this, m_y_device, dir.y_comp, t));

        x_thread.get();
        y_thread.get();
    }
    catch (std::exception& e) {
        Logger::log(e.what(), Logger::ERROR);
        success = false;
    }

    if (success) {
        Logger::log("Moved { " + std::to_string(dir.x_comp) + ", " + std::to_string(dir.y_comp) + " } in " + std::to_string(t) + " milliseconds.", Logger::INFO);
        Logger::log("Moved { " + std::to_string(dir.x_comp) + ", " + std::to_string(dir.y_comp) + " } in " + std::to_string(t) + " milliseconds.", Logger::INFO);
    }
    else {
        Logger::log("The movement { " + std::to_string(dir.x_comp) + ", " + std::to_string(dir.y_comp) + " } could not be completed.", Logger::ERROR);
    }
}

void Actuator::moveActuator(const unsigned char device, const int value, const int t) {
    try {
        std::chrono::milliseconds sleep_step = std::chrono::milliseconds(0);
        if (value != 0) {
            sleep_step = std::chrono::milliseconds(t / value); // still need to test negative steps
        }
        char* instr = new char(CMD_SIZE + DATA_SIZE);

        // Setup device and command numbers
        instr[0] = device;
        instr[1] = ZaberCmd::REL_MOVE;

        char* data = convertDataToBytes(STEP_FACTOR);

        for (int i = 0; i < DATA_SIZE; i++) {
            instr[i + 2] = data[i];
        }

        // TODO: This is yet to be tested, sorry I don't have Zaber actuators at home :(
        for (int i = value; i > 0; i--) {
            if (m_serial_port->isOpen()) {
                m_serial_port->write(instr, CMD_SIZE + DATA_SIZE);
            }
            else {
                Logger::log("ERROR: Failed to write to serial port " + (m_serial_port->portName()).toStdString() + " because it's not open.", Logger::ERROR);
                throw "Action could not be completed"; // TODO: Might want to figure out a better way than throwing exceptions, revisit after adding concurrency
            }

            std::this_thread::sleep_for(sleep_step);
        }
    }
    catch (...) {
        throw std::current_exception();
    }
}

//TODO: Add static method for getting current configuration of a given port
