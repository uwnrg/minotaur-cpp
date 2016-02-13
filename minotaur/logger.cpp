#include "logger.h"
#include <ctime>

Logger::Logger() {
    //m_type = LogType::ERROR;
    m_buffer = "";

    std::cout << Logger::GetCurrentTime() << "Logger initialized" << std::endl;
}

//TODO: Overload ostream operator
bool Logger::Log (std::string message) {
    m_buffer = Logger::GetCurrentTime() + message;

    std::cout << m_buffer << std::endl;
}

static std::string Logger::GetCurrentTime() {
    time(&m_rawtime);
    timeinfo = localtime(&rawtime);

    std::string clock_time;
    sprintf(clock_time, "%.2d:%.2d:%.2d ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    return clock_time;
}

Logger::~Logger() {
}
