/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Logger
*/

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>
#include <filesystem>
#include <ostream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <chrono>

namespace logger {

    typedef enum {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    } log_info_t;

    class Logger {
        public:
            Logger(std::string prefix = "", std::filesystem::path path = "./logfile.log");
            ~Logger();
        
            void log(log_info_t log, std::string message) const;
            void debug(std::string message) const;
            void info(std::string message) const;
            void warn(std::string message) const;
            void error(std::string message) const;
            void fatal(std::string message) const;

            void toggleTerminalLogging(bool enable);
        protected:
        private:
            std::filesystem::path filepath;
            std::string _prefix;
            bool _log_in_terminal;
            std::unique_ptr<std::ofstream> logfile;
    };

    // DO NOT TOUCH
    namespace thread_logger {
        inline Logger logger_instance;
    }

    inline Logger &getLogger() {
        return thread_logger::logger_instance;
    }

}
#endif /* !LOGGER_HPP_ */
