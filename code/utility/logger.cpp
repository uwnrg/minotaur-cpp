#include "logger.h"


std::string Logger::m_buffer;
QTextEdit* Logger::m_outfield = NULL;

void Logger::SetStream(QTextEdit* log_stream) {
    m_outfield = log_stream;
    Logger::Log("Logger's stream has been set.");
}

//TODO: Overload ostream operator
bool Logger::Log (std::string message, LogType type) {
    std::string color = Logger::GetTextColor(type);
    Logger::m_buffer = "<font color=\"" + color + "\">" + ClockTime::GetCurrentTime() + message + "</font>";

    if (Logger::m_outfield) {
        //log it
        m_outfield->append(QString::fromStdString(m_buffer));
    }
    else {
        std::cout << m_buffer << std::endl;
    }
    return true;
}
