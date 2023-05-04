//
// Created by xPC on 04.05.2023.
//

#include <string>
#include "ConfigLoader.h"
#include <fstream>
#include <nlohmann/json.hpp>

nlohmann::json loadConfig(const std::string& path) {
    std::ifstream f(path);
    nlohmann::json data = nlohmann::json::parse(f);
    return data;
}