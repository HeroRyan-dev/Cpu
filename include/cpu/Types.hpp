// Types.hpp
// Defines fundamental types for the CPU project

#pragma once

#include <cstdint>
#include <cstddef>

namespace cpu
{
    __extension__ using qword = unsigned __int128;

    using byte_t = std::uint8_t;
    using word = std::uint32_t; // Natural data width
    using dword = std::uint64_t;

    using address_t = std::uint32_t;
    using instruction_t = std::uint32_t;
    using opcode_t = std::uint8_t;
    using register_id_t = std::uint8_t;
    using usize_t = std::size_t;

}