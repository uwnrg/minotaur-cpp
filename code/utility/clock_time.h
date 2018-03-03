#ifndef CLOCK_TIME_H
#define CLOCK_TIME_H

#include <ctime>
#include <string>

class ClockTime {
public:
    static std::string getCurrentTime();

private:
    enum {
        TIME_CHAR_BUFFER = 10
    };

    static time_t m_raw_time;
    static struct tm *m_time_info;
};

#endif // CLOCK_TIME_H
