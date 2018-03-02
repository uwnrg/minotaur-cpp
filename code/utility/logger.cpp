#include "logger.h"

std::ostream &operator<<(std::ostream &ss, const QString &qstr) {
    return ss << qstr.toStdString();
}

Logger Logger::s_logger;

void Logger::setStream(QTextEdit *output_field) {
    s_logger.m_log_out.reset(reinterpret_cast<__log_out *>(new __log_text_field(output_field)));
}

void Logger::setStdout() {
    s_logger.m_log_out.reset(reinterpret_cast<__log_out *>(new __log_stdout));
}

bool Logger::clear_log() {
    return s_logger.m_log_out->clear();
}

Logger::Logger() :
    m_log_out(reinterpret_cast<__log_out *>(new __log_stdout)) {}

__log_stream::__log_stream(Logger::LogType log_type) :
    m_log_type(log_type) {}

__log_stream::~__log_stream() {
    m_buffer << std::endl;
    Logger::log(m_buffer.str(), m_log_type);
}

__log_stream::__log_stream(__log_stream &&o) noexcept :
    m_log_type(o.m_log_type) {
    m_buffer = std::move(o.m_buffer);
}

__log_stream log(Logger::LogType log_type) {
    return __log_stream(log_type);
}

bool __log_out::operator<<(const std::string &) {
    // Base class is no-op
    return false;
}

bool __log_stdout::operator<<(const std::string &str) {
    std::cout << str << std::endl;
}

bool __log_text_field::operator<<(const std::string &str) {
    if (m_output_field) {
        m_output_field->append(QString::fromStdString(str));
        return true;
    }
    return false;
}

bool __log_out::clear() {
    // Base class is no-op
    return false;
}

bool __log_stdout::clear() {
    // Cannot clear the console
    return false;
}

bool __log_text_field::clear() {
    if (m_output_field) {
        m_output_field->clear();
        return true;
    }
    return false;
}

__log_text_field::__log_text_field(QTextEdit *output_field) :
    m_output_field(output_field) {}
