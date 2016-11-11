#include "clock_time.h"

// initialize static data fields
time_t ClockTime::m_rawtime;
struct tm* ClockTime::m_timeinfo;

std::string ClockTime::getCurrentTime() {
    time(&ClockTime::m_rawtime);
    ClockTime::m_timeinfo = localtime(&ClockTime::m_rawtime);

    char clock_time [TIME_CHAR_BUFFER];
    sprintf(clock_time, "%.2d:%.2d:%.2d ", 
		ClockTime::m_timeinfo->tm_hour, 
		ClockTime::m_timeinfo->tm_min, 
		ClockTime::m_timeinfo->tm_sec);

    std::string time_in_str(clock_time);

    return time_in_str;
}
