#pragma once
#include "ExtendBuffer.h"
#include "ItemDefinition.h"
#include <string>
#include <vector>

class ItemsDat {
public:
    ExtendBuffer buffer;
    std::string key = "PBG892FXX982ABC*";
    ItemsDatMeta meta;

    ItemsDat() = default;
    ItemsDat(const std::vector<uint8_t>& data) : buffer(data) {}
    ItemsDat(size_t size) : buffer(size) {}

    int getWriteSize();
    std::string readString(bool encoded=false, int id=0);
    void writeString(const std::string &s, int id=0, bool encoded=false);
    void decode();
    void encode();
};
