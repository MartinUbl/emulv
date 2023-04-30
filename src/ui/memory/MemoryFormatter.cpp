//
// Created by Hynek on 30.04.2023.
//

#include <sstream>
#include <iomanip>
#include "MemoryFormatter.h"

std::string MemoryFormatter::formatHeader(MemoryFormat format) {
    std::stringstream ss;
    ss << std::setw(kAddressWidth) << std::setfill(' ') << "";

    switch (format) {
        case kHex: ss << " 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F"; break;
        case kDec: ss << "  00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F"; break;
    }

    // ASCII column header
    ss << "    0123456789ABCDEF   ";

    return ss.str();
}

std::string MemoryFormatter::formatMemory(int start_address, const std::vector<uint8_t> &memory, MemoryFormat format) {
    if (memory.empty()) {
        return "";
    }

    std::stringstream ss;
    std::stringstream ssAscii;

    int aligned_address = (start_address >> 4) << 4;
    uint8_t start_byte = start_address & 0xF;
    uint8_t current_byte = 0; // Byte column, ranges from 0 to 15

    ss << formatAddress_(aligned_address) << " ";

    // First empty bytes
    for (; current_byte < start_byte; ++current_byte) {
        ss << formatEmptyByte_(format) << " ";
        ssAscii << " ";
    }

    for (size_t i = 0; i < memory.size(); ++i, ++current_byte) {
        if (current_byte == 16) {
            current_byte = 0;
            aligned_address += 16;

            ss << "   " << ssAscii.str() << '\n';
            ss << formatAddress_(aligned_address) << " ";
            ssAscii.str("");
        }

        uint8_t byte = memory.at(i);
        ss << formatByte_(byte, format) << " ";
        ssAscii << formatChar_(byte);
    }

    // Last empty bytes
    for (; current_byte < 16; ++current_byte) {
        ss << formatEmptyByte_(format) << " ";
        ssAscii << " ";
    }

    ss << "   " << ssAscii.str();

    return ss.str();
}

char MemoryFormatter::formatChar_(uint8_t byte) {
    return byte >= 32 && byte < 127 ? (char)byte : '.';
}

std::string MemoryFormatter::formatByte_(int byte, MemoryFormat format) {
    std::stringstream ss;

    switch (format) {
        case kHex: ss << std::uppercase << std::hex << std::setw(2); break;
        case kDec: ss << std::setw(3); break;
    }

    ss << std::setfill('0') << byte;

    return ss.str();
}

std::string MemoryFormatter::formatEmptyByte_(MemoryFormat format) {
    switch (format) {
        case kHex: return "  ";
        case kDec: return "   ";
    }
    return "";
}

std::string MemoryFormatter::formatAddress_(int address) {
    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setw(kAddressWidth) << std::setfill('0') << address;

    return ss.str();
}
