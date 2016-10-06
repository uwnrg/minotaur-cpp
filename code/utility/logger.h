#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <QTextEdit>
#include <QString>
#include <iostream>

#include "clock_time.h"

class Logger {
public:
    enum LogType {
        INFO,
        ERROR,

        NUM_LOG_TYPES
    };
    static bool log(std::string message, LogType type = INFO);
    static void setStream(QTextEdit* log_stream);

private:
    static std::string m_buffer;
    static QTextEdit* m_outfield;
    inline static std::string getTextColor(LogType type) {
        switch (type) {
            case INFO:
                return "black";
            case ERROR:
                return "red";
            default:
                return "black";
        }
    }
};

#endif // LOGGER_H
