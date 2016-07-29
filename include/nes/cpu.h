#ifndef CPU_H
#define CPU_H

#include <cstdint>

#include "bitfield.h"
#include "nesmediator.h"

class CPU
{
public:
    CPU();

private:
    //Registers
    uint16_t PC;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t S;
    Bitfield P;

    NesMediator mediator;

    unsigned int cycleCount;

    //Addressing modes (returns an address)
    uint16_t implicitAM() const;
    uint16_t accumulatorAM() const;
    uint16_t immediateAM();
    uint16_t zeroPageAM();
    uint16_t zeroPageXAM();
    uint16_t zeroPageYAM();
    uint16_t relativeAM();
    uint16_t absoluteAM();
    uint16_t absoluteXAM();
    uint16_t absoluteYAM();
    uint16_t indirectAM();
    uint16_t indexedIndirectAM();
    uint16_t indirectIndexedAM();

    void tick();
};

#endif
