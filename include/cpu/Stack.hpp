#pragma once

#include <array>
#include <cstdint>
#include "cpu/Register.hpp"
#include <expected>
namespace cpu
{

    enum class StackError
    {
        Overflow,
        Underflow,
        InvalidStackPointer
    };


    class Stack
    {
        private:
            
            static constexpr std::size_t capacity_ = 1 << 12; // 16KB of stack storage

            Register& stack_ptr_;

            std::array<std::uint32_t, capacity_> storage_;

            static constexpr unsigned required_width_ = std::bit_width(capacity_);

        public:

            explicit Stack(Register& stack_ptr)
                : stack_ptr_(stack_ptr)
            {
                if (stack_ptr_.width() < required_width_)
                {
                    throw std::runtime_error("Stack pointer width is insufficient for stack capacity");
                }
            }

            [[nodiscard]]
            std::expected<void, StackError> push(std::uint32_t value) noexcept
            {
                auto sp = stack_ptr_.read();
                
                if (sp >= capacity_)
                {
                    return std::unexpected(StackError::Overflow);
                }

                storage_[sp.convert_to<std::size_t>()] = value;
                
                stack_ptr_.write(sp + 1);
                
                return {};
            }


            [[nodiscard]]
            std::expected<std::uint32_t, StackError> pop() noexcept
            {
                auto sp = stack_ptr_.read();
                
                if (sp == 0)
                {
                    return std::unexpected(StackError::Underflow);
                }
                
                if (sp > capacity_)
                {
                    return std::unexpected(StackError::InvalidStackPointer);
                }

                sp--;

                const auto value = storage_[sp.convert_to<std::size_t>()];
                
                stack_ptr_.write(sp);
                
                return value;
            
            }
    };

}