// Types.hpp
// Defines fundamental types for the CPU project

#pragma once

#include <cstdint>
#include <cstddef>
#include <boost/multiprecision/cpp_int.hpp>

namespace cpu
{
    
    using byte_t   = std::uint8_t;
    using word_t   = std::uint32_t; // Natural data width
    using dword_t  = std::uint64_t;
    using qword_t  = boost::multiprecision::uint128_t;
    using word256_t = boost::multiprecision::uint256_t;
    

    using opcode_t       = std::uint8_t;
    using address_t      = std::uint16_t;
    using register_id_t  = std::uint8_t;
    
    using instruction_t  = std::uint32_t;


    using usize_t        = std::size_t;

}