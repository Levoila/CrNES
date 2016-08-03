#include "cpubus.h"
#include "cpu.h"

#include <cassert>
#include <iostream>
#include <iomanip>

CpuBus::CpuBus(CpuRam* cpuRam, CartridgeMapper* cartridge)
    : cpuRam(cpuRam), cartridge(cartridge)
{

}

uint8_t CpuBus::read(uint16_t addr)
{
    assert(cpuRam);
    assert(cartridge);

    if (addr < 0x2000) { //CPU RAM locations
        return cpuRam->read(addr & 0x07FF);
    } else if (addr < 0x4000) { //PPU registers
        addr &= 0x2007;

        switch (addr) {
        case 0x2000:
            break;
        case 0x2001:
            break;
        case 0x2002:
            break;
        case 0x2003:
            break;
        case 0x2004:
            break;
        case 0x2005:
            break;
        case 0x2006:
            break;
        case 0x2007:
            break;
        default:
            assert(false);
            break;
        }
        return 0;
    } else if (addr < 0x4020) { //APU registers and controller registers
        return 0;
    } else {
        return cartridge->readCpuBus(addr);
    }
}

void CpuBus::write(uint16_t addr, uint8_t data)
{
    assert(cpuRam);
    assert(cartridge);

    if (addr < 0x2000) { //CPU RAM locations
        return cpuRam->write(addr & 0x07FF, data);
    } else if (addr < 0x4000) { //PPU registers
        addr &= 0x2007;

        switch (addr) {
        case 0x2000:
            break;
        case 0x2001:
            break;
        case 0x2002:
            break;
        case 0x2003:
            break;
        case 0x2004:
            break;
        case 0x2005:
            break;
        case 0x2006:
            break;
        case 0x2007:
            break;
        default:
            assert(false);
            break;
        }
    } else if (addr < 0x4020) { //APU registers, OAM_DMA and controller registers

    } else {
        cartridge->writeCpuBus(addr, data);
    }
}

void CpuBus::tick()
{
    //Called when the CPU has finished a cycle so we can tick
    //the APU and PPU the necessary number of times
}
