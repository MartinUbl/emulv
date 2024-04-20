#pragma once
#include <string>
/**
 * Configures the global spdlog logger.
 */
void setupLogger();
/**
 * Function to prevent crashing if it's impossible to write logs.
 * @param path Path to a test log file
 * @return True if it's possible to write, otherwise false.
 */
bool canWriteLog(const std::string& path);

