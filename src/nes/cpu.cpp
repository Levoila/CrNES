#include "cpu.h"

#include <iostream>

Cpu::Cpu()
    :cycleCount(0), resetSignal(true), S(0xFD), PC(0xC000), X(0), Y(0), A(0)
{
    generateOpcodes();
}

void Cpu::tick()
{
    if (resetSignal) {
        resetSignal = false;

        //Reset PPU

        PC = mediator.read(0xFFFC) | (mediator.read(0xFFFD) << 8);

        mediator.tick();
        mediator.tick();
        mediator.tick();
        mediator.tick();
        mediator.tick();
        mediator.tick();

        return;
    }

    auto opcode = opcodes[mediator.read(PC++)];

    uint16_t addr = getAddress(opcode.addrMode);
    executeOp(addr, opcode);

    ++cycleCount;

    mediator.tick();
}

void Cpu::executeOp(uint16_t addr, Opcode opcode)
{
    switch (opcode.op) {
    case Op::ADC:
        ADC(addr);
        break;
    case Op::AND:
        AND(addr);
        break;
    case Op::ASL:
        ASL(addr, opcode.addrMode);
        break;
    case Op::BCC:
        BCC(addr);
        break;
    case Op::BCS:
        BCS(addr);
        break;
    case Op::BEQ:
        BEQ(addr);
        break;
    case Op::BIT:
        BIT(addr);
        break;
    case Op::BMI:
        BMI(addr);
        break;
    case Op::BNE:
        BNE(addr);
        break;
    case Op::BPL:
        BPL(addr);
        break;
    case Op::BRK:
        BRK(addr);
        break;
    case Op::BVC:
        BVC(addr);
        break;
    case Op::BVS:
        BVS(addr);
        break;
    case Op::CLC:
        CLC(addr);
        break;
    case Op::CLD:
        CLD(addr);
        break;
    case Op::CLI:
        CLI(addr);
        break;
    case Op::CLV:
        CLV(addr);
        break;
    case Op::CMP:
        CMP(addr);
        break;
    case Op::CPX:
        CPX(addr);
        break;
    case Op::CPY:
        CPY(addr);
        break;
    case Op::DEC:
        DEC(addr);
        break;
    case Op::DEX:
        DEX();
        break;
    case Op::DEY:
        DEY();
        break;
    case Op::EOR:
        EOR(addr);
        break;
    case Op::INC:
        INC(addr);
        break;
    case Op::INX:
        INX();
        break;
    case Op::INY:
        INY();
        break;
    case Op::JMP:
        JMP(addr);
        break;
    case Op::JSR:
        JSR(addr);
        break;
    case Op::LDA:
        LDA(addr);
        break;
    case Op::LDX:
        LDX(addr);
        break;
    case Op::LDY:
        LDY(addr);
        break;
    case Op::LSR:
        LSR(addr, opcode.addrMode);
        break;
    case Op::NOP:
        NOP(addr);
        break;
    case Op::ORA:
        ORA(addr);
        break;
    case Op::PHA:
        PHA();
        break;
    case Op::PHP:
        PHP();
        break;
    case Op::PLA:
        PLA();
        break;
    case Op::PLP:
        PLP();
        break;
    case Op::ROL:
        ROL(addr, opcode.addrMode);
        break;
    case Op::ROR:
        ROR(addr, opcode.addrMode);
        break;
    case Op::RTI:
        RTI();
        break;
    case Op::RTS:
        RTS();
        break;
    case Op::SBC:
        SBS(addr);
        break;
    case Op::SEC:
        SEC();
        break;
    case Op::SED:
        SED();
        break;
    case Op::SEI:
        SEI();
        break;
    case Op::STA:
        STA(addr);
        break;
    case Op::STX:
        STX(addr);
        break;
    case Op::STY:
        STY(addr);
        break;
    case Op::TAX:
        TAX();
        break;
    case Op::TAY:
        TAY();
        break;
    case Op::TSX:
        TSX();
        break;
    case Op::TXA:
        TXA();
        break;
    case Op::TXS:
        TXS();
        break;
    case Op::TYA:
        TYA();
        break;
    default:
        std::cout << "Bad opcode!" << std::endl;
    }
}

uint16_t Cpu::getAddress(AddrMode addrMode)
{
    uint16_t addr = 0x00;

    switch (addrMode) {
    case AddrMode::IMPLICIT:
        addr = immediateAM();
        break;
    case AddrMode::ACCUMULATOR:
        addr = accumulatorAM();
        break;
    case AddrMode::IMMEDIATE:
        addr = immediateAM();
        break;
    case AddrMode::ZERO_PAGE:
        addr = zeroPageAM();
        break;
    case AddrMode::ZERO_PAGE_X:
        addr = zeroPageXAM();
        break;
    case AddrMode::ZERO_PAGE_Y:
        addr = zeroPageYAM();
        break;
    case AddrMode::RELATIVE:
        addr = relativeAM();
        break;
    case AddrMode::ABSOLUTE:
        addr = absoluteAM();
        break;
    case AddrMode::ABSOLUTE_X:
        addr = absoluteXAM();
        break;
    case AddrMode::ABSOLUTE_Y:
        addr = absoluteYAM();
        break;
    case AddrMode::INDIRECT:
        addr = indirectAM();
        break;
    case AddrMode::INDEXED_INDIRECT:
        addr = indexedIndirectAM();
        break;
    case AddrMode::INDIRECT_INDEXED:
        addr = indirectIndexedAM();
        break;
    default:
        std::cout << "Bad Addressing Mode!" << std::endl;
    }

    return addr;
}

void Cpu::generateOpcodes()
{
    opcodes.fill({Op::BAD_OP, AddrMode::BAD_MODE});

    //ADC
    opcodes[0x69] = {Op::ADC, AddrMode::IMMEDIATE};
    opcodes[0x65] = {Op::ADC, AddrMode::ZERO_PAGE};
    opcodes[0x75] = {Op::ADC, AddrMode::ZERO_PAGE_X};
    opcodes[0x6D] = {Op::ADC, AddrMode::ABSOLUTE};
    opcodes[0x7D] = {Op::ADC, AddrMode::ABSOLUTE_X};
    opcodes[0x79] = {Op::ADC, AddrMode::ABSOLUTE_Y};
    opcodes[0x61] = {Op::ADC, AddrMode::INDEXED_INDIRECT};
    opcodes[0x71] = {Op::ADC, AddrMode::INDIRECT_INDEXED};

    //AND
    opcodes[0x29] = {Op::AND, AddrMode::IMMEDIATE};
    opcodes[0x25] = {Op::AND, AddrMode::ZERO_PAGE};
    opcodes[0x35] = {Op::AND, AddrMode::ZERO_PAGE_X};
    opcodes[0x2D] = {Op::AND, AddrMode::ABSOLUTE};
    opcodes[0x3D] = {Op::AND, AddrMode::ABSOLUTE_X};
    opcodes[0x39] = {Op::AND, AddrMode::ABSOLUTE_Y};
    opcodes[0x21] = {Op::AND, AddrMode::INDEXED_INDIRECT};
    opcodes[0x31] = {Op::AND, AddrMode::INDIRECT_INDEXED};

    //ASL
    opcodes[0x0A] = {Op::ASL, AddrMode::ACCUMULATOR};
    opcodes[0x06] = {Op::ASL, AddrMode::ZERO_PAGE};
    opcodes[0x16] = {Op::ASL, AddrMode::ZERO_PAGE_X};
    opcodes[0x0E] = {Op::ASL, AddrMode::ABSOLUTE};
    opcodes[0x1E] = {Op::ASL, AddrMode::ABSOLUTE_X};

    //BCS
    opcodes[0xB0] = {Op::BCS, AddrMode::RELATIVE};

    //BEQ
    opcodes[0xF0] = {Op::BEQ, AddrMode::RELATIVE};

    //BIT
    opcodes[0x24] = {Op::BIT, AddrMode::ZERO_PAGE};
    opcodes[0x2C] = {Op::BIT, AddrMode::ABSOLUTE};

    //BMI
    opcodes[0x30] = {Op::BMI, AddrMode::RELATIVE};

    //BNE
    opcodes[0xD0] = {Op::BNE, AddrMode::RELATIVE};

    //BPL
    opcodes[0x10] = {Op::BPL, AddrMode::RELATIVE};

    //BRK
    opcodes[0x00] = {Op::BRK, AddrMode::IMPLICIT};

    //BVC
    opcodes[0x50] = {Op::BVC, AddrMode::RELATIVE};

    //BVS
    opcodes[0x70] = {Op::BVS, AddrMode::RELATIVE};

    //CLC
    opcodes[0x18] = {Op::CLC, AddrMode::IMPLICIT};

    //CLD
    opcodes[0xD8] = {Op::CLD, AddrMode::IMPLICIT};

    //CLI
    opcodes[0x58] = {Op::CLI, AddrMode::IMPLICIT};

    //CLV
    opcodes[0xB8] = {Op::CLV, AddrMode::IMPLICIT};

    //CMP
    opcodes[0xC9] = {Op::CMP, AddrMode::IMMEDIATE};
    opcodes[0xC5] = {Op::CMP, AddrMode::ZERO_PAGE};
    opcodes[0xD5] = {Op::CMP, AddrMode::ZERO_PAGE_X};
    opcodes[0xCD] = {Op::CMP, AddrMode::ABSOLUTE};
    opcodes[0xDD] = {Op::CMP, AddrMode::ABSOLUTE_X};
    opcodes[0xD9] = {Op::CMP, AddrMode::ABSOLUTE_Y};
    opcodes[0xC1] = {Op::CMP, AddrMode::INDEXED_INDIRECT};
    opcodes[0xD1] = {Op::CMP, AddrMode::INDIRECT_INDEXED};

    //CPX
    opcodes[0xE0] = {Op::CPX, AddrMode::IMMEDIATE};
    opcodes[0xE4] = {Op::CPX, AddrMode::ZERO_PAGE};
    opcodes[0xEC] = {Op::CPX, AddrMode::ABSOLUTE};

    //CPY
    opcodes[0xC0] = {Op::CPY, AddrMode::IMMEDIATE};
    opcodes[0xC4] = {Op::CPY, AddrMode::ZERO_PAGE};
    opcodes[0xCC] = {Op::CPY, AddrMode::ABSOLUTE};

    //DEC
    opcodes[0xC6] = {Op::DEC, AddrMode::ZERO_PAGE};
    opcodes[0xD6] = {Op::DEC, AddrMode::ZERO_PAGE_X};
    opcodes[0xCE] = {Op::DEC, AddrMode::ABSOLUTE};
    opcodes[0xDE] = {Op::DEC, AddrMode::ABSOLUTE_X};

    //DEX
    opcodes[0xCA] = {Op::DEX, AddrMode::IMPLICIT};

    //DEY
    opcodes[0x88] = {Op::DEY, AddrMode::IMPLICIT};

    //EOR
    opcodes[0x49] = {Op::EOR, AddrMode::IMMEDIATE};
    opcodes[0x45] = {Op::EOR, AddrMode::ZERO_PAGE};
    opcodes[0x55] = {Op::EOR, AddrMode::ZERO_PAGE_X};
    opcodes[0x4D] = {Op::EOR, AddrMode::ABSOLUTE};
    opcodes[0x5D] = {Op::EOR, AddrMode::ABSOLUTE_X};
    opcodes[0x59] = {Op::EOR, AddrMode::ABSOLUTE_Y};
    opcodes[0x41] = {Op::EOR, AddrMode::INDEXED_INDIRECT};
    opcodes[0x51] = {Op::EOR, AddrMode::INDIRECT_INDEXED};

    //INC
    opcodes[0xE6] = {Op::INC, AddrMode::ZERO_PAGE};
    opcodes[0xF6] = {Op::INC, AddrMode::ZERO_PAGE_X};
    opcodes[0xEE] = {Op::INC, AddrMode::ABSOLUTE};
    opcodes[0xFE] = {Op::INC, AddrMode::ABSOLUTE_X};

    //INX
    opcodes[0xE8] = {Op::INX, AddrMode::IMPLICIT};

    //INY
    opcodes[0xC8] = {Op::INY, AddrMode::IMPLICIT};

    //JMP
    opcodes[0x4C] = {Op::JMP, AddrMode::ABSOLUTE};
    opcodes[0x6C] = {Op::JMP, AddrMode::INDIRECT};

    //JSR
    opcodes[0x20] = {Op::JSR, AddrMode::ABSOLUTE};

    //LDA
    opcodes[0xA9] = {Op::LDA, AddrMode::IMMEDIATE};
    opcodes[0xA5] = {Op::LDA, AddrMode::ZERO_PAGE};
    opcodes[0xB5] = {Op::LDA, AddrMode::ZERO_PAGE_X};
    opcodes[0xAD] = {Op::LDA, AddrMode::ABSOLUTE};
    opcodes[0xBD] = {Op::LDA, AddrMode::ABSOLUTE_X};
    opcodes[0xB9] = {Op::LDA, AddrMode::ABSOLUTE_Y};
    opcodes[0xA1] = {Op::LDA, AddrMode::INDEXED_INDIRECT};
    opcodes[0xB1] = {Op::LDA, AddrMode::INDIRECT_INDEXED};

    //LDX
    opcodes[0xA2] = {Op::LDX, AddrMode::IMMEDIATE};
    opcodes[0xA6] = {Op::LDX, AddrMode::ZERO_PAGE};
    opcodes[0xB6] = {Op::LDX, AddrMode::ZERO_PAGE_X};
    opcodes[0xAE] = {Op::LDX, AddrMode::ABSOLUTE};
    opcodes[0xBE] = {Op::LDX, AddrMode::ABSOLUTE_X};

    //LDY
    opcodes[0xA0] = {Op::LDY, AddrMode::IMMEDIATE};
    opcodes[0xA4] = {Op::LDY, AddrMode::ZERO_PAGE};
    opcodes[0xB4] = {Op::LDY, AddrMode::ZERO_PAGE_X};
    opcodes[0xAC] = {Op::LDY, AddrMode::ABSOLUTE};
    opcodes[0xBC] = {Op::LDY, AddrMode::ABSOLUTE_X};

    //LSR
    opcodes[0x4A] = {Op::LSR, AddrMode::IMMEDIATE};
    opcodes[0x46] = {Op::LSR, AddrMode::ZERO_PAGE};
    opcodes[0x56] = {Op::LSR, AddrMode::ZERO_PAGE_X};
    opcodes[0x4E] = {Op::LSR, AddrMode::ABSOLUTE};
    opcodes[0x5E] = {Op::LSR, AddrMode::ABSOLUTE_X};

    //NOP
    opcodes[0xEA] = {Op::NOP, AddrMode::IMPLICIT};

    //ORA
    opcodes[0x09] = {Op::ORA, AddrMode::IMMEDIATE};
    opcodes[0x05] = {Op::ORA, AddrMode::ZERO_PAGE};
    opcodes[0x15] = {Op::ORA, AddrMode::ZERO_PAGE_X};
    opcodes[0x0D] = {Op::ORA, AddrMode::ABSOLUTE};
    opcodes[0x1D] = {Op::ORA, AddrMode::ABSOLUTE_X};
    opcodes[0x19] = {Op::ORA, AddrMode::ABSOLUTE_Y};
    opcodes[0x01] = {Op::ORA, AddrMode::INDEXED_INDIRECT};
    opcodes[0x11] = {Op::ORA, AddrMode::INDIRECT_INDEXED};

    //PHA
    opcodes[0x48] = {Op::PHA, AddrMode::IMPLICIT};

    //PHP
    opcodes[0x08] = {Op::PHP, AddrMode::IMPLICIT};

    //PLA
    opcodes[0x68] = {Op::PLA, AddrMode::IMPLICIT};

    //PLP
    opcodes[0x28] = {Op::PLP, AddrMode::IMPLICIT};

    //ROL
    opcodes[0x2A] = {Op::ROL, AddrMode::IMMEDIATE};
    opcodes[0x26] = {Op::ROL, AddrMode::ZERO_PAGE};
    opcodes[0x36] = {Op::ROL, AddrMode::ZERO_PAGE_X};
    opcodes[0x2E] = {Op::ROL, AddrMode::ABSOLUTE};
    opcodes[0x3E] = {Op::ROL, AddrMode::ABSOLUTE_X};

    //ROR
    opcodes[0x6A] = {Op::ROR, AddrMode::IMMEDIATE};
    opcodes[0x55] = {Op::ROR, AddrMode::ZERO_PAGE};
    opcodes[0x76] = {Op::ROR, AddrMode::ZERO_PAGE_X};
    opcodes[0x6E] = {Op::ROR, AddrMode::ABSOLUTE};
    opcodes[0x7E] = {Op::ROR, AddrMode::ABSOLUTE_X};

    //RTI
    opcodes[0x40] = {Op::RTI, AddrMode::IMPLICIT};

    //RTS
    opcodes[0x60] = {Op::RTS, AddrMode::IMPLICIT};

    //SBC
    opcodes[0xE9] = {Op::SBC, AddrMode::IMMEDIATE};
    opcodes[0xE5] = {Op::SBC, AddrMode::ZERO_PAGE};
    opcodes[0xF5] = {Op::SBC, AddrMode::ZERO_PAGE_X};
    opcodes[0xED] = {Op::SBC, AddrMode::ABSOLUTE};
    opcodes[0xFD] = {Op::SBC, AddrMode::ABSOLUTE_X};
    opcodes[0xF9] = {Op::SBC, AddrMode::ABSOLUTE_Y};
    opcodes[0xE1] = {Op::SBC, AddrMode::INDEXED_INDIRECT};
    opcodes[0xF1] = {Op::SBC, AddrMode::INDIRECT_INDEXED};

    //SEC
    opcodes[0x38] = {Op::SEC, AddrMode::IMPLICIT};

    //SED
    opcodes[0xF8] = {Op::SED, AddrMode::IMPLICIT};

    //SEI
    opcodes[0x78] = {Op::SEI, AddrMode::IMPLICIT};

    //STA
    opcodes[0x85] = {Op::STA, AddrMode::ZERO_PAGE};
    opcodes[0x95] = {Op::STA, AddrMode::ZERO_PAGE_X};
    opcodes[0x8D] = {Op::STA, AddrMode::ABSOLUTE};
    opcodes[0x9D] = {Op::STA, AddrMode::ABSOLUTE_X};
    opcodes[0x99] = {Op::STA, AddrMode::ABSOLUTE_Y};
    opcodes[0x81] = {Op::STA, AddrMode::INDEXED_INDIRECT};
    opcodes[0x91] = {Op::STA, AddrMode::INDIRECT_INDEXED};

    //STX
    opcodes[0x86] = {Op::STX, AddrMode::ZERO_PAGE};
    opcodes[0x96] = {Op::STX, AddrMode::ZERO_PAGE_X};
    opcodes[0x8E] = {Op::STX, AddrMode::ABSOLUTE};

    //STY
    opcodes[0x84] = {Op::STY, AddrMode::ZERO_PAGE};
    opcodes[0x94] = {Op::STY, AddrMode::ZERO_PAGE_X};
    opcodes[0x8C] = {Op::STY, AddrMode::ABSOLUTE};

    //TAX
    opcodes[0xAA] = {Op::TAX, AddrMode::IMPLICIT};

    //TAY
    opcodes[0xA8] = {Op::TAY, AddrMode::IMPLICIT};

    //TSX
    opcodes[0xBA] = {Op::TSX, AddrMode::IMPLICIT};

    //TXA
    opcodes[0x8A] = {Op::TXA, AddrMode::IMPLICIT};

    //TXS
    opcodes[0x9A] = {Op::TXS, AddrMode::IMPLICIT};

    //TYA
    opcodes[0x98] = {Op::TYA, AddrMode::IMPLICIT};
}

void Cpu::branchIf(uint16_t offsetAddr, bool condition)
{
    if (condition) {
        uint8_t data = mediator.read(offsetAddr);
        uint8_t PCL = static_cast<uint8_t>(PC);
        uint8_t result = PCL + data;

        PC += data;

        if (!(((PCL & 0x80) ^ (data & 0x80)) || ((PCL & 0x80) == (result & 0x80)))) {
            mediator.tick();
        }
        mediator.tick();
        mediator.tick();
    } else {
        ++PC;
        mediator.tick();
    }
}


//TODO : Check timing

uint16_t Cpu::implicitAM() const
{
    return 0;
}

uint16_t Cpu::accumulatorAM() const
{
    return 0;
}

uint16_t Cpu::immediateAM()
{
    return PC++;
}

uint16_t Cpu::zeroPageAM()
{
    return mediator.read(PC++);
}

uint16_t Cpu::zeroPageXAM()
{
    mediator.tick();
    mediator.tick();
    return static_cast<uint8_t>(mediator.read(PC++) + X);
}

uint16_t Cpu::zeroPageYAM()
{
    mediator.tick();
    mediator.tick();
    return static_cast<uint8_t>(mediator.read(PC++) + Y);
}

uint16_t Cpu::relativeAM()
{
    return mediator.read(PC++);
}

uint16_t Cpu::absoluteAM()
{
    mediator.tick();
    mediator.tick();
    return mediator.read(PC++) | (mediator.read(PC++) << 8);
}

uint16_t Cpu::absoluteXAM()
{
    uint16_t addr = mediator.read(PC++) | (mediator.read(PC++) << 8);

    mediator.tick();
    mediator.tick();
    if ((addr & 0x00FF) + X > 0xFF) {
        mediator.tick();
    }

    return addr + X;
}

uint16_t Cpu::absoluteYAM()
{
    uint16_t addr = mediator.read(PC++) | (mediator.read(PC++) << 8);

    mediator.tick();
    mediator.tick();
    if ((addr & 0x00FF) + Y > 0xFF) {
        mediator.tick();
    }

    return addr + Y;
}

uint16_t Cpu::indirectAM()
{
    uint8_t low = mediator.read(PC++);
    uint8_t high = mediator.read(PC++);
    uint8_t addrLow = mediator.read(low | (high << 8));
    uint8_t addrHigh = mediator.read(static_cast<uint8_t>(low + 1) | (high << 8)); //page wrapping

    mediator.tick();
    mediator.tick();
    mediator.tick();
    mediator.tick();

    return addrLow | (addrHigh << 8);
}

uint16_t Cpu::indexedIndirectAM()
{
    mediator.tick();
    mediator.tick();
    mediator.tick();
    mediator.tick();

    uint8_t base = mediator.read(PC++) + X;
    return mediator.read(base) | (mediator.read(static_cast<uint8_t>(base + 1)) << 8); //page wrapping
}

uint16_t Cpu::indirectIndexedAM()
{
    uint8_t base = mediator.read(PC++);
    uint16_t addr = mediator.read(base) | (mediator.read(static_cast<uint8_t>(base + 1)) << 8); //page wrapping

    mediator.tick();
    mediator.tick();
    mediator.tick();
    if ((addr & 0x00FF) + Y > 0xFF) {
        mediator.tick();
    }

    return addr + Y;
}

void Cpu::ADC(uint16_t addr)
{
    uint8_t d = mediator.read(addr);
    uint16_t result = A + d + (P.C ? 1 : 0);

    P.C = result > 0xFF;

    bool a = A & 0x80;
    bool b = (d + P.C) * 0x80;
    bool r = result * 0x80;
    P.V = (r & !(a | b)) | (!r & a & b);

    A = static_cast<uint8_t>(result);

    P.Z = A == 0;
    P.N = A & 0x80;

    mediator.tick();
}

void Cpu::AND(uint16_t addr)
{
    A &= mediator.read(addr);

    P.Z = A == 0;
    P.N = A & 0x80;

    mediator.tick();
}

void Cpu::ASL(uint16_t addr, AddrMode addrMode)
{
    uint8_t data = addrMode == AddrMode::ACCUMULATOR ? A : mediator.read(addr);
    uint8_t result = data << 1;

    P.C = data & 0x80;
    P.Z = result == 0;
    P.N = result & 0x80;

    if (addrMode == AddrMode::ACCUMULATOR) {
        A = result;
    } else {
        mediator.write(addr, result);
        mediator.tick();
    }
}

void Cpu::BCC(uint16_t addr)
{
    branchIf(addr, !P.C);
}

void Cpu::BCS(uint16_t addr)
{
    branchIf(addr, P.C);
}

void Cpu::BEQ(uint16_t addr)
{
    branchIf(addr, P.Z);
}

void Cpu::BIT(uint16_t addr)
{
    uint8_t result = A & mediator.read(addr);

    P.Z = result == 0;
    P.V = result & 0x40;
    P.N = result & 0x80;
}

void Cpu::BMI(uint16_t addr)
{
    branchIf(addr, P.N);
}

void Cpu::BNE(uint16_t addr)
{
    branchIf(addr, !P.Z);
}

void Cpu::BPL(uint16_t addr)
{
    branchIf(addr, !P.N);
}

void Cpu::BRK(uint16_t addr)
{
    ++PC;

    mediator.write(0x100 + S--, PC >> 8);
    mediator.write(0x100 + S--, PC);
    mediator.write(0x100 + S--, P.raw | 0x30);

    P.B = true;

    PC = mediator.read(0xFFFE) | (mediator.read(0xFFFF) << 8);

    mediator.tick();
    mediator.tick();
    mediator.tick();
    mediator.tick();
    mediator.tick();
    mediator.tick();
}

void Cpu::BVC(uint16_t addr)
{
    branchIf(addr, !P.V);
}

void Cpu::BVS(uint16_t addr)
{
    branchIf(addr, P.V);
}

void Cpu::CLC(uint16_t addr)
{
    P.C = false;
    mediator.tick();
}

void Cpu::CLD(uint16_t addr)
{
    P.D = false;
    mediator.tick();
}

void Cpu::CLI(uint16_t addr)
{
    P.I = false;
    mediator.tick();
}

void Cpu::CLV(uint16_t addr)
{
    P.V = false;
    mediator.tick();
}

void Cpu::CMP(uint16_t addr)
{
    uint8_t data = mediator.read(addr);
    uint8_t result = A - data;

    P.C = A >= data;
    P.Z = !result;
    P.N = result & 0x80;
}

void Cpu::CPX(uint16_t addr)
{
    uint8_t data = mediator.read(addr);
    uint8_t result = X - data;

    P.C = X >= data;
    P.Z = !result;
    P.N = result & 0x80;
}

void Cpu::CPY(uint16_t addr)
{
    uint8_t data = mediator.read(addr);
    uint8_t result = Y - data;

    P.C = Y >= data;
    P.Z = !result;
    P.N = result & 0x80;
}

void Cpu::DEC(uint16_t addr)
{
    uint8_t result = mediator.read(addr) - 1;
    mediator.write(addr, result);

    P.Z = result == 0;
    P.N = result & 0x80;

    mediator.tick();
}

void Cpu::DEX()
{
    --X;

    P.Z = X == 0;
    P.N = X & 0x80;

    mediator.tick();
}

void Cpu::DEY()
{
    --Y;

    P.Z = Y == 0;
    P.N = Y & 0x80;

    mediator.tick();
}

void Cpu::EOR(uint16_t addr)
{
    A ^= mediator.read(addr);

    P.Z = A == 0;
    P.N = A & 0x80;
}

void Cpu::INC(uint16_t addr)
{
    uint8_t result = mediator.read(addr) + 1;
    mediator.write(addr, result);

    P.Z = result == 0;
    P.N = result & 0x80;

    mediator.tick();
    mediator.tick();
}

void Cpu::INX()
{
    ++X;

    P.Z = X == 0;
    P.N = X & 0x80;

    mediator.tick();
}

void Cpu::INY()
{
    ++Y;

    P.Z = Y == 0;
    P.N = Y & 0x80;

    mediator.tick();
}

void Cpu::JMP(uint16_t addr)
{
    PC = addr;
}

void Cpu::JSR(uint16_t addr)
{
    mediator.write(0x100 + S--, PC >> 8);
    mediator.write(0x100 + S--, PC);

    PC = addr;

    mediator.tick();
    mediator.tick();
    mediator.tick();
}

void Cpu::LDA(uint16_t addr)
{
    A = mediator.read(addr);

    P.Z = A == 0;
    P.N = A & 0x80;
}

void Cpu::LDX(uint16_t addr)
{
    X = mediator.read(addr);

    P.Z = X == 0;
    P.N = X & 0x80;
}

void Cpu::LDY(uint16_t addr)
{
    Y = mediator.read(addr);

    P.Z = Y == 0;
    P.N = Y & 0x80;
}

void Cpu::LSR(uint16_t addr, AddrMode addrMode)
{
    uint8_t data = addrMode == AddrMode::ACCUMULATOR ? A : mediator.read(addr);
    uint8_t result = data >> 1;

    P.C = data & 0x01;
    P.Z = result == 0;
    P.N = result & 0x80;

    if (addrMode == AddrMode::ACCUMULATOR) {
        A = result;
    } else {
        mediator.write(addr, result);
        mediator.tick();
    }

    mediator.tick();
}

void Cpu::NOP(uint16_t addr)
{
    mediator.tick();
}

void Cpu::ORA(uint16_t addr)
{
    uint8_t result = A | mediator.read(addr);
    mediator.write(addr, result);

    P.Z = result == 0;
    P.N = result & 0x80;
}

void Cpu::PHA()
{
    mediator.write(0x100 + S--, A);

    mediator.tick();
    mediator.tick();
}

void Cpu::PHP()
{
    mediator.write(0x100 + S--, P.raw | 0x30);

    mediator.tick();
    mediator.tick();
}

void Cpu::PLA()
{
    A = mediator.read(0x100 + (++S));

    P.Z = A == 0;
    P.N = A & 0x80;

    mediator.tick();
    mediator.tick();
    mediator.tick();
}

void Cpu::PLP()
{
    P.raw = mediator.read(0x100 + (++S));

    mediator.tick();
    mediator.tick();
    mediator.tick();
}

void Cpu::ROL(uint16_t addr, AddrMode addrMode)
{
    uint8_t data = addrMode == AddrMode::ACCUMULATOR ? A : mediator.read(addr);
    uint8_t carry = P.C ? 1 : 0;
    P.C = data & 0x80;
    uint8_t result = data << 1 + carry;

    P.Z = result == 0;
    P.N = result & 0x80;

    if (addrMode == AddrMode::ACCUMULATOR) {
        A = result;
    } else {
        mediator.write(addr, result);
        mediator.tick();
    }

    mediator.tick();
}

void Cpu::ROR(uint16_t addr, AddrMode addrMode)
{
    uint8_t data = addrMode == AddrMode::ACCUMULATOR ? A : mediator.read(addr);
    uint8_t carry = (P.C ? 1 : 0) << 7;
    P.C = data & 0x80;
    uint8_t result = data >> 1 + carry;

    P.Z = result == 0;
    P.N = result & 0x80;

    if (addrMode == AddrMode::ACCUMULATOR) {
        A = result;
    } else {
        mediator.write(addr, result);
        mediator.tick();
    }

    mediator.tick();
}

void Cpu::RTI()
{
    P.raw = mediator.read(0x100 + (++S));
    PC = mediator.read(0x100 + (++S)) | (mediator.read(0x100 + (++S)) << 8);

    mediator.tick();
    mediator.tick();
    mediator.tick();
    mediator.tick();
    mediator.tick();
}

void Cpu::RTS()
{
    PC = mediator.read(0x100 + (++S)) | (mediator.read(0x100 + (++S)) << 8) + 1;

    mediator.tick();
    mediator.tick();
    mediator.tick();
    mediator.tick();
    mediator.tick();
}

void Cpu::SBS(uint16_t addr)
{
    uint16_t result = A - mediator.read(addr) - (P.C ? 0 : 1);
    uint8_t temp = -result - (P.C ? 0 : 1);

    bool a = A & 0x80;
    bool b = temp & 0x80;
    bool r = result & 0x80;

    P.V = ((r & !(a | b)) | (!r & a & b));

    A = static_cast<uint8_t>(result);

    P.Z = A == 0;
    P.N = A & 0x80;
    P.C = result > 0xFF;
}

void Cpu::SEC()
{
    P.C = true;
    mediator.tick();
}

void Cpu::SED()
{
    P.D = true;
    mediator.tick();
}

void Cpu::SEI()
{
    P.I = true;
    mediator.tick();
}

void Cpu::STA(uint16_t addr)
{
    mediator.write(addr, A);
    mediator.tick();
}

void Cpu::STX(uint16_t addr)
{
    mediator.write(addr, X);
    mediator.tick();
}

void Cpu::STY(uint16_t addr)
{
    mediator.write(addr, Y);
    mediator.tick();
}

void Cpu::TAX()
{
    X = A;

    P.Z = X == 0;
    P.N = X & 0x80;

    mediator.tick();
}

void Cpu::TAY()
{
    Y = A;

    P.Z = Y == 0;
    P.N = Y & 0x80;

    mediator.tick();
}

void Cpu::TSX()
{
    X = S;

    P.Z = X == 0;
    P.N = X & 0x80;

    mediator.tick();
}

void Cpu::TXA()
{
    A = X;

    P.Z = A == 0;
    P.N = A & 0x80;

    mediator.tick();
}

void Cpu::TXS()
{
    X = S;

    mediator.tick();
}

void Cpu::TYA()
{
    A = Y;

    P.Z = A == 0;
    P.N = A & 0x80;

    mediator.tick();
}
