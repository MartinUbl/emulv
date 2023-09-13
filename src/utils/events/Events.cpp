//
// Created by xPC on 12.09.2023.
//
// "Every type in C++ is determined at compile-time."
// "The auto keyword is a type inference feature in C++ that allows the compiler to deduce the type of variable from its initializer expression. The type of the variable is then automatically determined at compile time."
//
//

#include "Events.h"

#include <utility>

namespace EventsLib {
    void registerEmitter(std::shared_ptr<EventEmitter> emitter) {
        EmitterStore::getInstance().emitters.try_emplace(emitter->getName(), emitter);
    }

    void removeEmitter(const std::string &name) {
        EmitterStore::getInstance().emitters.erase(name);
    }

    void setGlobalEmitter(const std::shared_ptr<EventEmitter> &emitter) {
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
