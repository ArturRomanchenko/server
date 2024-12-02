#ifndef LOGS_HPP
#define LOGS_HPP

#include <iostream>
#include <string>
#include <mutex>
#include <fstream>
#include <iomanip>
#include <chrono>

namespace utils::logs
{
    enum class level {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    static std::mutex log_mutex;

    std::string get_current_time();
    void log(level log_level, const std::string &message, const std::string& module = "Server");
    void exitWithError(const std::string &error_message);

} // utils::logs

#endif // LOGS_HPP