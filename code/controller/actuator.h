#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "controller.h"
#include "qextserialport.h"


#define BAUD_RATE 9600
#define INSTR_SIZE 6
#define DATA_SIZE 4
#define BYTE_RANGE 256


class QextSerialPort;

class Actuator : public Controller
{
public:
    Actuator(const QString& serial_port, QextSerialPort::QueryMode mode = QextSerialPort::EventDriven, QObject * parent = 0);

    void SetSerPort(const QString& serial_port);
    QString* GetAvailablePorts() const;
    void Move(Controller::Dir);
    ~Actuator();

private:
    enum CMD {
        HOME = 1,
        RENUMBER = 2,
        ABS_MOVE = 20,
        REL_MOVE = 21,
        CONST_SPEED_MOVE = 22,
        STOP_MOVE = 23,
        SET_CURRENT_POS = 45,
        FIRM_VER = 51
    };
    QextSerialPort* m_serial_port;
    unsigned char m_device_number, m_data_buffer;

    //helper functions
	char* const ConvertDataToBytes(long int data);
	void SetDeviceNumber();
	int const IntPow(int x, int p);
};

#endif // ACTUATOR_H
