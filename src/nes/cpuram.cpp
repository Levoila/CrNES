#include "cpuram.h"

#include <cassert>

CpuRam::CpuRam()
{
    //TODO : Power-up state of RAM should be unreliable but the last emulator version
    //had a problem with a ROM of bump'n'jump reading unitialized memory locations
    mem.fill(0xFF);
}

uint8_t CpuRam::read(uint16_t addr)
{
    assert(addr < 0x0800);
    return mem[addr];
}

void CpuRam::write(uint16_t addr, uint8_t data)
{
    assert(addr < 0x0800);
    mem[addr] = data;
}
