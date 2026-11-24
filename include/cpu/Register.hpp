/*
    Cpu\include\cpu\Register.hpp
    Defines the Register class for the CPU project.
*/

#pragma once

// Standard library includes
#include <stdexcept>

// Project includes
#include "Types.hpp"

namespace cpu::detail
{
        /// @brief A class representing a single CPU register.
        /// @details This class provides methods for reading and writing the register value, as well as managing the register width.
        class Register
        {
            
            private:

                types::qword_t data_{};
                types::byte_t width_{};
                types::qword_t mask_{};

                /// @brief Generates a bitmask for the given width.
                static types::qword_t generateMask(types::byte_t width)
                {
                    if (width == 0 || width > 128)
                        throw std::invalid_argument("Invalid register width");

                    return (width == 128)
                        ? ~types::qword_t(0)
                        : (types::qword_t(1) << width) - 1;
                }
                
            public:
                
                /// @brief null
                Register() = default;

                /// @brief null
                explicit Register(types::byte_t width)
                    : width_{width}, mask_{generateMask(width)}
                {
                }

                /// @brief null
                explicit Register(types::qword_t value, types::byte_t width)
                    : data_{value}, width_{width}, mask_{generateMask(width)}
                {
                    data_ &= mask_;
                }

                /// @brief null
                types::qword_t read() const
                {
                    return data_;
                }

                /// @brief null
                void write(types::qword_t value)
                {
                    data_ = value & mask_;
                }

                /// @brief null
                types::byte_t width() const
                {
                    return width_;
                }
    
        };
}