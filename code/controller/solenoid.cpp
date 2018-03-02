#include "solenoid.h"

#include <QSerialPortInfo>

Solenoid::Solenoid(const QString &serial_port)
    : Controller(1, 1),
      m_serial(serial_port) {
    if (serial_port.isEmpty()) {
        // Autodetect Arduino port
        QSerialPortInfo port_to_use;
        auto ports = QSerialPortInfo::availablePorts();
        for (auto port : ports) {
            if (!port.isBusy() && (port.description().contains("Arduino") || port.manufacturer().contains("Arduino"))) {
                port_to_use = port;
                break;
            }
        }
        Logger::log("Autodetected Arduino port: " + port_to_use.portName().toStdString());
        m_serial.setPort(port_to_use);
    }
    m_serial.setBaudRate(QSerialPort::Baud9600);
    m_serial.setDataBits(QSerialPort::Data8);
    m_serial.setParity(QSerialPort::NoParity);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setFlowControl(QSerialPort::NoFlowControl);
    if (!m_serial.open(QIODevice::ReadWrite)) {
        Logger::log("Failed to open serial port: " + m_serial.portName().toStdString(), Logger::FATAL);
    } else {
        Logger::log("Opened serial port: " + m_serial.portName().toStdString());
        connect(&m_serial, &QSerialPort::readyRead, this, &Solenoid::readSerial);
    }
}

Solenoid::~Solenoid() {
    m_serial.close();
}

void Solenoid::readSerial() {
    QByteArray data = m_serial.readAll();
    std::string msg = data.toStdString();
    Q_EMIT serialRead(msg);
}

void Solenoid::move(Vector2i dir, int) {
#ifndef NDEBUG
    Logger::log("Moving Solenoid controller", Logger::DEBUG);
    log(Logger::DEBUG) << "Attempting to move " << dir;
#endif
    char binary = vectorToBinary(dir);
    m_serial.write(&binary, 1);
    if (!m_serial.waitForBytesWritten(100)) {
        Logger::log("Failed to execute movement", Logger::FATAL);
    } else {
        Logger::log("Movement send");
    }
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
