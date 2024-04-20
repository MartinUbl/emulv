#include "Events.h"
#include "spdlog/spdlog.h"

#include <utility>

namespace EventsLib {
    void registerEmitter(std::shared_ptr<EventEmitter> emitter) {
        spdlog::trace("EventsLib: The emitter {0} was registered", emitter->getName());
        EmitterStore::getInstance().emitters.try_emplace(emitter->getName(), emitter);
    }

    void removeEmitter(const std::string &name) {
        spdlog::trace("EventsLib: The emitter {0} was removed", name);
        EmitterStore::getInstance().emitters.erase(name);
        if(name == EmitterStore::getInstance().globalName)
            EmitterStore::getInstance().globalName.clear();
    }

    void setGlobalEmitter(const std::shared_ptr<EventEmitter> &emitter) {
        spdlog::trace("EventsLib: The emitter {0} has been set as global", emitter->getName());

        EmitterStore &store = EmitterStore::getInstance();

        store.globalName = emitter->getName();

        if (store.emitters.find(emitter->getName()) == store.emitters.end()) {
            //This emitter is not in the emitters map, register it
            registerEmitter(emitter);
        }
    }

    std::shared_ptr<EventEmitter> getEmitter(const std::string &name) {
        return EmitterStore::getInstance().emitters[name];
    }

    std::shared_ptr<EventEmitter> getGlobalEmitter() {
        if (EmitterStore &store = EmitterStore::getInstance(); !store.globalName.empty()) {
            return store.emitters[store.globalName];
        }

        //There is no global emitter, create one
        spdlog::trace("EventsLib: Creating a default global emitter...");
        auto global = std::make_shared<EventEmitter>("Global");
        setGlobalEmitter(global);
        return global;
    }

    void globalEmit(const std::string &evt_name) {
        getGlobalEmitter()->Emit(evt_name);
    }

    void globalEmit(const std::string &evt_name, EventData data) {
        getGlobalEmitter()->Emit(evt_name,std::move(data));
    }

    void globalOn(const std::string &evt_name, std::function<void(EventData)> listener) {
        getGlobalEmitter()->On(evt_name,std::move(listener));
    }
}
