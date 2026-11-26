#pragma once

namespace cpu::errors
{
    enum class RegisterError
    {
        NAME_NOT_FOUND,
        ID_NOT_FOUND,
        NULL_REGISTER,
        FILE_FULL,
        DUPLICATE_NAME,
        ALREADY_INCLUDED,
        INVALID
    };
}
