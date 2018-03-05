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
    /**
     * Default constructor will attempt to auto-dectect and connect
     * using default baud rate of 9600.
     */
    Solenoid();

    explicit Solenoid(
        const QString &serial_port,
        QSerialPort::BaudRate baud_rate = QSerialPort::Baud9600
    );

    ~Solenoid() override;

    void __move_delegate(Vector2i dir, int timer) override;

    /**
     * @return if the QSerialPort is currently managing an active connection
     */
    bool is_connected() const;

    /**
     * @return a reference to the QSerialPort object
     */
    const QSerialPort &serial_port() const;

    Q_SLOT void readSerial();

    /**
     * Attempt to connect to the Arduino. If the provided serial
     * port is empty, the function will try to autodetect a port.
     *
     * @param serial_port the serial port to use, e.g. "/dev/ttyACM0"
     * @param baud_rate   the serial baud rate, usually 9600
     */
    Q_SLOT void attempt_connection(
        const QString &serial_port = "",
        QSerialPort::BaudRate baud_rate = QSerialPort::Baud9600
    );

    /**
     * Attempt to disconnect from the serial port, if the port
     * is currently connected.
     */
    Q_SLOT void disconnect();

    /**
     * Signal emitted when the Arduino has sent a message to Minotaur.
     *
     * @param msg the message written by Arduino
     */
    Q_SIGNAL void serialRead(const std::string &msg);

    /**
     * Convert a move vector into a binary representation understood by the
     * Arduino code.
     *
     * @param dir the vector representing movement direction
     * @return vector encoded as a single byte
     */
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
