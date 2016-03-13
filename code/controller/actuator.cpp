#include "actuator.h"
#include "../utility/logger.h"

Actuator::Actuator(const QString& serial_port, const PortSettings& settings, QextSerialPort::QueryMode mode) {
    m_serial_port = new QextSerialPort(serial_port, settings, mode);
    //on successful connection
    if (m_serial_port->lastError() == 0) {
        Logger::Log(serial_port.toStdString() + " successfully opened!", Logger::INFO);
        m_serial_port->flush();
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

int Actuator::SetSerPort(const QString& serial_port) {
	if (m_serial_port->portName() == serial_port) {
		return 0;
	}
	//close the current port
	if (m_serial_port->isOpen()) {
		m_serial_port->flush();
		m_serial_port->close();
	}
	
	m_serial_port->setPortName(serial_port);
	if (m_serial_port->lastError() != 0) {
		Logger::Log(m_serial_port->errorString().toStdString(), Logger::ERROR);
		return -1;
	}
	return 0;
}

int Actuator::ChangeSettings(const PortSettings& settings) {
	m_serial_port->setBaudRate(settings.BaudRate);
	m_serial_port->setDataBits(settings.DataBits);
	m_serial_port->setParity(settings.Parity);
	m_serial_port->setStopBits(settings.StopBits);
	m_serial_port->setFlowControl(settings.FlowControl);
	m_serial_port->setTimeout(settings.Timeout_Millisec);

	if (m_serial_port->lastError() != 0) {
		Logger::Log(m_serial_port->errorString().toStdString(), Logger::ERROR);
		return -1;
	}
	return 0;
}

Actuator::~Actuator() {
    delete m_serial_port;
}

void Actuator::Move(Controller::Dir) {

}

//TODO: Add static method for getting current configuration of a given port