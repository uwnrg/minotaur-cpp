#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "controller.h"
#include "qextserialport.h"


#define BAUD_RATE 9600


class QextSerialPort;

class Actuator : public Controller
{
public:
    Actuator(const QString& serial_port, QextSerialPort::QueryMode mode = QextSerialPort::EventDriven, QObject * parent = 0);

    void SetComPort();
    void Move(Controller::Dir);
    ~Actuator();

private:
    QextSerialPort* m_serial_port;

    //helper functions

};

#endif // ACTUATOR_H
