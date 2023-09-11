#pragma once

#include <vector>
#include <map>
#include <string>
#include <functional>
#include "AbstractEvent.h"

class EventEmitter {
    std::map<std::string, std::vector<std::function<void(AbstractEvent*)>>> eventMap;
public:
    void On(const std::string &evt_name, std::function<void(AbstractEvent*)> listener);

    void Emit(const std::string &evt_name, AbstractEvent* data);
};
