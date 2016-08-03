#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <array>

#include "bitfield.h"
#include "cpubus.h"
#include "cpulogger.h"
#include "opdef.h"

class Cpu
{
public:
    Cpu();

    void tick();
    void setMediator(CpuBus* mediator) { this->bus = mediator; }

private:
    //Registers
    uint16_t PC;
    uint8_t A;
    uint8_t X;
    uint8_t Y;
    uint8_t S;
    Bitfield P;

    unsigned int cycleCount;
    bool resetSignal;

    CpuBus* bus;

    std::array<Opcode, 0x100> opcodes;

    CpuLogger logger;

    void executeOp(uint16_t addr, Opcode opcode);
    uint16_t getAddress(AddrMode addrMode);
    void generateOpcodes();
    void branchIf(uint16_t offsetAddr, bool condition);


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

    //Operations (takes an address)
    void ADC(uint16_t addr);
    void AND(uint16_t addr);
    void ASL(uint16_t addr, AddrMode addrMode);
    void BCC(uint16_t addr);
    void BCS(uint16_t addr);
    void BEQ(uint16_t addr);
    void BIT(uint16_t addr);
    void BMI(uint16_t addr);
    void BNE(uint16_t addr);
    void BPL(uint16_t addr);
    void BRK(uint16_t addr);
    void BVC(uint16_t addr);
    void BVS(uint16_t addr);
    void CLC(uint16_t addr);
    void CLD(uint16_t addr);
    void CLI(uint16_t addr);
    void CLV(uint16_t addr);
    void CMP(uint16_t addr);
    void CPX(uint16_t addr);
    void CPY(uint16_t addr);
    void DEC(uint16_t addr);
    void DEX();
    void DEY();
    void EOR(uint16_t addr);
    void INC(uint16_t addr);
    void INX();
    void INY();
    void JMP(uint16_t addr);
    void JSR(uint16_t addr);
    void LDA(uint16_t addr);
    void LDX(uint16_t addr);
    void LDY(uint16_t addr);
    void LSR(uint16_t addr, AddrMode addrMode);
    void NOP(uint16_t addr);
    void ORA(uint16_t addr);
    void PHA();
    void PHP();
    void PLA();
    void PLP();
    void ROL(uint16_t addr, AddrMode addrMode);
    void ROR(uint16_t addr, AddrMode addrMode);
    void RTI();
    void RTS();
    void SBC(uint16_t addr);
    void SEC();
    void SED();
    void SEI();
    void STA(uint16_t addr);
    void STX(uint16_t addr);
    void STY(uint16_t addr);
    void TAX();
    void TAY();
    void TSX();
    void TXA();
    void TXS();
    void TYA();
};

#endif
