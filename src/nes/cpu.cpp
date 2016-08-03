#include "cpu.h"

#include <iostream>
#include <cassert>

Cpu::Cpu()
    : PC(0xC000), A(0), X(0), Y(0), S(0xFD), cycleCount(0), resetSignal(true), logger("cpu_log.txt")
{
    P.raw = 0x34;

    generateOpcodes();
}

void Cpu::tick()
{
    if (resetSignal) {
        resetSignal = false;

        //TODO: Reset PPU

        PC = bus->read(0xFFFC) | (bus->read(0xFFFD) << 8);

        bus->tick();
        bus->tick();
        bus->tick();
        bus->tick();
        bus->tick();
        bus->tick();

        return;
    }

    logger.setPC(PC);
    uint8_t opId = bus->read(PC++);
    auto opcode = opcodes[opId];

    logger.addMemLocation(opId);
    logger.setOpcode(opcode);
    logger.setRegisters(A, X, Y, S, P);

    uint16_t addr = getAddress(opcode.addrMode);
    executeOp(addr, opcode);

    logger.finishInstruction();
    ++cycleCount;

    bus->tick();
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
        SBC(addr);
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
        std::cout << "Bad opcode enum : " << static_cast<int>(opcode.op) << std::endl;
        assert(false);
        break;
    }
}

uint16_t Cpu::getAddress(AddrMode addrMode)
{
    uint16_t addr = 0x00;

    switch (addrMode) {
    case AddrMode::IMPLICIT:
        addr = implicitAM();
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
        assert(false);
        break;
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

    //BCC
    opcodes[0x90] = {Op::BCC, AddrMode::RELATIVE};

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
    opcodes[0xB6] = {Op::LDX, AddrMode::ZERO_PAGE_Y};
    opcodes[0xAE] = {Op::LDX, AddrMode::ABSOLUTE};
    opcodes[0xBE] = {Op::LDX, AddrMode::ABSOLUTE_Y};

    //LDY
    opcodes[0xA0] = {Op::LDY, AddrMode::IMMEDIATE};
    opcodes[0xA4] = {Op::LDY, AddrMode::ZERO_PAGE};
    opcodes[0xB4] = {Op::LDY, AddrMode::ZERO_PAGE_X};
    opcodes[0xAC] = {Op::LDY, AddrMode::ABSOLUTE};
    opcodes[0xBC] = {Op::LDY, AddrMode::ABSOLUTE_X};

    //LSR
    opcodes[0x4A] = {Op::LSR, AddrMode::ACCUMULATOR};
    opcodes[0x46] = {Op::LSR, AddrMode::ZERO_PAGE};
    opcodes[0x56] = {Op::LSR, AddrMode::ZERO_PAGE_X};
    opcodes[0x4E] = {Op::LSR, AddrMode::ABSOLUTE};
    opcodes[0x5E] = {Op::LSR, AddrMode::ABSOLUTE_X};

    //NOP
    opcodes[0xEA] = {Op::NOP, AddrMode::IMPLICIT};
    opcodes[0x04] = {Op::NOP, AddrMode::ZERO_PAGE};
    opcodes[0x0C] = {Op::NOP, AddrMode::ABSOLUTE};
    opcodes[0x14] = {Op::NOP, AddrMode::ZERO_PAGE_X};
    opcodes[0x1A] = {Op::NOP, AddrMode::IMPLICIT};
    opcodes[0x1C] = {Op::NOP, AddrMode::ABSOLUTE_X};
    opcodes[0x34] = {Op::NOP, AddrMode::ZERO_PAGE_X};
    opcodes[0x3A] = {Op::NOP, AddrMode::IMPLICIT};
    opcodes[0x3C] = {Op::NOP, AddrMode::ABSOLUTE_X};
    opcodes[0x44] = {Op::NOP, AddrMode::ZERO_PAGE};
    opcodes[0x54] = {Op::NOP, AddrMode::ZERO_PAGE_X};
    opcodes[0x5A] = {Op::NOP, AddrMode::IMPLICIT};
    opcodes[0x5C] = {Op::NOP, AddrMode::ABSOLUTE_X};
    opcodes[0x64] = {Op::NOP, AddrMode::ZERO_PAGE};
    opcodes[0x74] = {Op::NOP, AddrMode::ZERO_PAGE_X};
    opcodes[0x7A] = {Op::NOP, AddrMode::IMPLICIT};
    opcodes[0x7C] = {Op::NOP, AddrMode::ABSOLUTE_X};
    opcodes[0x80] = {Op::NOP, AddrMode::IMMEDIATE};
    opcodes[0x82] = {Op::NOP, AddrMode::IMMEDIATE};
    opcodes[0x89] = {Op::NOP, AddrMode::IMMEDIATE};
    opcodes[0xC2] = {Op::NOP, AddrMode::IMMEDIATE};
    opcodes[0xD4] = {Op::NOP, AddrMode::ZERO_PAGE_X};
    opcodes[0xDA] = {Op::NOP, AddrMode::IMPLICIT};
    opcodes[0xDC] = {Op::NOP, AddrMode::ABSOLUTE_X};
    opcodes[0xE2] = {Op::NOP, AddrMode::IMMEDIATE};
    opcodes[0xEA] = {Op::NOP, AddrMode::IMPLICIT};
    opcodes[0xF4] = {Op::NOP, AddrMode::ZERO_PAGE_X};
    opcodes[0xFA] = {Op::NOP, AddrMode::IMPLICIT};
    opcodes[0xFC] = {Op::NOP, AddrMode::ABSOLUTE_X};

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
    opcodes[0x2A] = {Op::ROL, AddrMode::ACCUMULATOR};
    opcodes[0x26] = {Op::ROL, AddrMode::ZERO_PAGE};
    opcodes[0x36] = {Op::ROL, AddrMode::ZERO_PAGE_X};
    opcodes[0x2E] = {Op::ROL, AddrMode::ABSOLUTE};
    opcodes[0x3E] = {Op::ROL, AddrMode::ABSOLUTE_X};

    //ROR
    opcodes[0x6A] = {Op::ROR, AddrMode::ACCUMULATOR};
    opcodes[0x66] = {Op::ROR, AddrMode::ZERO_PAGE};
    opcodes[0x76] = {Op::ROR, AddrMode::ZERO_PAGE_X};
    opcodes[0x6E] = {Op::ROR, AddrMode::ABSOLUTE};
    opcodes[0x7E] = {Op::ROR, AddrMode::ABSOLUTE_X};

    //RTI
    opcodes[0x40] = {Op::RTI, AddrMode::IMPLICIT};

    //RTS
    opcodes[0x60] = {Op::RTS, AddrMode::IMPLICIT};

    //SBC
    opcodes[0xE9] = {Op::SBC, AddrMode::IMMEDIATE};
    opcodes[0xEB] = {Op::SBC, AddrMode::IMMEDIATE};
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
    opcodes[0x96] = {Op::STX, AddrMode::ZERO_PAGE_Y};
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
        uint8_t data = bus->read(offsetAddr);
        uint8_t PCL = static_cast<uint8_t>(PC);
        uint8_t result = PCL + data;

        PC += static_cast<int8_t>(data); //Offset is SIGNED

        if (!(((PCL & 0x80) ^ (data & 0x80)) || ((PCL & 0x80) == (result & 0x80)))) {
            bus->tick();
        }
        bus->tick();
        bus->tick();
    } else {
        bus->tick();
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
    //TODO : This is no good. I don't think the call to read() could be optimized away by an empty
    //version of the logger. There shouldn't be any side effects from the read as PC should be in ROM.
    logger.addMemLocation(bus->read(PC));
    return PC++;
}

uint16_t Cpu::zeroPageAM()
{
    uint8_t addr = bus->read(PC++);
    logger.addMemLocation(addr);
    return addr;
}

uint16_t Cpu::zeroPageXAM()
{
    bus->tick();
    bus->tick();
    uint8_t mem = bus->read(PC++);
    logger.addMemLocation(mem);
    return static_cast<uint8_t>(mem + X);
}

uint16_t Cpu::zeroPageYAM()
{
    bus->tick();
    bus->tick();
    uint8_t mem = bus->read(PC++);
    logger.addMemLocation(mem);
    return static_cast<uint8_t>(mem + Y);
}

uint16_t Cpu::relativeAM()
{
    //TODO : This is no good. I don't think the call to read() could be optimized away by an empty
    //version of the logger. There shouldn't be any side effects from the read as PC should be in ROM.
    logger.addMemLocation(bus->read(PC));
    return PC++;
}

uint16_t Cpu::absoluteAM()
{
    bus->tick();
    bus->tick();

    uint8_t low = bus->read(PC++);
    uint8_t high = bus->read(PC++);

    logger.addMemLocation(low);
    logger.addMemLocation(high);

    return low | (high << 8);
}

uint16_t Cpu::absoluteXAM()
{
    uint8_t low = bus->read(PC++);
    uint8_t high = bus->read(PC++);
    uint16_t addr = low | (high << 8);


    logger.addMemLocation(low);
    logger.addMemLocation(high);

    bus->tick();
    bus->tick();
    if ((addr & 0x00FF) + X > 0xFF) {
        bus->read(addr + X - 0x100); //Dummy read
        bus->tick();
    }

    return addr + X;
}

uint16_t Cpu::absoluteYAM()
{
    uint8_t low = bus->read(PC++);
    uint8_t high = bus->read(PC++);
    uint16_t addr = low | (high << 8);


    logger.addMemLocation(low);
    logger.addMemLocation(high);

    bus->tick();
    bus->tick();
    if ((addr & 0x00FF) + Y > 0xFF) {
        bus->read(addr + Y - 0x100); //Dummy read
        bus->tick();
    }

    return addr + Y;
}

uint16_t Cpu::indirectAM()
{
    uint8_t low = bus->read(PC++);
    uint8_t high = bus->read(PC++);
    uint8_t addrLow = bus->read(low | (high << 8));
    uint8_t addrHigh = bus->read(static_cast<uint8_t>(low + 1) | (high << 8)); //page wrapping

    logger.addMemLocation(low);
    logger.addMemLocation(high);

    bus->tick();
    bus->tick();
    bus->tick();
    bus->tick();

    return addrLow | (addrHigh << 8);
}

uint16_t Cpu::indexedIndirectAM()
{
    bus->tick();
    bus->tick();
    bus->tick();
    bus->tick();

    uint8_t offset = bus->read(PC++);
    uint8_t base = offset + X;

    logger.addMemLocation(offset);

    return bus->read(base) | (bus->read(static_cast<uint8_t>(base + 1)) << 8); //page wrapping
}

uint16_t Cpu::indirectIndexedAM()
{
    uint8_t base = bus->read(PC++);
    uint16_t addr = bus->read(base) | (bus->read(static_cast<uint8_t>(base + 1)) << 8); //page wrapping

    logger.addMemLocation(base);

    bus->tick();
    bus->tick();
    bus->tick();
    if ((addr & 0x00FF) + Y > 0xFF) {
        bus->read(addr + Y - 0x100); //Dummy read
        bus->tick();
    }

    return addr + Y;
}

void Cpu::ADC(uint16_t addr)
{
    uint8_t d = bus->read(addr);
    uint16_t result = A + d + (P.C ? 1 : 0);

    P.C = result > 0xFF;

    bool a = A & 0x80;
    bool b = (d + (P.C ? 1 : 0)) & 0x80;
    bool r = result & 0x80;
    P.V = (r & !(a | b)) | (!r & a & b);

    A = static_cast<uint8_t>(result);

    P.Z = A == 0;
    P.N = A & 0x80;

    bus->tick();
}

void Cpu::AND(uint16_t addr)
{
    A &= bus->read(addr);

    P.Z = A == 0;
    P.N = A & 0x80;

    bus->tick();
}

void Cpu::ASL(uint16_t addr, AddrMode addrMode)
{
    uint8_t data = addrMode == AddrMode::ACCUMULATOR ? A : bus->read(addr);
    uint8_t result = data << 1;

    P.C = data & 0x80;
    P.Z = result == 0;
    P.N = result & 0x80;

    if (addrMode == AddrMode::ACCUMULATOR) {
        A = result;
    } else {
        bus->write(addr, result);
        bus->tick();
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
    uint8_t mem = bus->read(addr);
    uint8_t result = A & mem;

    P.Z = result == 0;
    P.V = mem & 0x40;
    P.N = mem & 0x80;
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

    bus->write(0x100 + S--, PC >> 8);
    bus->write(0x100 + S--, PC);
    bus->write(0x100 + S--, P.raw | 0x30);

    P.B = true;

    PC = bus->read(0xFFFE) | (bus->read(0xFFFF) << 8);

    bus->tick();
    bus->tick();
    bus->tick();
    bus->tick();
    bus->tick();
    bus->tick();
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
    bus->tick();
}

void Cpu::CLD(uint16_t addr)
{
    P.D = false;
    bus->tick();
}

void Cpu::CLI(uint16_t addr)
{
    P.I = false;
    bus->tick();
}

void Cpu::CLV(uint16_t addr)
{
    P.V = false;
    bus->tick();
}

void Cpu::CMP(uint16_t addr)
{
    uint8_t data = bus->read(addr);
    uint8_t result = A - data;

    P.C = A >= data;
    P.Z = !result;
    P.N = result & 0x80;
}

void Cpu::CPX(uint16_t addr)
{
    uint8_t data = bus->read(addr);
    uint8_t result = X - data;

    P.C = X >= data;
    P.Z = !result;
    P.N = result & 0x80;
}

void Cpu::CPY(uint16_t addr)
{
    uint8_t data = bus->read(addr);
    uint8_t result = Y - data;

    P.C = Y >= data;
    P.Z = !result;
    P.N = result & 0x80;
}

void Cpu::DEC(uint16_t addr)
{
    uint8_t result = bus->read(addr) - 1;
    bus->write(addr, result);

    P.Z = result == 0;
    P.N = result & 0x80;

    bus->tick();
}

void Cpu::DEX()
{
    --X;

    P.Z = X == 0;
    P.N = X & 0x80;

    bus->tick();
}

void Cpu::DEY()
{
    --Y;

    P.Z = Y == 0;
    P.N = Y & 0x80;

    bus->tick();
}

void Cpu::EOR(uint16_t addr)
{
    A ^= bus->read(addr);

    P.Z = A == 0;
    P.N = A & 0x80;
}

void Cpu::INC(uint16_t addr)
{
    uint8_t result = bus->read(addr) + 1;
    bus->write(addr, result);

    P.Z = result == 0;
    P.N = result & 0x80;

    bus->tick();
    bus->tick();
}

void Cpu::INX()
{
    ++X;

    P.Z = X == 0;
    P.N = X & 0x80;

    bus->tick();
}

void Cpu::INY()
{
    ++Y;

    P.Z = Y == 0;
    P.N = Y & 0x80;

    bus->tick();
}

void Cpu::JMP(uint16_t addr)
{
    PC = addr;
}

void Cpu::JSR(uint16_t addr)
{
    bus->write(0x100 + S--, (PC-1) >> 8);
    bus->write(0x100 + S--, (PC-1));

    PC = addr;

    bus->tick();
    bus->tick();
    bus->tick();
}

void Cpu::LDA(uint16_t addr)
{
    A = bus->read(addr);

    P.Z = A == 0;
    P.N = A & 0x80;
}

void Cpu::LDX(uint16_t addr)
{
    X = bus->read(addr);

    P.Z = X == 0;
    P.N = X & 0x80;
}

void Cpu::LDY(uint16_t addr)
{
    Y = bus->read(addr);

    P.Z = Y == 0;
    P.N = Y & 0x80;
}

void Cpu::LSR(uint16_t addr, AddrMode addrMode)
{
    uint8_t data = addrMode == AddrMode::ACCUMULATOR ? A : bus->read(addr);
    uint8_t result = data >> 1;

    P.C = data & 0x01;
    P.Z = result == 0;
    P.N = result & 0x80;

    if (addrMode == AddrMode::ACCUMULATOR) {
        A = result;
    } else {
        bus->write(addr, result);
        bus->tick();
    }

    bus->tick();
}

void Cpu::NOP(uint16_t addr)
{
    bus->tick();
}

void Cpu::ORA(uint16_t addr)
{
    A |= bus->read(addr);

    P.Z = A == 0;
    P.N = A & 0x80;
}

void Cpu::PHA()
{
    bus->write(0x100 + S--, A);

    bus->tick();
    bus->tick();
}

void Cpu::PHP()
{
    bus->write(0x100 + S--, P.raw | 0x30);

    bus->tick();
    bus->tick();
}

void Cpu::PLA()
{
    A = bus->read(0x100 + (++S));

    P.Z = A == 0;
    P.N = A & 0x80;

    bus->tick();
    bus->tick();
    bus->tick();
}

void Cpu::PLP()
{
    //Bits 4 and 5 are ignored
    P.raw = (P.raw & 0x30) | (bus->read(0x100 + (++S)) & 0xCF);

    bus->tick();
    bus->tick();
    bus->tick();
}

void Cpu::ROL(uint16_t addr, AddrMode addrMode)
{
    uint8_t data = addrMode == AddrMode::ACCUMULATOR ? A : bus->read(addr);
    uint8_t carry = P.C ? 1 : 0;
    P.C = data & 0x80;
    uint8_t result = (data << 1) + carry;

    P.Z = result == 0;
    P.N = result & 0x80;

    if (addrMode == AddrMode::ACCUMULATOR) {
        A = result;
    } else {
        bus->write(addr, result);
        bus->tick();
    }

    bus->tick();
}

void Cpu::ROR(uint16_t addr, AddrMode addrMode)
{
    uint8_t data = addrMode == AddrMode::ACCUMULATOR ? A : bus->read(addr);
    uint8_t carry = (P.C ? 1 : 0) << 7;
    P.C = data & 0x01;
    uint8_t result = (data >> 1) + carry;

    P.Z = result == 0;
    P.N = result & 0x80;

    if (addrMode == AddrMode::ACCUMULATOR) {
        A = result;
    } else {
        bus->write(addr, result);
        bus->tick();
    }

    bus->tick();
}

void Cpu::RTI()
{
    P.raw = (P.raw & 0x30) | (bus->read(0x100 + (++S)) & 0xCF);
    PC = bus->read(0x100 + (++S));
    PC |= bus->read(0x100 + (++S)) << 8;

    bus->tick();
    bus->tick();
    bus->tick();
    bus->tick();
    bus->tick();
}

void Cpu::RTS()
{
    PC = bus->read(0x100 + (++S));
    PC |= (bus->read(0x100 + (++S)) << 8);
    ++PC;

    bus->tick();
    bus->tick();
    bus->tick();
    bus->tick();
    bus->tick();
}

void Cpu::SBC(uint16_t addr)
{
    uint8_t mem = bus->read(addr);
    uint16_t result = A - mem - (P.C ? 0 : 1);
    uint8_t temp = -mem - (P.C ? 0 : 1);

    bool a = A & 0x80;
    bool b = temp & 0x80;
    bool r = result & 0x80;

    P.V = ((r & !(a | b)) | (!r & a & b));

    A = static_cast<uint8_t>(result);

    P.Z = A == 0;
    P.N = A & 0x80;
    P.C = result <= 0xFF;
}

void Cpu::SEC()
{
    P.C = true;
    bus->tick();
}

void Cpu::SED()
{
    P.D = true;
    bus->tick();
}

void Cpu::SEI()
{
    P.I = true;
    bus->tick();
}

void Cpu::STA(uint16_t addr)
{
    bus->write(addr, A);
    bus->tick();
}

void Cpu::STX(uint16_t addr)
{
    bus->write(addr, X);
    bus->tick();
}

void Cpu::STY(uint16_t addr)
{
    bus->write(addr, Y);
    bus->tick();
}

void Cpu::TAX()
{
    X = A;

    P.Z = X == 0;
    P.N = X & 0x80;

    bus->tick();
}

void Cpu::TAY()
{
    Y = A;

    P.Z = Y == 0;
    P.N = Y & 0x80;

    bus->tick();
}

void Cpu::TSX()
{
    X = S;

    P.Z = X == 0;
    P.N = X & 0x80;

    bus->tick();
}

void Cpu::TXA()
{
    A = X;

    P.Z = A == 0;
    P.N = A & 0x80;

    bus->tick();
}

void Cpu::TXS()
{
    S = X;

    bus->tick();
}

void Cpu::TYA()
{
    A = Y;

    P.Z = A == 0;
    P.N = A & 0x80;

    bus->tick();
}
