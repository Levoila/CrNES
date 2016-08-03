#ifndef CPURAM_H
#define CPURAM_H

#include "imemory.h"

#include <cstdint>
#include <array>

class CpuRam : public IMemory
{
public:
    CpuRam();

    virtual uint8_t read(uint16_t addr) override;
    virtual void write(uint16_t addr, uint8_t data) override;
private:
    std::array<uint8_t, 0x0800> mem;
};

#endif
