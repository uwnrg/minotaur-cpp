#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <iostream>

#define TIME_CHAR_BUFFER 10

enum LogType {
    ERROR,
    WARNING,
    INFO,

    NUM_LOG_TYPES
};

class Logger {
public:
    Logger();
    bool Log(std::string message);
    void SetType(LogType type) {m_type = type; }
    ~Logger();

private:
    std::string m_buffer;
    LogType m_type;
    struct tm *m_timeinfo;
    time_t m_rawtime;
    std::string GetCurrentTime();
};

#endif // LOGGER_H
