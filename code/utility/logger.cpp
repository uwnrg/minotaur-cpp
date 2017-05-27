#include "logger.h"

#ifndef NDEBUG
    bool Logger::m_debug = true;
#else
    bool Logger::m_debug = false;
#endif

std::string Logger::m_buffer;
QTextEdit* Logger::m_outfield = NULL;

void Logger::setStream(QTextEdit* log_stream) {
    Logger::m_outfield = log_stream;
    Logger::log("Logger's stream has been set.");
}

//TODO: Overload ostream operator
bool Logger::log (std::string message, LogType type) {
    if (type == LogType::DEBUG && !Logger::m_debug) return false;
    std::string color = Logger::getTextColor(type);
    Logger::m_buffer = "<font color=\"" + color + "\">" + ClockTime::getCurrentTime() + message + "</font>";

    if (Logger::m_outfield != nullptr) {
        //log it
        Logger::m_outfield->append(QString::fromStdString(m_buffer));
    }
    else {
        std::cout << m_buffer << std::endl;
    }
    return true;
}
