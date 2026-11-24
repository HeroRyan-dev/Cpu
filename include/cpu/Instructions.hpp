#pragma once

#include "Opcode.hpp"
#include "Types.hpp"

namespace cpu
{
    struct Instruction
    {
        cpu::Opcode opcode{};
        register_id_t operand1{};
        register_id_t operand2{};
    };





}





