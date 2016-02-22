#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <QTextEdit>
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
    static void SetStream(QTextEdit* log_stream);

private:
    static std::string m_buffer;
    static LogType m_type;
    static QTextEdit* m_outfield;
};

#endif // LOGGER_H
