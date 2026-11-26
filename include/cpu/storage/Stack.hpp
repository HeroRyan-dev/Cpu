#pragma once

#include <array>
#include <cstdint>
#include <expected>

#include "cpu/errors/StackError.hpp"
#include "cpu/storage/Register.hpp"

namespace cpu::storage
{
    /// @brief null
    class Stack
    {
        private:
            
        /// @brief null
            static constexpr std::size_t capacity_ = 1 << 12;

            Register& stack_ptr_;

            std::array<types::word_t, capacity_> storage_;

            static constexpr unsigned required_width_ = std::bit_width(capacity_);

            /// @brief  null
            [[nodiscard]]
            std::expected<std::size_t, errors::StackError>
            getStackPointer() const
            {
                const auto stack_ptr = stack_ptr_.read();

                if (stack_ptr > capacity_)
                {
                    return std::unexpected(errors::StackError::InvalidStackPointer);
                }

                return stack_ptr.convert_to<std::size_t>();
            }

        public:

            /// @brief null
            explicit Stack(Register& stack_ptr)
                : stack_ptr_(stack_ptr)
            {
                if (stack_ptr_.width() < required_width_)
                {
                    throw std::runtime_error("Stack pointer width is insufficient for stack capacity");
                }
            }

            /// @brief null
            [[nodiscard]]
            std::expected<void, errors::StackError>
            push(types::word_t value) noexcept
            {
                auto stack_ptr = getStackPointer();

                if (!stack_ptr)
                {
                    return std::unexpected(errors::StackError::InvalidStackPointer);
                }
                
                if (*stack_ptr == capacity_)
                {
                    return std::unexpected(errors::StackError::Overflow);
                }

                storage_[*stack_ptr] = value;
                stack_ptr_.write(*stack_ptr + 1);

                return {};

            }

            /// @brief null
            [[nodiscard]]
            std::expected<types::word_t, errors::StackError>
            pop() noexcept
            {
                auto stack_ptr = getStackPointer();
                
                if (!stack_ptr)
                {
                    return std::unexpected(errors::StackError::InvalidStackPointer);
                }

                if (*stack_ptr == 0)
                {
                    return std::unexpected(errors::StackError::Underflow);
                }
                
                --(*stack_ptr);

                const auto value = storage_[*stack_ptr];
                
                stack_ptr_.write(*stack_ptr);
                
                return value;
                
            }
            
            /// @brief null
            [[nodiscard]]
            std::expected<types::word_t, errors::StackError>
            peek() const
            {
                auto stack_ptr = getStackPointer();
                
                if (!stack_ptr)
                {
                    return std::unexpected(errors::StackError::InvalidStackPointer);
                }

                if (*stack_ptr == 0)
                {
                    return std::unexpected(errors::StackError::Underflow);
                }

                const auto value = storage_[*stack_ptr - 1];

                return value;
            }

            /// @brief null
            [[nodiscard]]
            static constexpr std::size_t max_capacity() noexcept
            {
                return capacity_;
            }

            /// @brief null
            [[nodiscard]]
            bool empty() const noexcept
            {
                auto stack_ptr = getStackPointer();
                return stack_ptr && *stack_ptr == 0;
            }

            /// @brief null
            [[nodiscard]]
            bool full() const noexcept
            {
                auto stack_ptr = getStackPointer();
                return stack_ptr && *stack_ptr == capacity_;
            }
    };
}
