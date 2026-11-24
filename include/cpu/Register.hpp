#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include "Types.hpp"
#include <cstdint>

namespace cpu
{

    

    /// @brief A class representing a single CPU register.
    /// @details This class provides methods for reading and writing the register value, as well as managing the register width.
    class Register
    {
        
        private:

            using value_t = boost::multiprecision::uint128_t;

            value_t data_{};
            uint8_t width_{0};


            value_t generateMask(unsigned width)
            {
                return (width >= 128) ? static_cast<value_t>(-1) : ((static_cast<value_t>(1) << width) - 1);
            }
            

        public:
            
            // Should only be used when initializing a register without a specific width
            Register() = default;

            explicit Register(uint8_t width)
                : data_{0}, width_{width}
            {
                // N\A
            }


            explicit Register(value_t value, uint8_t width)
                : data_{value & generateMask(width)}, width_{width}
            {
                // N\A
            }

            // Infallible read of the register value
            value_t read() const
            {
                return data_;
            }

            // Infallible write to the register value
            void write(value_t value)
            {
                data_ = value & generateMask(width_);
            }

            /// @brief Returns the width of the register in bits.
            uint8_t width() const
            {
                return width_;
            }


    };
}