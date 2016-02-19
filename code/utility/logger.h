#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <iostream>
#include <ctime>
#include <stdio.h>

#define TIME_CHAR_BUFFER 10

enum LogType {
    ERROR,
    WARNING,
    INFO,

    NUM_LOG_TYPES
};

class Logger {
public:
    static bool Log(std::string message);
    static void SetType(LogType type) {m_type = type; }

private:
    static std::string m_buffer;
    static LogType m_type;
    static struct tm *m_timeinfo;
    static time_t m_rawtime;
    static std::string GetCurrentTime();
};

#endif // LOGGER_H
