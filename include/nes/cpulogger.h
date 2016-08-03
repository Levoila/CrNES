#ifndef CPULOGGER_H
#define CPULOGGER_H

#include "opdef.h"
#include "bitfield.h"

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>

class CpuLogger
{
public:
    CpuLogger(const std::string& filename);

    void setPC(uint16_t PC) { this->PC = PC; }
    void setOpcode(Opcode opcode) { this->opcode = opcode; }
    void addMemLocation(uint8_t memLocation) { memLocations.push_back(memLocation); }
    void setRegisters(uint8_t A, uint8_t X, uint8_t Y, uint8_t S, Bitfield P)
    {
        this->A = A;
        this->X = X;
        this->Y = Y;
        this->S = S;
        this->P = P;
    }

    void finishInstruction();
private:
    std::string getOpName() const;
    std::string getAddrStr() const;

    uint16_t PC;
    std::vector<uint8_t> memLocations;
    Opcode opcode;
    int A;
    int X;
    int Y;
    int S;
    Bitfield P;

    int lineCount;
    std::ofstream file;
};

#endif
