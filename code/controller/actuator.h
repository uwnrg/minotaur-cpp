#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "controller.h"
#include "qextserialport.h"

#define BYTE_RANGE	256
#define DATA_SIZE	4

// For Zaber, settings must be: 9600 baud, no hand shaking, 8 data bits, no parity, one stop bit.
const PortSettings DEFAULT_SETTINGS = { BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_XONXOFF, 10 };


class QextSerialPort;

class Actuator : public Controller
{
public:
    Actuator(const QString& serial_port, const PortSettings& settings = DEFAULT_SETTINGS, QextSerialPort::QueryMode mode = QextSerialPort::EventDriven);

    int SetSerPort(const QString& serial_port);
	int ChangeSettings(const PortSettings& settings);
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
