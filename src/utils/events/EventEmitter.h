#include <map>
#include <string>
#include <functional>

#ifndef EMULV_EVENTEMITTER_H
#define EMULV_EVENTEMITTER_H
class EventEmitter {
    std::map<std::string, std::vector<std::function<void( std::string )>>> eventMap;
public:
    void On(const std::string &evt_name, std::function<void(std::string )> listener );
    void Emit(const std::string &evt_name, std::string data);
};
#endif //EMULV_EVENTEMITTER_H
