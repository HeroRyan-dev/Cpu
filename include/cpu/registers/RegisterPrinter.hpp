#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <ostream>
#include <string>
#include <string_view>

#include "cpu/registers/RegisterFile.hpp"

namespace cpu::registers
{
    class RegisterPrinter
    {
    public:
        inline static constexpr std::array<std::string_view, RegisterCount> Names =
        {
            "R0", "R1", "R2", "R3",
            "R4", "R5", "R6", "R7",
            "R8", "R9", "R10", "R11",
            "R12", "R13", "R14", "R15"
        };

        // Print all registers in decimal.
        static void print(const RegisterFile& registers, std::ostream& output)
        {
            for (std::size_t i = 0; i < RegisterCount; ++i)
            {
                const qword value = registers[static_cast<RegisterId>(i)];

                output << Names[i] << ": " << toDecimal(value) << '\n';
            }
        }

        // Print all registers in hexadecimal.
        static void printHex(const RegisterFile& registers, std::ostream& output)
        {
            for (std::size_t i = 0; i < RegisterCount; ++i)
            {
                const qword value = registers[static_cast<RegisterId>(i)];

                output << Names[i] << ": 0x"
                       << std::hex << std::setfill('0')
                       << std::setw(16) << static_cast<std::uint64_t>(value >> 64)
                       << std::setw(16) << static_cast<std::uint64_t>(value)
                       << std::dec << '\n';
            }
        }

        // Print all registers in binary.
        static void printBinary(const RegisterFile& registers, std::ostream& output)
        {
            for (std::size_t i = 0; i < RegisterCount; ++i)
            {
                const qword value = registers[static_cast<RegisterId>(i)];

                output << Names[i] << ": " << toBinary(value) << '\n';
            }
        }

        // Convert a 128-bit value to decimal.
        [[nodiscard]]
        static std::string toDecimal(qword value)
        {
            if (value == 0)
                return "0";

            std::string result;

            while (value != 0)
            {
                result += static_cast<char>('0' + value % 10);
                value /= 10;
            }

            std::reverse(result.begin(), result.end());

            return result;
        }

        // Convert a 128-bit value to hexadecimal.
        [[nodiscard]]
        static std::string toHex(qword value)
        {
            constexpr char digits[] = "0123456789ABCDEF";

            if (value == 0)
                return "0";

            std::string result;

            while (value != 0)
            {
                result += digits[value & 0xF];
                value >>= 4;
            }

            std::reverse(result.begin(), result.end());

            return result;
        }

        // Convert a 128-bit value to binary.
        [[nodiscard]]
        static std::string toBinary(qword value)
        {
            if (value == 0)
                return "0";

            std::string result;

            while (value != 0)
            {
                result += (value & 1) ? '1' : '0';
                value >>= 1;
            }

            std::reverse(result.begin(), result.end());

            return result;
        }
    };
}
