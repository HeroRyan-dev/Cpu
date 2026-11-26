#pragma once

namespace cpu::errors
{
    enum class StackError
    {
        Overflow,
        Underflow,
        InvalidStackPointer
    };
}
