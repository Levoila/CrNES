#ifndef IMEMORY_H
#define IMEMORY_H

#include <cstdint>

class IMemory
{
public:
    virtual uint8_t read(uint16_t addr) = 0;
    virtual void write(uint16_t addr, uint8_t data) = 0;
private:

};

#endif
