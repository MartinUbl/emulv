#pragma once

#include <vector>
#include <map>
#include <string>
#include <functional>
#include "EventData.h"

namespace EventsLib {
    class EventEmitter {
    private:
        std::map<std::string, std::vector<std::function<void(EventData)>>> eventMap_;
        std::string name_;
    public:

        /**
        * Construct a new EventEmitter.
        * @param name Name of this EventEmitter
        */
        EventEmitter(const std::string &name);

        /**
        * Attach a listener function, which will be executed when certain even is emitted.
        * (Name of this method is with upper case "O" to avoid conflict with Qt library macro)
        * @param evt_name Name of the handled event
        * @param listener Reference to the handler function
        */
        void On(const std::string &evt_name, std::function<void(EventData)> listener);

        /**
        * Emits an event, appropriate listeners are then notified.
        * (Name of this method is with upper case "E" to avoid conflict with Qt library macro)
        * @param evt_name Name of the event
        * @param data Data attached to this event
        */
        void Emit(const std::string &evt_name, EventData data);

        /**
         * Emits an event, appropriate listeners are then notified.
         * This is a special case. Emits an event without any data attached to it.
         * Internally the standard Emit function will be called with an empty EventData object.
         * @param evt_name Name of the event
         */
        void Emit(const std::string &evt_name);

        /**
         * @return Name of this EventEmitter
         */
        std::string getName();
    };
}
