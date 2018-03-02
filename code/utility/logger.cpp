#include "logger.h"

void Logger::setStream(QTextEdit *output_field) {
    s_logger.m_output_field = output_field;
}

bool Logger::log(const std::string &message, LogType type) {
    return s_logger.log_message<std::string>(message, type);
}

bool Logger::qlog(const QString &message, LogType type) {
    return s_logger.log_message<QString>(message, type);
}

Logger::Logger() :
    m_output_field(nullptr) {}

__log_stream::__log_stream(Logger::LogType log_type) :
    m_log_type(log_type) {}

__log_stream::~__log_stream() {
    m_buffer << std::endl;
    Logger::log(m_buffer.str(), m_log_type);
}

__log_stream::__log_stream(__log_stream &&o) noexcept :
    m_log_type(o) {
    m_buffer = std::move(o.m_buffer);
}

__log_stream log(Logger::LogType log_type) {
    return __log_stream(log_type);
}
