#include <fstream>
#include <iostream>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <libriscv/machine.hpp>
#include <sstream>
#include "Controller.h"
#include "spdlog/spdlog.h"
#include "LoggerConfig.h"

int main(int argc, char **argv) {
    //Global exception handler - will print exception before exiting program.
    try {
        setupLogger();

        spdlog::info("The program main function has started.");

        //Start controller
        Controller c(argc, argv);
        return c.showWindow();
    }
    catch (const std::exception &ex) {

        std::ostringstream error_message;
        error_message << "An exception has occurred. The program will be terminated.\n"
                      << "Exception name: " << typeid(ex).name() << "\n"
                      << "Exception message: " << ex.what() << "\n"
                      << "Errno: " << errno << "\n";

        spdlog::error(error_message.str());
        return EXIT_FAILURE;
    }
    catch (...) {

        std::ostringstream error_message;
        error_message << "An unknown error has occurred. The program will be terminated.\n"
                      << "Error name: " << typeid(std::current_exception()).name() << "\n"
                      << "Errno: " << errno << "\n";

        spdlog::error(error_message.str());
        return EXIT_FAILURE;
    }
}