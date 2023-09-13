//
// Created by xPC on 04.05.2023.
//

#include <string>
#include "ConfigLoader.h"
#include "spdlog/spdlog.h"
#include "uart.h"
#include "gpio.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace configLoader {

    nlohmann::json loadConfig(const std::string &path) {
        std::ifstream f(path);
        nlohmann::json data = nlohmann::json::parse(f);
        return data;
    }

    ConfigData configParser(const std::string &path) {
        const nlohmann::json &config = loadConfig(path);
        ConfigData data;

        spdlog::info("Parsing peripherals configuration...");

        for (const auto &item: config.items()) {
            if (item.key() == "device") {
                //The device element
                spdlog::info("Found a new 'device' config entry");

                for (const auto &val: item.value().items()) {
                    if (val.key() == "ram") {
                        //Information about ram
                        data.ramSize = val.value()["size"].get<uint64_t>();
                        data.ramStartAddress = strtoull(val.value()["start-address"].get<std::string>().c_str(),
                                                        nullptr, 16);

                        spdlog::info("Set the RamSize to {0} and RamStartAddress to {1}", data.ramSize,
                                     data.ramStartAddress);
                    } else if (val.key() == "program-arguments") {
                        //The program arguments list
                        data.programArgs = val.value().get<std::vector<std::string>>();

                        spdlog::info("Set the program_arguments");
                    }
                }
            } else if (item.key() == "peripherals") {
                //The peripherals element
                spdlog::info("Found a new 'peripherals' config entry");

                //Iterate over the peripherals list
                for (const auto &val: item.value().items()) {
                    std::string type = val.value()["type"].get<std::string>();
                    std::string name = val.value()["name"].get<std::string>();
                    uint64_t startAddress = strtoull(
                            val.value()["mapping"]["start-address"].get<std::string>().c_str(), nullptr, 16);
                    uint64_t endAddress = strtoull(val.value()["mapping"]["end-address"].get<std::string>().c_str(),
                                                   nullptr, 16);

                    spdlog::info("Found a peripheral of type: {0} name: {1} startAddress: {2} endAddress: {3}", type,
                                 name,
                                 startAddress, endAddress);
                    //A GPIO_Port element
                    if (type == "GPIO_Port") {
                        data.peripheralDevices[name] = new modules::GPIO_Port(name, startAddress, endAddress);
                    }

                    //An UART_Device element
                    if (type == "UART_Device") {
                        data.peripheralDevices[name] = new modules::UART_Device(name, startAddress, endAddress);
                    }
                }
            }
        }
        return data;
    }

}
