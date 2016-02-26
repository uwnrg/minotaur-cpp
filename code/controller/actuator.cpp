#include "actuator.h"

Actuator::Actuator(const QString& serial_port, QextSerialPort::QueryMode mode, QObject * parent) {
    m_serial_port = new QextSerialPort();
    /*//on successful connection
    if (m_serial_port != NULL) {
        //log
        //flush it
        m_serial_port->setBaudRate(BaudRateType(BAUD_RATE));
        //log it here
    }
    else {
        //log serial_port could not be opened
    }*/
}

Actuator::~Actuator() {
    delete m_serial_port;
}

void Actuator::Move(Controller::Dir) {}
