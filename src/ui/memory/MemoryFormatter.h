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
    static std::string FormatHeader(MemoryFormat format);
    static std::string FormatMemory(int start_address, const std::vector<uint8_t>& memory, MemoryFormat format);

private:
    static const int kAddressWidth = 8;

    static char FormatChar(uint8_t byte);
    static std::string FormatByte(int byte, MemoryFormat format);
    static std::string FormatAddress(int address);
    static std::string FormatEmptyByte(MemoryFormat format);
};

#endif //EMULV_MEMORYFORMATTER_H
