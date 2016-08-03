#ifndef CARTRIDGEMAPPER_H
#define CARTRIDGEMAPPER_H

#include "imemory.h"

#include <string>
#include <vector>
#include <array>

enum class Mirroring { VERTICAL, HORIZONTAL, FOUR_SCREEN, SINGLE_SCREEN, BAD_MIRRORING };

class CartridgeMapper
{
public:
    CartridgeMapper(Mirroring mirroring, const std::vector<std::array<uint8_t, 0x4000>>& prgRom, const std::vector<std::array<uint8_t, 0x2000>>& chrRom);

    int getMapperId() const { return mapperId; }
    Mirroring getMirroring() const { return mirroring; }

    virtual uint8_t readCpuBus(uint16_t addr) = 0;
    virtual uint8_t readPpuBus(uint16_t addr) = 0;

    virtual void writeCpuBus(uint16_t addr, uint8_t data) = 0;
    virtual void writePpuBus(uint16_t addr, uint8_t data) = 0;

protected:
    Mirroring mirroring;
    int mapperId;

    std::vector<uint8_t> prgRom;
    std::vector<uint8_t> chrRom;
};

CartridgeMapper* loadCartridgeMapperFromFile(const std::string& filename);

template<size_t N>
std::vector<uint8_t> toContiguousVector(const std::vector<std::array<uint8_t, N>>& data)
{
    std::vector<uint8_t> vec(N * data.size());

    auto currentPos = vec.begin();
    for (const auto& arr : data) {
        currentPos = std::copy(arr.cbegin(), arr.cend(), currentPos);
    }

    return vec;
}

#endif
