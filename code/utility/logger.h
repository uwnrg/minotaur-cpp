#ifndef LOGGER_H
#define LOGGER_H

#include <QTextEdit>
#include <QString>
#include <QTextStream>

#include <iostream>
#include <string>
#include <memory>
#include <sstream>

#include "clock_time.h"

class __log_stream;

/**
 * Objects of this type forward log outputs.
 * Thie base class is no-op.
 */
struct __log_out {
    virtual bool operator<<(const std::string &str);

    virtual bool clear();
};

/**
 * Forward log outputs to stdout.
 */
struct __log_stdout : public __log_out {
    bool operator<<(const std::string &str) override;

    bool clear() override;
};

/**
 * Forward log outputs to a QTextEdit.
 */
struct __log_text_field : public __log_out {
    explicit __log_text_field(QTextEdit *output_field);

    bool operator<<(const std::string &str) override;

    bool clear() override;
private:
    QTextEdit *m_output_field;
};

std::ostream &operator<<(std::ostream &ss, const QString &qstr);

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
    template<typename string_t>
    static bool log(const string_t &message, LogType type = INFO) {
        return s_logger.log_message<string_t>(message, type);
    }

    /**
     * Set the QTextEdit to which logging output should be
     * forwarded instead of stdout.
     *
     * @param output_field pointer to the output field
     */
    static void setStream(QTextEdit *output_field);

    /**
     * Set the logger to output to the console.
     */
    static void setStdout();

    /**
     * Clear the log output.
     *
     * @return true if the logging output has been cleared
     */
    static bool clear_log();

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
            default: // INFO
                ss << "black";
                break;
        }
        ss << "\">"
           << ClockTime::getCurrentTime()
           << message
           << "</font>";
        return *m_log_out << ss.str();
    }

    std::unique_ptr<__log_out> m_log_out;

    friend class __log_stream;
};

/**
 * Log stream class returned by @code log(Logger::LogType) @endcode
 * which accepts logging output as a stream.
 */
class __log_stream {
public:
    explicit __log_stream(Logger::LogType log_type);

    __log_stream(__log_stream &&o) noexcept;

    /**
     * Destructor inserts a newline in the buffer then outputs
     * the message.
     */
    ~__log_stream();

    /**
     * Insert a value into the output buffer.
     *
     * @tparam out_t output type
     * @param v      output type value
     * @return       reference to this output stream
     */
    template<typename out_t>
    __log_stream &operator<<(const out_t &v) {
        m_buffer << v;
        return *this;
    }

private:
    std::stringstream m_buffer;
    Logger::LogType m_log_type = Logger::LogType::INFO;
};

/**
 * Log function, usage: @code log(Logger::DEBUG) << message; @endcode
 *
 * @param log_type the log type
 * @return a logging stream
 */
__log_stream log(Logger::LogType log_type = Logger::INFO);

__log_stream debug();

__log_stream fatal();

#endif // LOGGER_H
