#include "../include/file_io.h"
#include <cstdint>
#include <fstream>
#include <stdexcept>

std::vector<uint8_t> readFile(const std::string &path) {
    std::ifstream ifs(path, std::ios::binary | std::ios::ate);
    if (!ifs) throw std::runtime_error("Failed to open file: " + path);
    std::ifstream::pos_type pos = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::vector<uint8_t> data(static_cast<size_t>(pos));
    if (!data.empty()) ifs.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(pos));
    return data;
}

void writeFile(const std::string &path, const std::vector<uint8_t> &data) {
    std::ofstream ofs(path, std::ios::binary);
    if (!ofs) throw std::runtime_error("Failed to open file for writing: " + path);
    if (!data.empty()) ofs.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
}
