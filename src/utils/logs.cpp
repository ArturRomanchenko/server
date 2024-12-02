#include "utils/logs.hpp"

std::string utils::logs::get_current_time()
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&time);

    std::ostringstream timeStream;
    timeStream << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return timeStream.str();
}

/**
 * TODO:
 *  
 * File creation 
 * Catch exceptions
 *  
 */

void utils::logs::log(level log_level, const std::string &message, const std::string& module)
{
    std::lock_guard<std::mutex> guard(log_mutex);
    
    std::string log_message = "[" + get_current_time() + "]";

    switch(log_level) {
        case level::DEBUG: log_message += " [DEBUG]"; break;
        case level::INFO: log_message += " [INFO]"; break;
        case level::WARNING: log_message += " [WARN]"; break;
        case level::ERROR: log_message += " [ERROR]"; break;
        case level::FATAL: log_message += " [FATAL]"; break;
    }

    log_message += " [" + module + "]";
    log_message += " " + message;

    std::ofstream out("../logs/application.log", std::ios::app);
    if (out.is_open())
    {
        out << log_message << '\n';
    }
    else
    {
        std::cerr << "Error opening '.log' file for writing!" << std::endl;
    }
    out.close();
}

/*
void utils::logs::exitWithError(const std::string &error_message)
{
    log(level::ERROR, error_message);
    exit(1); // It might be worth using an exception instead of exit(1) 
}
*/