#include "logger.h"


std::string Logger::m_buffer;
LogType Logger::m_type;
QTextBrowser* Logger::m_outfield = NULL;

void Logger::SetStream(QTextBrowser* log_stream) {
    m_outfield = log_stream;
}

//TODO: Overload ostream operator
bool Logger::Log (std::string message, LogType type) {
    switch (type) {
        case INFO:
            Logger::m_buffer = ClockTime::GetCurrentTime() + message;
            break;

        case ERROR:
            //TODO: Set custom color of to the buffer
            break;

        default:
            //TODO: Address  unrecognised type here
            break;
    }

    if (Logger::m_outfield) {
        //log it
        m_outfield->append(QString::fromStdString(m_buffer));
    }
    else {
        std::cout << m_buffer << std::endl;
    }
    return true;
}
