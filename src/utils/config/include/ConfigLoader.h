//
// Created by xPC on 04.05.2023.
//

#pragma once

#include "nlohmann/json.hpp"

//struct ConfigData {
//    uint64_t ramSize;
//    uint64_t ramStartAddress;
//    std::vector<std::basic_string<char>> programArgs;
//    std::map<std::basic_string<char>, modules::PeripheralDevice *> peripheralDevices;
//};

/**
 * Will load the specified configuration file.
 * @param path Path to the JSON file.
 * @return Parsed JSON object
 */
nlohmann::json loadConfig(const std::string &path);