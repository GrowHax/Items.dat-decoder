#pragma once
#include <cstdint>
#include <vector>
#include <stdexcept>

struct ExtendBuffer {
    std::vector<uint8_t> data;
    size_t mempos = 0;

    ExtendBuffer() = default;
    ExtendBuffer(size_t size) : data(size), mempos(0) {}
    ExtendBuffer(const std::vector<uint8_t>& d) : data(d), mempos(0) {}

    uint8_t readU8() {
        if (mempos >= data.size()) throw std::out_of_range("readU8 out of range");
        return data[mempos++];
    }
    int8_t readI8() { return static_cast<int8_t>(readU8()); }

    uint16_t readU16() {
        if (mempos + 1 >= data.size()) throw std::out_of_range("readU16 out of range");
        uint16_t v = data[mempos] | (data[mempos+1] << 8);
        mempos += 2;
        return v;
    }
    int16_t readI16() { return static_cast<int16_t>(readU16()); }

    uint32_t readU32() {
        if (mempos + 3 >= data.size()) throw std::out_of_range("readU32 out of range");
        uint32_t v = data[mempos] | (data[mempos+1] << 8) | (data[mempos+2] << 16) | (data[mempos+3] << 24);
        mempos += 4;
        return v;
    }
    int32_t readI32() { return static_cast<int32_t>(readU32()); }

    void writeU8(uint8_t v) {
        if (mempos >= data.size()) data.resize(mempos+1);
        data[mempos++] = v;
    }
    void writeI8(int8_t v) { writeU8(static_cast<uint8_t>(v)); }

    void writeU16(uint16_t v) {
        if (mempos + 1 >= data.size()) data.resize(mempos+2);
        data[mempos++] = v & 0xFF;
        data[mempos++] = (v >> 8) & 0xFF;
    }
    void writeI16(int16_t v) { writeU16(static_cast<uint16_t>(v)); }

    void writeU32(uint32_t v) {
        if (mempos + 3 >= data.size()) data.resize(mempos+4);
        data[mempos++] = v & 0xFF;
        data[mempos++] = (v >> 8) & 0xFF;
        data[mempos++] = (v >> 16) & 0xFF;
        data[mempos++] = (v >> 24) & 0xFF;
    }
    void writeI32(int32_t v) { writeU32(static_cast<uint32_t>(v)); }
};
