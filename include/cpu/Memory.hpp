/*
    Cpu\include\cpu\Memory.hpp
    Defines the Memory class for the CPU project.
*/

#pragma once

// Standard library includes
#include <array>
#include <cstdint>
#include <expected>

// Project includes
#include "cpu/Types.hpp"

namespace cpu::storage
{
    
    enum class MemoryError
    {
        AddressOutOfBounds
    };


    /// @brief null
    class Memory
    {
        private:
            
            constexpr static std::size_t capacity_ = 1 << 14;

            std::array<types::word_t, capacity_> storage_;
        
        public:
            
            /// @brief null
            Memory() : storage_{} {}

            [[nodiscard]]
            /// @brief null
            std::expected<types::word_t, MemoryError>
            read(types::address_t address) const noexcept
            {
                if (address >= capacity_)
                {
                    return std::unexpected(MemoryError::AddressOutOfBounds);
                }
                
                return storage_[address];

            }

            [[nodiscard]]
            /// @brief null
            std::expected<void, MemoryError>
            write(types::address_t address, types::word_t value) noexcept
            {
                if (address >= capacity_)
                {
                    return std::unexpected(MemoryError::AddressOutOfBounds);
                }

                storage_[address] = value;
                return {};
            }
    
    };

}