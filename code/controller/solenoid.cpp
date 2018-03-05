#include "solenoid.h"

#include <QSerialPortInfo>

Solenoid::Solenoid()
    : Controller(true, true) {
    // For convenience, attempt auto-detect connection at launch
    attempt_connection();
}

Solenoid::Solenoid(
    const QString &serial_port,
    QSerialPort::BaudRate baud_rate
)
    : Controller(true, true) {
    attempt_connection(serial_port, baud_rate);
}

Solenoid::~Solenoid() {
    m_serial.close();
}

void Solenoid::attempt_connection(
    const QString &serial_port,
    QSerialPort::BaudRate baud_rate
) {
    if (serial_port.isEmpty()) {
        // Auto-detect Arduino port
        QSerialPortInfo port_to_use;
        bool found_port = false;
        auto ports = QSerialPortInfo::availablePorts();
        for (auto &port : ports) {
            if (
                !port.isBusy() && (port.description().contains("Arduino") ||
                                   port.manufacturer().contains("Arduino"))
            ) {
                port_to_use = port;
                found_port = true;
                break;
            }
        }
        if (!found_port) {
            fatal() << "No Arduino port specified, failed to autodetect port";
            return;
        }
        log() << "Auto-detected Arduino port: " << port_to_use.portName();
        // Use our auto-detected port info
        m_serial.setPort(port_to_use);
    } else {
        // Set the provided port name
        m_serial.setPortName(serial_port);
    }
    m_serial.setBaudRate(baud_rate);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
    if (!m_serial.open(QIODevice::ReadWrite)) {
        fatal() << "Failed to open serial port: " << m_serial.portName();
    } else {
        log() << "Opened serial port: " << m_serial.portName();
        connect(&m_serial, &QSerialPort::readyRead, this, &Solenoid::readSerial);
    }
}

void Solenoid::disconnect() {
    if (m_serial.isOpen()) {
        log() << "Disconnecting serial port";
        m_serial.close();
    }
}

void Solenoid::readSerial() {
    QByteArray data = m_serial.readAll();
    std::string msg = data.toStdString();
    Q_EMIT serialRead(msg);
}

void Solenoid::__move_delegate(Vector2i dir, int) {
#ifndef NDEBUG
    debug() << "Moving Solenoid controller";
    debug() << "Attempting to move " << dir;
#endif
    if (!m_serial.isOpen()) {
        fatal() << "Failed to execute movement: serial port not open";
        return;
    }
    char binary = vectorToBinary(dir);
    m_serial.write(&binary, 1);
    if (!m_serial.waitForBytesWritten(200)) {
        fatal() << "Failed to execute movement: write timed out";
    } else {
        log() << "Movement sent";
    }
}

bool Solenoid::is_connected() const {
    return m_serial.isOpen();
}

const QSerialPort &Solenoid::serial_port() const {
    return m_serial;
}

char Solenoid::vectorToBinary(Vector2i dir) {
    int direction = 0b0;
    switch (dir.y()) {
        case 1:
            direction = direction | Direction::UP;
            break;
        case -1:
            direction = direction | Direction::DOWN;
            break;
        default:
            break;
    }
    switch (dir.x()) {
        case 1:
            direction = direction | Direction::RIGHT;
            break;
        case -1:
            direction = direction | Direction::LEFT;
            break;
        default:
            break;
    }
    return static_cast<char>(direction);
}
