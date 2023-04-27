//
// Created by xPC on 04.04.2023.
//

#ifndef EMULV_PERIPHERALDEVICE_H
#define EMULV_PERIPHERALDEVICE_H

#include <cstdint>

#include "../utils/events/EventEmitter.h"

namespace modules {

    class PeripheralDevice {
       
        protected:
            std::string Name;
            EventEmitter &Emitter;
            uint64_t Start_Address;
            uint64_t End_Address;
        
        public:
            PeripheralDevice(const std::string &name, EventEmitter &emitter, uint64_t start_address, uint64_t end_address) :
                Name(name),
                Emitter(emitter),
                Start_Address(start_address),
                End_Address(end_address)
            {}

            virtual void WriteByte(uint64_t address, uint8_t value) = 0;
            virtual void WriteHalfword(uint64_t address, uint16_t value) = 0;
            virtual void WriteWord(uint64_t address, uint32_t value) = 0;
            virtual void WriteDoubleword(uint64_t address, uint64_t value) = 0;

            virtual uint8_t ReadByte(uint64_t address) = 0;
            virtual uint16_t ReadHalfword(uint64_t address) = 0;
            virtual uint32_t ReadWord(uint64_t address) = 0;
            virtual uint64_t ReadDoubleword(uint64_t address) = 0;

            std::string GetName() {
                return Name;
            }
            uint64_t GetStartAddress() {
                return Start_Address;
            };
            uint64_t GetEndAddress() {
                return End_Address;
            };
        };

}

#endif //EMULV_PERIPHERALDEVICE_H
