#ifndef NESMEDIATOR_H
#define NESMEDIATOR_H

#include "imemory.h"

//class CPU;

class NesMediator : public IMemory
{
public:
    virtual uint8_t read(uint16_t addr) override;
    virtual void write(uint16_t addr, uint8_t data) override;

    void tick();
private:
    //CPU* nes;
};

#endif
