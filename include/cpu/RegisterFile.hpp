#pragma once

// Standard library includes
#include <memory>
#include <array>
#include <cstdint>
#include <string_view>
#include <stdexcept>
#include <utility>
#include <expected>
#include <iostream>

// Project includes
#include "cpu/Register.hpp"


namespace cpu
{
    
    enum class RegisterError
    {
        NotFound,
        NullRegister,
        FileFull,
        DuplicateName,
        AlreadyIncluded,
        InvalidName
    };

    class RegisterFile
    {
        
        private:
            // String to Register mapping for quick lookup by name
            static constexpr std::size_t capacity_ = 128;
            
            std::array<std::string, capacity_> names_{};
            std::array<std::unique_ptr<Register>, capacity_> registers_{};


            bool nameUsed(std::string_view name) const noexcept
            {
                for (const auto& n : names_)
                {
                    if (n == name)
                    {
                        return true;
                    }
                }
                return false;
            }

            bool registerUsed(const Register* register_ptr) const noexcept
            {
                for (const auto& r : registers_)
                {
                    if (r.get() == register_ptr)
                    {
                        return true;
                    }
                }
                return false;
            }

            int findEmptySlot() const noexcept
            {
                for (std::size_t i = 0; i < capacity_; ++i)
                {
                    if (names_[i].empty() && !registers_[i])
                    {
                        return static_cast<int>(i);
                    }
                }
                return -1; // No empty slot found
            }

            int findSlotByName(std::string_view name) const noexcept
            {
                for (std::size_t i = 0; i < capacity_; ++i)
                {
                    if (names_[i] == name)
                    {
                        return static_cast<int>(i);
                    }
                }
                
                return -1; // Not found
            }
            
        public:
            
            RegisterFile() = default;

            std::expected<Register*, RegisterError>
            locate(std::string_view name) noexcept
            {
                
                for (std::size_t i = 0; i < capacity_; ++i)
                {
                    
                    if (names_[i] == name) // Found the register by name
                    {
                        
                        if (!registers_[i]) // Register exists in name mapping but is null
                        {
                            return std::unexpected(RegisterError::NullRegister);
                        }
                        
                        return registers_[i].get();
                    
                    }

                }
                
                return std::unexpected(RegisterError::NotFound);
            }
            
            std::expected<void, RegisterError>
            include (std::string_view name, std::unique_ptr<Register> register_ptr) noexcept
            {
                if (name.empty())
                {
                    return std::unexpected(RegisterError::InvalidName); // Assuming InvalidName is not defined in RegisterError
                }

                if (!register_ptr)
                {
                    return std::unexpected(RegisterError::NullRegister);
                }

                if (nameUsed(name))
                {
                    return std::unexpected(RegisterError::DuplicateName);
                }

                if (registerUsed(register_ptr.get()))
                {
                    return std::unexpected(RegisterError::AlreadyIncluded);
                }

                int emptySlot = findEmptySlot();
                if (emptySlot == -1)
                {
                    return std::unexpected(RegisterError::FileFull);
                }

                names_[emptySlot] = name;
                registers_[emptySlot] = std::move(register_ptr);

                return {}; // Success
            }

            std::expected<void, RegisterError>
            remove (std::string_view name) noexcept
            {
                if (name.empty())
                {
                    return std::unexpected(RegisterError::InvalidName);
                }

                int slot = findSlotByName(name);
                if (slot != -1)
                {
                    names_[slot].clear();
                    registers_[slot].reset();
                    return {}; // Success
                }

                return std::unexpected(RegisterError::NotFound);
            }
            
            void clear() noexcept
            {
                for (int i = 0; i < capacity_; ++i)
                {
                    names_[i].clear();
                    registers_[i].reset();
                }

            }

    };

}