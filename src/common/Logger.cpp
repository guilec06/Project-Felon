/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Logger
*/

#include "Logger.hpp"
namespace logger {
    Logger::Logger(std::string prefix, std::filesystem::path file)
    : _prefix(prefix), filepath(file)
    {
        logfile->open(filepath, std::ios::out | std::ios::app);
        if (!logfile->is_open())
            std::cerr << "Error: Could not open logfile " << filepath << std::endl;
        _log_in_terminal = true;
    }

    Logger::~Logger()
    {
        if (logfile->is_open()) {
            logfile->close();
        }
    }

    void Logger::log(log_info_t log, std::string message) const
    {
        std::stringstream logBuffer;
        auto instant = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(instant);
        std::tm *now_tm = std::localtime(&now_time);

        std::string logType = log != DEBUG ? log != INFO ? log != WARN ? log == FATAL ? "FATAL" : "ERROR" : "WARN" : "INFO" : "DEBUG";

        logBuffer
            << "[" + logType + "]"
            << "[" << std::put_time(now_tm, "%Y-%m-%d") << "]"
            << "[" << std::put_time(now_tm, "%H:%M:%S") << "]"
            << " "
            << (_prefix.empty() ? "" : "[" + _prefix + "] ")
            << message
            << std::endl;

        if (logfile->is_open())
            *logfile << logBuffer.str();

        if (_log_in_terminal)
            std::cerr << logBuffer.str();
    }

    void Logger::debug(std::string message) const
    {
        this->log(DEBUG, message);
    }

    void Logger::info(std::string message) const
    {
        this->log(INFO, message);
    }

    void Logger::warn(std::string message) const
    {
        this->log(WARN, message);
    }

    void Logger::error(std::string message) const
    {
        this->log(ERROR, message);
    }

    void Logger::fatal(std::string message) const
    {
        this->log(FATAL, message);
    }

    void Logger::toggleTerminalLogging(bool enable)
    {
        _log_in_terminal = enable;
    }
}