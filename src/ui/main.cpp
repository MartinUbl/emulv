#include <iostream>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStandardPaths>
#include <QFontDatabase>
#include <QQmlContext>
#include <QIcon>
#include <QWKQuick/qwkquickglobal.h>

#include <sstream>
#include "spdlog/spdlog.h"
#include "LoggerConfig.h"
#include "Events.h"

// Implemented below main
int startQt(int argc, char **argv);

int main(int argc, char **argv) {
    //Global exception handler - will print exception before exiting program.
    try {
        //Initialize the global emitter
        EventsLib::getGlobalEmitter();

        setupLogger();
        spdlog::info("The program main function has started.");

        return startQt(argc, argv);
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

int startQt(int argc, char **argv) {
    QGuiApplication app(argc, argv);

    QGuiApplication::setOrganizationName("emulv_org");
    QGuiApplication::setApplicationName("emulv");
    QGuiApplication::setApplicationVersion(QT_VERSION_STR);
    QGuiApplication::setWindowIcon(QIcon(":/assets/ev_square.svg"));

    // ENABLE OPENGL
    qputenv("QSG_RHI_BACKEND", "opengl");
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QQmlApplicationEngine engine;

    // Qt Borderless window library
    QWK::registerTypes(&engine);

    QObject::connect(
            &engine,
            &QQmlApplicationEngine::objectCreationFailed,
            &app,
            []() { QCoreApplication::exit(-1); },
            Qt::QueuedConnection);
    engine.loadFromModule("EmulvQt", "Main");

    //Monospace font
    const QFont monospaceFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    engine.rootContext()->setContextProperty("monospaceFont", monospaceFont);

    spdlog::trace("Qt has been started.");
    return app.exec();
}
