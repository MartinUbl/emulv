//
// Created by xPC on 12.09.2023.
//

#include "EventData.h"

#include <utility>

EventsLib::EventData::EventData(std::initializer_list<std::pair<const std::string, std::any>> args) : data_(args) {}

void EventsLib::EventData::addData(const std::string &key, std::any value) {
    data_[key] = std::move(value);
}

std::any EventsLib::EventData::getData(const std::string &key) {
    return data_[key];
}

std::map<std::string, std::any> EventsLib::EventData::getData() {
    return data_;
}

void EventsLib::EventData::removeData(const std::string &key) {
    data_.erase(key);
}
