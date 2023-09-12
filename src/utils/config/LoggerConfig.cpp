//
// Created by xPC on 11.09.2023.
//

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <fstream>
#include "LoggerConfig.h"

/**
 * Function to prevent crashing if it's impossible to write logs.
 * @param path Path to a test log file
 * @return True if it's possible to write, otherwise false.
 */
bool canWriteLog(const char *path) {
    std::ofstream file;
    file.open(path, std::ios_base::app);
    if (!file.is_open()) {
        return false;
    }
    file.close();
    remove(path);
    return true;
}

/**
 * Configures the global spdlog logger.
 */
void setupLogger() {
    //Set up global SpdLogger - writes both to console and to file
    std::vector<spdlog::sink_ptr> sinks;
    //TODO: Impossible to print anything to console on windows, unless running with admin privileges or with debugger attached
//#ifndef _WIN32
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
//#endif
    //TODO: If installed into "Program files", it's impossible to write logs
    if (canWriteLog("logs/test.txt")) {
        sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/logfile.txt", 23, 59));
    }
    auto combined_logger = std::make_shared<spdlog::logger>("GL", begin(sinks), end(sinks));
    combined_logger->set_level(spdlog::level::trace);
    spdlog::register_logger(combined_logger);
    spdlog::set_default_logger(combined_logger);
    spdlog::set_level(spdlog::level::trace);
}