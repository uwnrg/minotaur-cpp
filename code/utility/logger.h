#ifndef LOGGER_H
#define LOGGER_H

#include <QTextEdit>
#include <QString>

#include <iostream>
#include <string>
#include <sstream>

#include "clock_time.h"

class __log_stream;

class Logger {
public:
    enum LogType {
        INFO,
        FATAL,
        DEBUG
    };

    /**
     * Log a message to a set output stream, with a timestamp.
     *
     * @param message message to log
     * @param type    the log type
     * @return        true if the log was successful
     */
    static bool log(const std::string &message, LogType type = INFO);

    /**
     * Method for logging QString. Needed due to function
     * overload ambiguity.
     *
     * @param message message to log as a QString
     * @param type    the log type
     * @return        true if the log was successful
     */
    static bool qlog(const QString &message, LogType type = INFO);

    /**
     * Set the QTextEdit to which logging output should be
     * forwarded instead of stdout.
     *
     * @param output_field pointer to the output field
     */
    static void setStream(QTextEdit *output_field);

private:
    static Logger s_logger;

    Logger();

    template<typename string_t>
    bool log_message(const string_t &message, LogType type) {
        std::stringstream ss;
        ss << "<font color=\"";
        switch (type) {
            case FATAL:
                ss << "red";
                break;
            case DEBUG:
                ss << "blue";
                break;
            default:
                // INFO
                ss << "black";
                break;
        }
        ss << "\">"
           << ClockTime::getCurrentTime()
           << message
           << "</font>";
        if (m_output_field != nullptr) {
            m_output_field->append(QString::fromStdString(ss.str()));
        } else {
            std::cout << ss.str() << std::endl;
        }
        return true;
    }

    QTextEdit *m_output_field;

    friend class __log_stream;
};

class __log_stream {
public:
    explicit __log_stream(Logger::LogType log_type);

    __log_stream(__log_stream &&o) noexcept;

    ~__log_stream();

    template<typename out_t>
    __log_stream &operator<<(const out_t &v) {
        m_buffer << v;
        return *this;
    }

private:
    std::stringstream m_buffer;
    Logger::LogType m_log_type = Logger::LogType::INFO;
};

__log_stream log(Logger::LogType);

#endif // LOGGER_H
