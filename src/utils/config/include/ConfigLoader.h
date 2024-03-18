//
// Created by xPC on 04.05.2023.
//

#pragma once

#include "nlohmann/json.hpp"
#include "PeripheralsApi.h"

namespace configLoader {
    /**
    * Stores all data gathered from the configuration file.
    */
    struct ConfigData {
        uint64_t ramSize;
        uint64_t ramStartAddress;
        std::vector<std::basic_string<char>> programArgs;
        std::map<std::basic_string<char>, peripherals::PeripheralsApi *> peripheralDevices;
    };

    /**
    * Will parse the specified JSON config file. And return it's values.
    * @param path Filesystem path of the config file
    * @return A ConfigData object containing loaded data
    */
    ConfigData configParser(const std::string &path);

}
