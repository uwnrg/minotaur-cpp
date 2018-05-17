#ifndef SOLENOID_H
#define SOLENOID_H

#include "controller.h"

#include <fstream>
#include <queue>

#include <QObject>
#include <QSerialPort>

enum SerialStatus {
    DISCONNECTED,
    CONNECTING,
    CONNECTED
};

/**
 * This controller manages a connection to the Arduino and handles
 * sending and receiving messages.
 */
class Solenoid : public Controller {
Q_OBJECT

public:
    /**
     * Default constructor will attempt to auto-detect and connect
     * using default baud rate of 9600.
     */
    Solenoid();

    explicit Solenoid(
        const QString &serial_port,
        QSerialPort::BaudRate baud_rate = QSerialPort::Baud9600
    );

    ~Solenoid() override;

    vector2i to_vector2i(Dir dir) override;

    void __move_delegate(vector2i dir, int timer) override;

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
    Q_SLOT void attempt_disconnect();

    Q_SLOT void change_up_power(int value);

    Q_SLOT void change_down_power(int value);

    Q_SLOT void change_left_power(int value);

    Q_SLOT void change_right_power(int value);

    Q_SLOT void change_delay(int value);

    /**
     * Signal emitted when the Arduino has sent a message to Minotaur.
     *
     * @param msg the message written by Arduino
     */
    Q_SIGNAL void serialRead(const std::string &msg);

    /**
     * Signal emitted when the status of the serial port has changed.
     * Emits CONNECTING, DISCONNECTED, and CONNECTED.
     *
     * @param status the new port status
     */
    Q_SIGNAL void serial_status(SerialStatus status);

    /**
     * Convert a move vector into a binary representation understood by the
     * Arduino code.
     *
     * Two 16-bit words represent the signed (x, y) values of the vector
     * followed by the movement time in milliseconds [0-65535].
     *
     * @param dir the vector representing movement direction
     * @return vector encoded as a single byte
     */
    static QByteArray encode_message(vector2i dir, int time);

private:
    enum Direction {
        LEFT = 97,
        RIGHT = 100,
        DOWN = 115,
        UP = 119
    };

    void change_power(int value, enum Direction direction);

    QSerialPort m_serial;
};

#endif // SOLENOID_H
