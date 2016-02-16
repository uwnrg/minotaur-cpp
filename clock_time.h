#ifndef CLOCK_TIME_H
#define CLOCK_TIME_H

#include <ctime>

class ClockTime {
public:
    inline ClockTime() { }
    inline static void GetStringTime () { }
    inline virtual ~ClockTime() { }

private:
    static time_t* m_rawtime;
    static struct tm* m_timeinfo;
};

#endif // CLOCK_TIME_H
