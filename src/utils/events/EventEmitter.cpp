#include <map>
#include <string>
#include <functional>
#include "EventEmitter.h"
#include "AbstractEvent.h"
#include "spdlog/spdlog.h"

void EventEmitter::On(const std::string &evt_name, std::function<void(AbstractEvent*)> listener) {
    spdlog::info("EventEmitter registered a new listener to the event: {0}", evt_name);

    auto it = eventMap.find(evt_name);
    if (it != eventMap.end()) {
        it->second.push_back(listener);
        return;
    }
    eventMap[evt_name] = std::vector<std::function<void(AbstractEvent*)>>();
    eventMap[evt_name].push_back(listener);
}

void EventEmitter::Emit(const std::string &evt_name, AbstractEvent* data) {
    spdlog::info("EventEmitter is emitting an event with the name: {0}", evt_name);

    auto evts = eventMap.find(evt_name);
    if (evts == eventMap.end()) return;
    for (auto it = std::begin(evts->second);
         it != std::end(evts->second); ++it) {
        auto &func = *it;
        func(data);
    }
}
