#pragma once

#include "nlohmann/json.hpp"
#include "PeripheralsApi.h"
#include "LibLoader.h"
#include <unordered_set>

namespace configLoader {
    /**
    * Stores all data gathered from the configuration file.
    */
    struct ConfigData {
        uint64_t ramSize;
        uint64_t ramStartAddress;
        std::vector<std::string> programArgs;
        std::map<std::string, std::shared_ptr<peripherals::PeripheralsApi>> peripheralDevices;
    };

    class parser {
    public:
        /**
         * Will parse the specified JSON config file. And return it's values.
         * @param path Filesystem path of the config file
         * @return A ConfigData object containing loaded data
        */
        static ConfigData loadJSON(const std::string &path);

    private:
        static nlohmann::json _loadFile(const std::string &path);
        inline static std::unordered_set<std::unique_ptr<LibLoader<peripherals::PeripheralsApi>>> _libLoaders{};
    };

}
