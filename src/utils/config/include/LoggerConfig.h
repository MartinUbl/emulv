//
// Created by xPC on 11.09.2023.
//

#ifndef EMULV_LOGGERCONFIG_H
#define EMULV_LOGGERCONFIG_H

/**
 * Configures the global spdlog logger.
 */
void setupLogger();
/**
 * Function to prevent crashing if it's impossible to write logs.
 * @param path Path to a test log file
 * @return True if it's possible to write, otherwise false.
 */
bool canWriteLog(const char *path);

#endif //EMULV_LOGGERCONFIG_H
