#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <sstream>
#include <memory>

#include "clock_time.h"

class QString;
class QTextEdit;
class log_stream;

/**
 * Objects of this type forward log outputs.
 * Thie base class is no-op.
 */
struct log_out {
    virtual bool operator<<(const std::string &str);

    virtual bool clear();
};

/**
 * Forward log outputs to stdout.
 */
struct log_stdout : public log_out {
    bool operator<<(const std::string &str) override;

    bool clear() override;
};

/**
 * Forward log outputs to a QTextEdit.
 */
struct log_text_field : public log_out {
    explicit log_text_field(QTextEdit *output_field);

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
    static bool log(const std::string &message, LogType type = INFO);
    static bool log(const QString &message, LogType type = INFO);

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

    bool log_message(const std::string &message, LogType type);

    std::unique_ptr<log_out> m_log_out;

    friend class log_stream;
};

/**
 * Log stream class returned by @code log(Logger::LogType) @endcode
 * which accepts logging output as a stream.
 */
class log_stream {
public:
    explicit log_stream(Logger::LogType log_type);

    log_stream(log_stream &&o) noexcept;

    /**
     * Destructor inserts a newline in the buffer then outputs
     * the message.
     */
    ~log_stream();

    /**
     * Insert a value into the output buffer.
     *
     * @tparam out_t output type
     * @param v      output type value
     * @return       reference to this output stream
     */
    template<typename out_t>
    log_stream &operator<<(const out_t &v) {
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
log_stream log(Logger::LogType log_type = Logger::INFO);

log_stream debug();

log_stream fatal();

#endif // LOGGER_H
