#include <map>
#include <string>
#include <functional>
#include "EventEmitter.h"
#include "EventData.h"
#include "spdlog/spdlog.h"

namespace EventsLib {

    void EventEmitter::On(const std::string &evt_name, std::function<void(EventData)> listener) {
        std::scoped_lock lock{emitterMutex_};

        spdlog::info("EventEmitter {0} registered a new listener to the event: {1}", name_, evt_name);

        auto it = eventMap_.find(evt_name);
        if (it != eventMap_.end()) {
            it->second.push_back(listener);
            return;
        }
        eventMap_[evt_name] = std::vector<std::function<void(EventData)>>();
        eventMap_[evt_name].push_back(listener);
    }

    void EventEmitter::Emit(const std::string &evt_name, EventData data) {
        std::scoped_lock lock{emitterMutex_};

        spdlog::info("EventEmitter {0} is emitting an event with the name: {1}", name_, evt_name);

        auto evts = eventMap_.find(evt_name);
        if (evts == eventMap_.end()) return;
        for (auto it = std::begin(evts->second); it != std::end(evts->second); ++it) {
            auto &func = *it;
            func(data);
        }
    }

    void EventEmitter::Emit(const std::string &evt_name) {
        Emit(evt_name, EventData());
    }

    std::string EventEmitter::getName() {
        return name_;
    }

    EventEmitter::EventEmitter(const std::string &name) : name_(name) {}


}


