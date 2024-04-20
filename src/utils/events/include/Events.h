#pragma once

#include <map>
#include <string>
#include <memory>
#include "EventEmitter.h"

namespace EventsLib {
    class EmitterStore {
    public:
        std::map<std::string, std::shared_ptr<EventEmitter>> emitters;
        std::string globalName;

        static EmitterStore &getInstance() {
            static EmitterStore instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }
    };

    /**
     * Register an event emitter.
     * Registered emitter can later be retrieved via the getEmitter() function.
     * @param emitter
     */
    void registerEmitter(std::shared_ptr<EventEmitter> emitter);

    /**
     * Removes a registered emitter from internal store.
     * This emitter will no longer be available via the getEmitter() function.
     * @param name
     */
    void removeEmitter(const std::string &name);

    /**
     * Sets this emitter as global.
     * It can later be retrieved via the getGlobalEmitter() function.
     * @param emitter
     */
    void setGlobalEmitter(const std::shared_ptr<EventEmitter>& emitter);

    /**
     * Returns an emitter which was previously registered using registerEmitter().
     * @param name Name of the stored emitter
     * @return
     */
    std::shared_ptr<EventEmitter> getEmitter(const std::string &name);

    /**
     * Returns the global emitter.
     * (A new one is created if no global emitter exists)
     * @return
     */
    std::shared_ptr<EventEmitter> getGlobalEmitter();

    /**
     * Emits an event via the global emitter.
     * Special case function, internally creates an empty EventData object.
     * @param evt_name Name of the event
     */
    void globalEmit(const std::string &evt_name);

    /**
     * Emits an event via the global emitter.
     * @param evt_name Name of the event
     * @param data Event's data
     */
    void globalEmit(const std::string &evt_name, EventData data);

    /**
     * Attaches a listener function to the global emitter.
     * @param evt_name Name of handled event
     * @param listener Reference to the handler function
     */
    void globalOn(const std::string &evt_name, std::function<void(EventData)> listener);


}
