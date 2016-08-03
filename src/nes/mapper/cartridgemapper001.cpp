#include "cartridgemapper001.h"

#include <cassert>

CartridgeMapper001::CartridgeMapper001(Mirroring mirroring, const std::vector<std::array<uint8_t, 0x4000>>& prgRom, const std::vector<std::array<uint8_t, 0x2000>>& chrRom)
    : CartridgeMapper(mirroring, prgRom, chrRom)
{
    prgRam.fill(0xFF);
    mapperId = 0;

    //If we only have one page of prg rom we have to mirror it for the higher addresses
    prgRomMask = prgRom.size() == 1 ? 0xBFFF : 0xFFFF;
}

uint8_t CartridgeMapper001::readCpuBus(uint16_t addr)
{
    assert(addr >= 0x6000 && addr < 0x10000);

    if (addr < 0x8000) {
        return prgRam[addr - 0x6000];
    } else {
        return prgRom[(addr & prgRomMask) - 0x8000];
    }
}

uint8_t CartridgeMapper001::readPpuBus(uint16_t addr)
{
    return 0x00;
}

void CartridgeMapper001::writeCpuBus(uint16_t addr, uint8_t data)
{
    assert(addr >= 0x6000 && addr < 0x8000);

    prgRam[addr - 0x6000] = data;
}

void CartridgeMapper001::writePpuBus(uint16_t addr, uint8_t data)
{

}
