#include "cpulogger.h"

#include <iomanip>
#include <sstream>

CpuLogger::CpuLogger(const std::string& filename)
    : lineCount(0), file(filename)
{

}

void CpuLogger::finishInstruction()
{
    ++lineCount;

    if (lineCount > 50000) {
        return;
    }

    file << std::uppercase << std::hex;
    file << std::setfill('0') << std::setw(4) << PC << "  ";

    {
        std::ostringstream oss;
        oss << std::uppercase << std::hex << std::setfill('0');
        for (auto memLoc : memLocations) {
            oss << std::setw(2) << static_cast<int>(memLoc) << " ";
        }
        file << std::setw(10) << std::left << std::setfill(' ') << oss.str();
    }

    file << getOpName() << " " << std::setw(28) << getAddrStr()
         << std::setfill('0') << std::right << "A:" << std::setw(2) << A
         << " X:" << std::setw(2) << X
         << " Y:" << std::setw(2) << Y
         << " P:" << std::setw(2) << static_cast<int>(P.raw) << " S:" << S;

    file << std::endl;

    memLocations.clear();
}

std::string CpuLogger::getOpName() const
{
    switch (opcode.op) {
    case Op::ADC:
        return "ADC";
        break;
    case Op::AND:
        return "AND";
        break;
    case Op::ASL:
        return "ASL";
        break;
    case Op::BCC:
        return "BCC";
        break;
    case Op::BCS:
        return "BCS";
        break;
    case Op::BEQ:
        return "BEQ";
        break;
    case Op::BIT:
        return "BIT";
        break;
    case Op::BMI:
        return "BMI";
        break;
    case Op::BNE:
        return "BNE";
        break;
    case Op::BPL:
        return "BPL";
        break;
    case Op::BRK:
        return "BRK";
        break;
    case Op::BVC:
        return "BVC";
        break;
    case Op::BVS:
        return "BVS";
        break;
    case Op::CLC:
        return "CLC";
        break;
    case Op::CLD:
        return "CLD";
        break;
    case Op::CLI:
        return "CLI";
        break;
    case Op::CLV:
        return "CLV";
        break;
    case Op::CMP:
        return "CMP";
        break;
    case Op::CPX:
        return "CPX";
        break;
    case Op::CPY:
        return "CPY";
        break;
    case Op::DEC:
        return "DEC";
        break;
    case Op::DEX:
        return "DEX";
        break;
    case Op::DEY:
        return "DEY";
        break;
    case Op::EOR:
        return "EOR";
        break;
    case Op::INC:
        return "INC";
        break;
    case Op::INX:
        return "INX";
        break;
    case Op::INY:
        return "INY";
        break;
    case Op::JMP:
        return "JMP";
        break;
    case Op::JSR:
        return "JSR";
        break;
    case Op::LDA:
        return "LDA";
        break;
    case Op::LDX:
        return "LDX";
        break;
    case Op::LDY:
        return "LDY";
        break;
    case Op::LSR:
        return "LSR";
        break;
    case Op::NOP:
        return "NOP";
        break;
    case Op::ORA:
        return "ORA";
        break;
    case Op::PHA:
        return "PHA";
        break;
    case Op::PHP:
        return "PHP";
        break;
    case Op::PLA:
        return "PLA";
        break;
    case Op::PLP:
        return "PLP";
        break;
    case Op::ROL:
        return "ROL";
        break;
    case Op::ROR:
        return "ROR";
        break;
    case Op::RTI:
        return "RTI";
        break;
    case Op::RTS:
        return "RTS";
        break;
    case Op::SBC:
        return "SBC";
        break;
    case Op::SEC:
        return "SEC";
        break;
    case Op::SED:
        return "SED";
        break;
    case Op::SEI:
        return "SEI";
        break;
    case Op::STA:
        return "STA";
        break;
    case Op::STX:
        return "STX";
        break;
    case Op::STY:
        return "STY";
        break;
    case Op::TAX:
        return "TAX";
        break;
    case Op::TAY:
        return "TAY";
        break;
    case Op::TSX:
        return "TSX";
        break;
    case Op::TXA:
        return "TXA";
        break;
    case Op::TXS:
        return "TXS";
        break;
    case Op::TYA:
        return "TYA";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}

std::string CpuLogger::getAddrStr() const
{
    std::ostringstream oss;
    oss << std::setfill('0');

    switch (opcode.addrMode) {
    case AddrMode::IMPLICIT:
    case AddrMode::ACCUMULATOR:
        break;
    case AddrMode::IMMEDIATE:
        oss << "#$" << std::hex << std::setw(2) << static_cast<int>(memLocations[1]);
        break;
    case AddrMode::ZERO_PAGE:
        oss << "$" << std::hex << std::setw(2) << static_cast<int>(memLocations[1]);
        break;
    case AddrMode::ZERO_PAGE_X:
        oss << "$" << std::hex << std::setw(2) << static_cast<int>(memLocations[1])
            << ",X @ " << std::setw(2) << (X + memLocations[1]);
        break;
    case AddrMode::ZERO_PAGE_Y:
        oss << "$" << std::hex << std::setw(2) << static_cast<int>(memLocations[1])
            << ",Y @ " << std::setw(2) << (Y + memLocations[1]);
        break;
    case AddrMode::RELATIVE:
        oss << "";
        break;
    case AddrMode::ABSOLUTE:
        oss << "";
        break;
    case AddrMode::ABSOLUTE_X:
        oss << "";
        break;
    case AddrMode::ABSOLUTE_Y:
        oss << "";
        break;
    case AddrMode::INDIRECT:
        oss << "";
        break;
    case AddrMode::INDEXED_INDIRECT:
        oss << "";
        break;
    case AddrMode::INDIRECT_INDEXED:
        oss << "";
        break;
    default:
        oss << "UNKNOWN";
        break;
    }

    return oss.str();
}
