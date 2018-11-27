#include "logger.h"
#include <QString>
#include <QTextEdit>
#include <fstream>
#include <string.h>

std::ostream &operator<<(std::ostream &ss, const QString &qstr) {
    return ss << qstr.toStdString();
}

void Logger::write_to_file(const std::string &message){
    std::ofstream myfile("logs_to_file.txt", std::fstream::app);
    if (myfile.is_open()) {
        myfile << ClockTime::getCurrentTime() << message << "\n";
        myfile.close();
    }
}

bool Logger::log(const std::string &message, LogType type) {
    return s_logger.log_message(message, type);
}

bool Logger::log(const QString &message, LogType type) {
    return s_logger.log_message(message.toStdString(), type);
}

Logger Logger::s_logger;

bool Logger::log_message(const std::string &message, LogType type) {
    std::stringstream ss;
    ss << "<font color=\"";
    switch (type) {
        case FATAL:
            ss << "red";
            break;
        case DEBUG:
            ss << "blue";
            break;
        default: // INFO
            ss << "black";
            break;
    }
    ss << "\">"
       << ClockTime::getCurrentTime()
       << message
       << "</font>";
    write_to_file(message);
    return *m_log_out << ss.str();
}

void Logger::setStream(QTextEdit *output_field) {
    s_logger.m_log_out.reset(reinterpret_cast<log_out *>(new log_text_field(output_field)));
}

void Logger::setStdout() {
    s_logger.m_log_out.reset(reinterpret_cast<log_out *>(new log_stdout));
}

bool Logger::clear_log() {
    return s_logger.m_log_out->clear();
}

Logger::Logger() :
    m_log_out(reinterpret_cast<log_out *>(new log_stdout)) {}

log_stream::log_stream(Logger::LogType log_type) :
    m_log_type(log_type) {}

log_stream::~log_stream() {
    m_buffer << std::endl;
    Logger::log(m_buffer.str(), m_log_type);
}

log_stream::log_stream(log_stream &&o) noexcept :
    m_log_type(o.m_log_type) {
    m_buffer = std::move(o.m_buffer);
}

log_stream log(Logger::LogType log_type) {
    return log_stream(log_type);
}

log_stream debug() {
    return log(Logger::DEBUG);
}

log_stream fatal() {
    return log(Logger::FATAL);
}

bool log_out::operator<<(const std::string &) {
    // Base class is no-op
    return false;
}

bool log_stdout::operator<<(const std::string &str) {
    std::cout << str << std::endl;
    return true;
}

bool log_text_field::operator<<(const std::string &str) {
    if (m_output_field) {
        m_output_field->append(QString::fromStdString(str));
        return true;
    }
    return false;
}

bool log_out::clear() {
    // Base class is no-op
    return false;
}

bool log_stdout::clear() {
    // Cannot clear the console
    return false;
}

bool log_text_field::clear() {
    if (m_output_field) {
        m_output_field->clear();
        return true;
    }
    return false;
}

log_text_field::log_text_field(QTextEdit *output_field) :
    m_output_field(output_field) {}
