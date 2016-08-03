#ifndef NESMEDIATOR_H
#define NESMEDIATOR_H

#include "imemory.h"
#include "cpuram.h"
#include "cartridgemapper.h"

class Cpu;

class CpuBus : public IMemory
{
public:
    CpuBus(CpuRam* cpuRam = nullptr, CartridgeMapper* cartridge = nullptr);

    virtual uint8_t read(uint16_t addr) override;
    virtual void write(uint16_t addr, uint8_t data) override;

    void tick();
    void setCartridge(CartridgeMapper* cartridge) { this->cartridge = cartridge; }
private:
    CpuRam* cpuRam;
    CartridgeMapper* cartridge;
};

#endif
