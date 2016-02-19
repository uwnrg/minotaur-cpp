#include "logger.h"
#include <ctime>
#include <stdio.h>


std::string Logger::m_buffer;
LogType Logger::m_type;
struct tm* Logger::m_timeinfo;
time_t Logger::m_rawtime;

//TODO: Overload ostream operator
bool Logger::Log (std::string message) {
    Logger::m_buffer = Logger::GetCurrentTime() + message;

    std::cout << m_buffer << std::endl;

    return true;
}

std::string Logger::GetCurrentTime() {
    time(&Logger::m_rawtime);
    Logger::m_timeinfo = localtime(&Logger::m_rawtime);

    char clock_time [TIME_CHAR_BUFFER];
    sprintf(clock_time, "%.2d:%.2d:%.2d ", Logger::m_timeinfo->tm_hour, Logger::m_timeinfo->tm_min, Logger::m_timeinfo->tm_sec);

    std::string time_in_str(clock_time);

    return time_in_str;
}
