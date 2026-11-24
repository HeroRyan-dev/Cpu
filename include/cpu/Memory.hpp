#pragma once

#include <array>
#include <cstdint>

namespace cpu
{
    
    /// @brief Represents the memory of the CPU.
    /// @details This class can be extended to include methods for reading and writing memory, as well as managing memory size and layout.
    class Memory
    {
        private:
            
            std::array<std::uint32_t, 1 << 16> storage_;
        
        public:
            
            /// @brief Constructs a new Memory object with all storage initialized to zero.
            Memory() : storage_{0} {}

            /// @brief Reads a 32-bit value from the specified memory address.
            std::uint32_t read(std::uint16_t address) const
            {
                return storage_[address];
            }

            /// @brief Writes a 32-bit value to the specified memory address.
            void write(std::uint16_t address, std::uint32_t value)
            {
                storage_[address] = value;
            }
    
    };

}