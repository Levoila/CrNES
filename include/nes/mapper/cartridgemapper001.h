#ifndef CARTRIDGEMAPPER001_H
#define CARTRIDGEMAPPER001_H

#include "cartridgemapper.h"

#include <vector>
#include <array>

class CartridgeMapper001 : public CartridgeMapper
{
public:
    CartridgeMapper001(Mirroring mirroring, const std::vector<std::array<uint8_t, 0x4000>>& prgRom, const std::vector<std::array<uint8_t, 0x2000>>& chrRom);

    uint8_t readCpuBus(uint16_t addr) override;
    uint8_t readPpuBus(uint16_t addr) override;

    void writeCpuBus(uint16_t addr, uint8_t data) override;
    void writePpuBus(uint16_t addr, uint8_t data) override;
private:
    std::array<uint8_t, 0x2000> prgRam;
    uint16_t prgRomMask;
};

#endif
