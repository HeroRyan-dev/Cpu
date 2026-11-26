#pragma once

#include <array>
#include <cstdint>
#include <string_view>
#include <stdexcept>
#include <utility>

#include "cpu/registers/Register.hpp"

namespace cpu::registers
{
    enum RegisterId : std::uint8_t
    {
        R0, R1, R2, R3, R4, R5, R6, R7,
        R8, R9, R10, R11, R12, R13, R14, R15,

        RegisterCount
    };

    namespace detail
    {
        inline constexpr std::array<unsigned, RegisterCount> registerWidths =
        {
            32, 32, 32, 32, 32, 32, 32, 32,
            64, 64, 64, 64, 64, 64, 64, 64
        };

        constexpr qword makeMask(unsigned width)
        {
            if (width == 0 || width > 128)
                throw std::out_of_range("Invalid register width");

            return width == 128
                ? ~qword{0}
                : (qword{1} << width) - 1;
        }

        template<std::size_t... I>
        constexpr auto makeRegisters(std::index_sequence<I...>)
        {
            return std::array<Register, sizeof...(I)>
            {
                Register{makeMask(registerWidths[I])}...
            };
        }

        constexpr RegisterId parseRegisterId(std::string_view name)
        {
            if (name.size() < 2 || name.size() > 3 || name[0] != 'R')
                throw std::out_of_range("Invalid register name");

            if (name.size() == 3 && name[1] == '0')
                throw std::out_of_range("Invalid register name");

            unsigned index = 0;

            for (std::size_t i = 1; i < name.size(); ++i)
            {
                const char c = name[i];

                if (c < '0' || c > '9')
                    throw std::out_of_range("Invalid register name");

                index = index * 10 + (c - '0');
            }

            if (index >= RegisterCount)
                throw std::out_of_range("Register does not exist");

            return static_cast<RegisterId>(index);
        }
    }

    class RegisterFile
    {
    public:
        constexpr Register& operator[](RegisterId id)
        {
            return registers_[id];
        }

        constexpr const Register& operator[](RegisterId id) const
        {
            return registers_[id];
        }

        constexpr Register& operator[](std::string_view name)
        {
            return registers_[detail::parseRegisterId(name)];
        }

        constexpr const Register& operator[](std::string_view name) const
        {
            return registers_[detail::parseRegisterId(name)];
        }

        constexpr void reset()
        {
            for (auto& reg : registers_)
            {
                reg.reset();
            }
        }

    private:
        std::array<Register, RegisterCount> registers_ =
            detail::makeRegisters(std::make_index_sequence<RegisterCount>{});
    };
}
