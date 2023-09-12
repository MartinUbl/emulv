//
// Created by xPC on 12.09.2023.
//

#include "Events.h"

namespace EventsLib {
    void registerEmitter(std::shared_ptr<EventEmitter> emitter) {
        EmitterStore::getInstance().emitters.try_emplace(emitter->getName(), emitter);
    }

    void removeEmitter(const std::string &name) {
        EmitterStore::getInstance().emitters.erase(name);
    }

    void setGlobalEmitter(const std::shared_ptr<EventEmitter>& emitter) {
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
        auto global = std::make_shared<EventEmitter>("GlobalEmitter");
        setGlobalEmitter(global);
        return global;
    }
}
