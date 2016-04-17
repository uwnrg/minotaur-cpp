#include <thread>
#include <chrono>

#include "actuator.h"
#include "../utility/logger.h"

Actuator::Actuator(const QString& serial_port, const PortSettings& settings, QextSerialPort::QueryMode mode) {
    m_serial_port = new QextSerialPort(serial_port, settings, mode);
	m_invert_x = 1;
	m_invert_y = 1;
    //on successful connection
    if (m_serial_port->lastError() == 0) {
        Logger::log(serial_port.toStdString() + " successfully opened!", Logger::INFO);
        m_serial_port->flush();
		// Send a renumbering request for the devices
		setDeviceNumber();
    }
    else {
        Logger::log("ERROR: " + serial_port.toStdString() + " could not be opened! " + m_serial_port->errorString().toStdString(), Logger::ERROR);
    }
}

char* const Actuator::convertDataToBytes(long int data) {
	if (data < 0) {
		data = intPow(BYTE_RANGE, 4) + data;
	}

	char result[DATA_SIZE];
	
	for (int i = DATA_SIZE - 1; i >= 0; ++i) {
		int temp = intPow(BYTE_RANGE, i);
		result[i] = data / temp;
		data = data - temp;
	}

	return result;
}

void Actuator::setDeviceNumber() {
	char instr[] = { 0, 2, 0, 0, 0, 0 };
	
	m_serial_port->write(instr);

	m_x_device = 0;
	m_y_device = 1;
}

int const Actuator::intPow(int x, int p) {
	if (p == 0) return 1;
	if (p == 1) return x;

	int tmp = intPow(x, p / 2);
	if (p % 2 == 0) return tmp * tmp;
	else return x * tmp * tmp;
}

int Actuator::setSerPort(const QString& serial_port) {
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
		Logger::log(m_serial_port->errorString().toStdString(), Logger::ERROR);
		return -1;
	}
	return 0;
}

int Actuator::changeSettings(const PortSettings& settings) {
	m_serial_port->setBaudRate(settings.BaudRate);
	m_serial_port->setDataBits(settings.DataBits);
	m_serial_port->setParity(settings.Parity);
	m_serial_port->setStopBits(settings.StopBits);
	m_serial_port->setFlowControl(settings.FlowControl);
	m_serial_port->setTimeout(settings.Timeout_Millisec);

	if (m_serial_port->lastError() != 0) {
		Logger::log(m_serial_port->errorString().toStdString(), Logger::ERROR);
		return -1;
	}
	return 0;
}

void Actuator::invertDevices() {
	unsigned char temp = m_x_device;
	m_x_device = m_y_device;
	m_y_device = temp;
}

Actuator::~Actuator() {
    delete m_serial_port;
}

void Actuator::move(Vector2i dir, int time) {
	std::promise<bool> error_x, error_y;
	auto ferror_x = error_x.get_future();
	auto ferror_y = error_y.get_future();

	std::thread x_thread(Actuator::moveActuator, m_serial_port, m_x_device, dir.x, time, std::ref(error_x));
	std::thread y_thread(Actuator::moveActuator, m_serial_port, m_y_device, dir.y, time, std::ref(error_y));

	// Wait for the threads to finish before exiting the function
	x_thread.join();
	y_thread.join();

	// Check if any errors were generated using the flags
	bool success;
	try {
		success = (ferror_x.get() || ferror_y.get());
	}
	catch (std::exception& e) {
		Logger::log(e.what(), Logger::ERROR);
		success = false;
	}

	if (success) {
		Logger::log("Moved { " + std::to_string(dir.x) + ", " + std::to_string(dir.y) + " } in " + std::to_string(time) + " milliseconds.", Logger::INFO);
	}
}

void Actuator::moveActuator(QextSerialPort* ser_port, const unsigned char device, const int value, const int time, std::promise<bool>& success) {
	try {
		std::chrono::milliseconds sleep_step = std::chrono::milliseconds(time / value);
		char* instr = new char(CMD_SIZE + DATA_SIZE);

		// Setup device and command numbers
		instr[0] = device;
		instr[1] = ZaberCmd::REL_MOVE;

		char* data = convertDataToBytes(STEP_FACTOR);

		for (int i = 0; i < DATA_SIZE; i++) {
			instr[i + 2] = data[i];
		}

		// This is yet to be tested, sorry I don't have Zaber actuators at home :(
		for (int i = value; i > 0; i--) {
			if (ser_port->isOpen()) {
				ser_port->write(instr, CMD_SIZE + DATA_SIZE);
			}
			else {
				Logger::log("ERROR: Failed to write to serial port " + (ser_port->portName()).toStdString() + "because it's not open.", Logger::ERROR);
				success.set_value(false);
				return;
			}

			std::this_thread::sleep_for(sleep_step);
		}

		success.set_value(true);
	}
	catch (...) {
		success.set_exception(std::current_exception());
	}
}

//TODO: Add static method for getting current configuration of a given port