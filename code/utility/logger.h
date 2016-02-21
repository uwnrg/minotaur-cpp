#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <QTextBrowser>
#include <QString>
#include <iostream>

#include "clock_time.h"

enum LogType {
    INFO,
    ERROR,

    NUM_LOG_TYPES
};

class Logger {
public:
    static bool Log(std::string message, LogType type = INFO);
    static void SetType(LogType type) {m_type = type; }
    static void SetStream(QTextBrowser* log_stream);

private:
    static std::string m_buffer;
    static LogType m_type;
    static QTextBrowser* m_outfield;
};

#endif // LOGGER_H
