#pragma once

#include "Types.hpp"

namespace cpu
{

    enum class Opcode : opcode_t
    {
        NOP = 0x00,
        MOV = 0x01,
        ADD = 0x02,
        SUB = 0x03,
        MUL = 0x04,
        DIV = 0x05,
        AND = 0x06,
        OR  = 0x07,
        XOR = 0x08,
        NOT = 0x09,
        JMP = 0x0A,
        JZ  = 0x0B,
        JNZ = 0x0C,
        CALL= 0x0D,
        RET = 0x0E,
        HLT = 0x0F
    };

    inline const char* OpcodeToString(Opcode opcode)
    {
        switch (opcode)
        {
            case Opcode::NOP: return "NOP";
            case Opcode::MOV: return "MOV";
            case Opcode::ADD: return "ADD";
            case Opcode::SUB: return "SUB";
            case Opcode::MUL: return "MUL";
            case Opcode::DIV: return "DIV";
            case Opcode::AND: return "AND";
            case Opcode::OR:  return "OR";
            case Opcode::XOR: return "XOR";
            case Opcode::NOT: return "NOT";
            case Opcode::JMP: return "JMP";
            case Opcode::JZ:  return "JZ";
            case Opcode::JNZ: return "JNZ";
            case Opcode::CALL:return "CALL";
            case Opcode::RET: return "RET";
            case Opcode::HLT: return "HLT";
            default: return "UNKNOWN";
        }
    }
}