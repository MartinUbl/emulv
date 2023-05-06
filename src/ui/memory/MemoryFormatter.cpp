//
// Created by Hynek on 30.04.2023.
//
#include "MemoryFormatter.h"

#include <sstream>
#include <iomanip>

std::string MemoryFormatter::FormatHeader(MemoryFormat format) {
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

std::string MemoryFormatter::FormatMemory(int start_address, const std::vector<uint8_t> &memory, MemoryFormat format) {
    if (memory.empty()) {
        return "";
    }

    std::stringstream ss;
    std::stringstream ss_ascii;

    int aligned_address = (start_address >> 4) << 4;
    uint8_t start_byte = start_address & 0xF;
    uint8_t current_byte = 0; // Byte column, ranges from 0 to 15

    ss << FormatAddress_(aligned_address) << " ";

    // First empty bytes
    for (; current_byte < start_byte; ++current_byte) {
        ss << FormatEmptyByte_(format) << " ";
        ss_ascii << " ";
    }

    for (size_t i = 0; i < memory.size(); ++i, ++current_byte) {
        // If current byte needs to be on a new line, add a new line
        if (current_byte == 16) {
            current_byte = 0;
            aligned_address += 16;

            ss << "   " << ss_ascii.str() << '\n';
            ss << FormatAddress_(aligned_address) << " ";
            ss_ascii.str("");
        }

        uint8_t byte = memory.at(i);
        ss << FormatByte_(byte, format) << " ";
        ss_ascii << FormatChar_(byte);
    }

    // Last empty bytes
    for (; current_byte < 16; ++current_byte) {
        ss << FormatEmptyByte_(format) << " ";
        ss_ascii << " ";
    }

    ss << "   " << ss_ascii.str();

    return ss.str();
}

char MemoryFormatter::FormatChar_(uint8_t byte) {
    return byte >= 32 && byte < 127 ? (char)byte : '.';
}

std::string MemoryFormatter::FormatByte_(int byte, MemoryFormat format) {
    std::stringstream ss;

    switch (format) {
        case kHex: ss << std::setw(2) << std::uppercase << std::hex; break;
        case kDec: ss << std::setw(3); break;
    }

    ss << std::setfill('0') << byte;

    return ss.str();
}

std::string MemoryFormatter::FormatEmptyByte_(MemoryFormat format) {
    switch (format) {
        case kHex: return "  ";
        case kDec: return "   ";
    }
    return "";
}

std::string MemoryFormatter::FormatAddress_(int address) {
    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setw(kAddressWidth) << std::setfill('0') << address;
    return ss.str();
}
