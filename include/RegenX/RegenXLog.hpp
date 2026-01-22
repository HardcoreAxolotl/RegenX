#pragma once

#include <string>

namespace regenx::log {

    /// Initialize logging. Optional, recommended if you want to save logs to a file.
    bool init();

    /// Print a general message to the console.
    /// @param msg The message to output.
    void print(const std::string& msg);

    /// Log an informational message. May not save to file if logging is not set up.
    /// @param msg The message to log.
    void info(const std::string& msg);

    /// Log a debug message. May not save to file if logging is not set up.
    /// @param msg The message to log.
    void debug(const std::string& msg);

    /// Log a trace message for detailed execution tracking. May not save to file if logging is not set up.
    /// @param msg The message to log.
    void trace(const std::string& msg);

    /// Log a warning message. May not save to file if logging is not set up.
    /// @param msg The message to log.
    void warning(const std::string& msg);

    /// Log an error message. May not save to file if logging is not set up.
    /// @param msg The message to log.
    void error(const std::string& msg);

    /// Log a fatal message and stop the program. May not save to file if logging is not set up.
    /// @param msg The message to log.
    void fatal(const std::string& msg);

    /// Log a system-level message. May not save to file if logging is not set up.
    /// @param msg The message to log.
    void system(const std::string& msg);

}
