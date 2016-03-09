#include "actuator.h"
#include "../utility/logger.h"

Actuator::Actuator(const QString& serial_port, QextSerialPort::QueryMode mode, QObject * parent) {
    m_serial_port = new QextSerialPort(serial_port, mode, parent);
    //on successful connection
    if (m_serial_port->lastError() == 0) {
        Logger::Log(serial_port.toStdString() + " successfully opened!", Logger::INFO);
        // For Zaber, settings must be: 9600 baud, no hand shaking, 8 data bits, no parity, one stop bit.
        m_serial_port->flush();
        m_serial_port->setBaudRate(BaudRateType(BAUD_RATE));
        m_serial_port->setFlowControl(FLOW_XONXOFF);
        m_serial_port->setDataBits(DATA_8);
        m_serial_port->setParity(PAR_NONE);
        m_serial_port->setStopBits(STOP_1);
    }
    else {
        Logger::Log("ERROR: " + serial_port.toStdString() + " could not be opened! " + m_serial_port->errorString().toStdString(), Logger::ERROR);
    }
}

char* const Actuator::ConvertDataToBytes(long int data) {
	if (data < 0) {
		data = IntPow(BYTE_RANGE, 4) + data;
	}

	char result[DATA_SIZE];
	
	for (int i = DATA_SIZE - 1; i >= 0; ++i) {
		int temp = IntPow(BYTE_RANGE, i);
		result[i] = data / temp;
		data = data - temp;
	}

	return result;
}

void Actuator::SetDeviceNumber() {
	char instr[6];
}

int const Actuator::IntPow(int x, int p) {
	if (p == 0) return 1;
	if (p == 1) return x;

	int tmp = IntPow(x, p / 2);
	if (p % 2 == 0) return tmp * tmp;
	else return x * tmp * tmp;
}

Actuator::~Actuator() {
    delete m_serial_port;
}

void Actuator::Move(Controller::Dir) {

}

//TODO: Add static method for getting current configuration of a given port