//
// Created by Hynek on 30.04.2023.
//

#ifndef EMULV_MEMORYFORMATTER_H
#define EMULV_MEMORYFORMATTER_H

#include <cstdint>
#include <string>
#include <vector>

enum MemoryFormat {
    kHex, kDec
};

class MemoryFormatter {
public:
    static std::string formatHeader(MemoryFormat format);
    static std::string formatMemory(int start_address, const std::vector<uint8_t>& memory, MemoryFormat format);

private:
    static constexpr size_t kAddressWidth = 8;

    static char formatChar_(uint8_t byte);
    static std::string formatByte_(int byte, MemoryFormat format);
    static std::string formatAddress_(int address);
    static std::string formatEmptyByte_(MemoryFormat format);
};

#endif //EMULV_MEMORYFORMATTER_H
