//
// Created by xPC on 12.09.2023.
//

#pragma once

#include <any>
#include <string>
#include <map>

namespace EventsLib {
    class EventData {
    private:
        std::map<std::string, std::any> data_;
    public:
        /**
         * Adds a value to the data object.
         * @param key Key which identifies the value
         * @param value Any value
         */
        void addData(const std::string &key, std::any value);

        /**
         * Gets a value associated with certain key.
         * @param key Key of the value
         * @return Value associated with key
         */
        std::any getData(const std::string &key);

        /**
         * This hashmap contains everything stored inside of this object.
         * @return Hashmap with all of the data
         */
        std::map<std::string, std::any> getData();

        EventData();

        /**
         * Easier way to create this object.
         * Usage: EventData{{key0,value0},{key1,value1}, ...}
         * @param args a list of {key,value} pairs
         */
        EventData(std::initializer_list<std::pair<const std::string, std::any>> args);
    };
}
