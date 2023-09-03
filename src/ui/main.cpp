#include <fstream>
#include <iostream>

#include <libriscv/machine.hpp>
#include <sstream>
#include "Controller.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"

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

int main(int argc, char **argv) {
    //Global exception handler - will print exception before exiting program.
    try {
        //Set up global SpdLogger - writes both to console and to file
        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
        //TODO: If installed into "Program files", it's impossible to write logs
        if(canWriteLog("logs/test.txt")) {
            sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/logfile.txt", 23, 59));
        }
        auto combined_logger = std::make_shared<spdlog::logger>("Global Logger", begin(sinks), end(sinks));
        spdlog::register_logger(combined_logger);
        spdlog::set_default_logger(combined_logger);
        spdlog::info("The program main function has started.");

        //Start controller
        Controller c(argc, argv);
        return c.ShowWindow();
    }
    catch (const std::exception &ex) {

        std::ostringstream error_message;
        error_message << "An exception has occurred. The program will be terminated.\n"
                      << "Exception name: " << typeid(ex).name() << "\n"
                      << "Exception message: " << ex.what() << "\n"
                      << "Errno: " << errno << "\n";

        spdlog::error(error_message.str());
        //QMessageBox::critical(nullptr, "Error", QString::fromStdString(error_message.str()));
        return EXIT_FAILURE;
    }
    catch (...) {

        std::ostringstream error_message;
        error_message << "An unknown error has occurred. The program will be terminated.\n"
                      << "Error name: " << typeid(std::current_exception()).name() << "\n"
                      << "Errno: " << errno << "\n";

        spdlog::error(error_message.str());
        //QMessageBox::critical(nullptr, "Error", QString::fromStdString(error_message.str()));
        return EXIT_FAILURE;
    }
}