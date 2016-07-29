#include "cpu.h"

CPU::CPU()
    :cycleCount(0)
{

}

void CPU::tick()
{
    ++cycleCount;
    mediator.tick();
}


//TODO : Check timing
uint16_t CPU::implicitAM() const
{
    return 0;
}

uint16_t CPU::accumulatorAM() const
{
    return 0;
}

uint16_t CPU::immediateAM()
{
    return PC++;
}

uint16_t CPU::zeroPageAM()
{
    return mediator.read(PC++);
}

uint16_t CPU::zeroPageXAM()
{
    tick();
    tick();
    return static_cast<uint8_t>(mediator.read(PC++) + X);
}

uint16_t CPU::zeroPageYAM()
{
    tick();
    tick();
    return static_cast<uint8_t>(mediator.read(PC++) + Y);
}

uint16_t CPU::relativeAM()
{
    return mediator.read(PC++);
}

uint16_t CPU::absoluteAM()
{
    tick();
    tick();
    return mediator.read(PC++) | (mediator.read(PC++) << 8);
}

uint16_t CPU::absoluteXAM()
{
    uint16_t addr = mediator.read(PC++) | (mediator.read(PC++) << 8);

    tick();
    tick();
    if ((addr & 0x00FF) + X > 0xFF) {
        tick();
    }

    return addr + X;
}

uint16_t CPU::absoluteYAM()
{
    uint16_t addr = mediator.read(PC++) | (mediator.read(PC++) << 8);

    tick();
    tick();
    if ((addr & 0x00FF) + Y > 0xFF) {
        tick();
    }

    return addr + Y;
}

uint16_t CPU::indirectAM()
{
    uint8_t low = mediator.read(PC++);
    uint8_t high = mediator.read(PC++);
    uint8_t addrLow = mediator.read(low | (high << 8));
    uint8_t addrHigh = mediator.read(static_cast<uint8_t>(low + 1) | (high << 8)); //page wrapping

    tick();
    tick();
    tick();
    tick();

    return addrLow | (addrHigh << 8);
}

uint16_t CPU::indexedIndirectAM()
{
    tick();
    tick();
    tick();
    tick();

    uint8_t base = mediator.read(PC++) + X;
    return mediator.read(base) | (mediator.read(static_cast<uint8_t>(base + 1)) << 8); //page wrapping
}

uint16_t CPU::indirectIndexedAM()
{
    uint8_t base = mediator.read(PC++);
    uint16_t addr = mediator.read(base) | (mediator.read(static_cast<uint8_t>(base + 1)) << 8); //page wrapping

    tick();
    tick();
    tick();
    if ((addr & 0x00FF) + Y > 0xFF) {
        tick();
    }

    return addr + Y;
}





















