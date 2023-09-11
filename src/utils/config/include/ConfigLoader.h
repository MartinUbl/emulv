//
// Created by xPC on 04.05.2023.
//

#pragma once

#include "nlohmann/json.hpp"


/**
 * Will load the specified configuration file.
 * @param path Path to the JSON file.
 * @return Parsed JSON object
 */
nlohmann::json loadConfig(const std::string& path);
