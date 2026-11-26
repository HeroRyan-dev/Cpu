#pragma once

#include "cpu/isa/Opcode.hpp"
#include "cpu/Types.hpp"

namespace cpu::isa
{
    struct Instruction
    {
        Opcode opcode{};
        types::register_id_t operand1{};
        types::register_id_t operand2{};
    };
}
