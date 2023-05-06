#include <fstream>
#include <iostream>

#include <libriscv/machine.hpp>
#include <sstream>

#include "emulator/EmulatorUnit.h"
#include "ui/Controller.h"

int main(int argc, char **argv) {
    //Global exception handler - will show an error window with the exception before exiting program.
    try {
        Controller c(argc, argv);
        return c.ShowWindow();
    }
    catch (const std::exception &ex) {

        std::ostringstream error_message;
        error_message << "An exception has occurred. The program will be terminated.\n"
                      << "Exception name: " << typeid(ex).name() << "\n"
                      << "Exception message: " << ex.what() << "\n"
                      << "Errno: " << errno << "\n";

        std::cout << error_message.str() << std::endl;

        //QMessageBox::critical(nullptr, "Error", QString::fromStdString(error_message.str()));
        return EXIT_FAILURE;
    }
    catch (...) {

        std::ostringstream error_message;
        error_message << "An unknown error has occurred. The program will be terminated.\n"
                      << "Error name: " << typeid(std::current_exception()).name() << "\n"
                      << "Errno: " << errno << "\n";

        std::cout << error_message.str() << std::endl;

        //QMessageBox::critical(nullptr, "Error", QString::fromStdString(error_message.str()));
        return EXIT_FAILURE;
    }
}

