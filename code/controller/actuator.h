#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "controller.h"
#include "qextserialport.h"

#define BYTE_RANGE	256
#define DATA_SIZE	4
#define CMD_SIZE	2
#define STEP_FACTOR 5
#define STEP_TIME	10

// For Zaber, settings must be: 9600 baud, no hand shaking, 8 data bits, no parity, one stop bit.
const PortSettings DEFAULT_SETTINGS = { BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_XONXOFF, 10 };


class QextSerialPort;


enum ZaberCmd {
	HOME = 1,
	RENUMBER = 2,
	ABS_MOVE = 20,
	REL_MOVE = 21,
	CONST_SPEED_MOVE = 22,
	STOP_MOVE = 23,
	SET_CURRENT_POS = 45,
	FIRM_VER = 51
};

class Actuator : public Controller
{
public:
    Actuator(const QString& serial_port = "", const PortSettings& settings = DEFAULT_SETTINGS, QextSerialPort::QueryMode mode = QextSerialPort::EventDriven);
	Actuator(const Actuator&);
    int setSerPort(const QString& serial_port);
	int changeSettings(const PortSettings& settings);
	void invertDevices();
	void move(Dir dir, int time = STEP_TIME);
	void move(Vector2i dir, int time = STEP_TIME);
    ~Actuator();

private:
    QextSerialPort* m_serial_port;
    unsigned char m_x_device, m_y_device;

    //helper functions
	static char* const convertDataToBytes(long int data);
	void setDeviceNumber();
	static int const intPow(int x, int p);
	void moveActuator(unsigned char device, const int value, const int time);
};

#endif // ACTUATOR_H
