#include "logger.h"
#include <ctime>
#include <stdio.h>

Logger::Logger() {
    //m_type = LogType::ERROR;
    m_buffer = "";

    std::cout << Logger::GetCurrentTime() << "Logger initialized" << std::endl;
}

//TODO: Overload ostream operator
bool Logger::Log (std::string message) {
    m_buffer = Logger::GetCurrentTime() + message;

    std::cout << m_buffer << std::endl;

    return true;
}

std::string Logger::GetCurrentTime() {
    time(&m_rawtime);
    m_timeinfo = localtime(&m_rawtime);

    char clock_time [TIME_CHAR_BUFFER];
    sprintf(clock_time, "%.2d:%.2d:%.2d ", m_timeinfo->tm_hour, m_timeinfo->tm_min, m_timeinfo->tm_sec);

    std::string time_in_str(clock_time);

    return time_in_str;
}

Logger::~Logger() {
}
