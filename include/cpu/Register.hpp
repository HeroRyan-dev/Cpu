#pragma once

#include "Types.hpp"

namespace cpu
{
    class Register
    {
    public:
        constexpr Register() = default;

        constexpr explicit Register(qword mask)
            : mask_(mask)
        {}

        constexpr Register& operator=(qword value)
        {
            value_ = value & mask_;
            return *this;
        }

        constexpr Register& operator=(const Register& other)
        {
            return *this = static_cast<qword>(other);
        }

        [[nodiscard]]
        constexpr operator qword() const
        {
            return value_;
        }

        [[nodiscard]]
        constexpr qword getMask() const
        {
            return mask_;
        }

        constexpr void reset()
        {
            value_ = 0;
        }

    private:
        qword mask_ = ~qword{0};
        qword value_{};
    };
}