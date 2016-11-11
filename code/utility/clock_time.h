#ifndef CLOCK_TIME_H
#define CLOCK_TIME_H

#include <ctime>
#include <stdio.h>
#include <string>

#define TIME_CHAR_BUFFER 10
class ClockTime {
public:
    static std::string getCurrentTime ();

private:
    static time_t m_rawtime;
    static struct tm* m_timeinfo;
};

#endif // CLOCK_TIME_H
