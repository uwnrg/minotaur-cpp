#include "clock_time.h"

// initialize static data fields
time_t ClockTime::m_raw_time;
struct tm *ClockTime::m_time_info;

std::string ClockTime::getCurrentTime() {
    time(&ClockTime::m_raw_time);
    ClockTime::m_time_info = localtime(&ClockTime::m_raw_time);

    char clock_time[TIME_CHAR_BUFFER];
    sprintf(
        clock_time,
        "%.2d:%.2d:%.2d ",
        ClockTime::m_time_info->tm_hour,
        ClockTime::m_time_info->tm_min,
        ClockTime::m_time_info->tm_sec
    );

    std::string time_str(clock_time);

    return time_str;
}
