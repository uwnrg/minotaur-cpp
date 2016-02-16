#include "clock_time.h"

// initialize static data fields
time_t* ClockTime::m_rawtime = NULL;
struct tm* ClockTime::m_timeinfo = NULL;
/*
static std::string ClockTime::GetStringTime() {
    //time(ClockTime::m_rawTime);
    //ClockTime::m_timeinfo = localtime(ClockTime::m_rawtime);
    return "";
}
*/
