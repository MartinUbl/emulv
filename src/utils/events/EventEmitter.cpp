#include <map>
#include <string>
#include <functional>
#include "EventEmitter.h"

void EventEmitter::On(const std::string &evt_name, std::function<void(std::string)> listener) {
    auto it = eventMap.find(evt_name);
    if (it != eventMap.end()) {
        it->second.push_back(listener);
        return;
    }
    eventMap[evt_name] = std::vector<std::function<void(std::string)>>();
    eventMap[evt_name].push_back(listener);
}

void EventEmitter::Emit(const std::string &evt_name, std::string data) {
    auto evts = eventMap.find(evt_name);
    if (evts == eventMap.end()) return;
    for (std::vector<std::function<void(std::string)>>::iterator it = std::begin(evts->second);
         it != std::end(evts->second); ++it) {
        auto &func = *it;
        func(data);
    }
}
