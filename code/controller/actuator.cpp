#include "actuator.h"
#include "../utility/logger.h"

Actuator::Actuator(const QString& serial_port, QextSerialPort::QueryMode mode, QObject * parent) {
    m_serial_port = new QextSerialPort(serial_port, mode, parent);
    //on successful connection
    if (m_serial_port->lastError() == 0) {
        Logger::Log(serial_port.toStdString() + " successfully opened!", Logger::INFO);
        m_serial_port->flush();
        m_serial_port->setBaudRate(BaudRateType(BAUD_RATE));
    }
    else {
        Logger::Log("ERROR: " + serial_port.toStdString() + " could not be opened! " + m_serial_port->errorString().toStdString(), Logger::ERROR);
    }
}

Actuator::~Actuator() {
    delete m_serial_port;
}

void Actuator::Move(Controller::Dir) {}
