/*
    Cpu\include\cpu\RegisterFile.hpp
    Defines the RegisterFile class for the CPU project.
*/

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
#include "cpu/Types.hpp"

namespace cpu
{
    namespace errors
    {
        enum class RegisterError
        {
            NAME_NOT_FOUND,
            ID_NOT_FOUND,
            NULL_REGISTER,
            FILE_FULL,
            DUPLICATE_NAME,
            ALREADY_INCLUDED,
            INVALID
        };
    }

    namespace storage
    {
        /// @brief null
        class RegisterFile
        {
            
            private:
                static constexpr std::size_t capacity_ = 128;
                
                ///
                struct Entry
                {
                    std::string name_;
                    std::unique_ptr<detail::Register> register_;
                };

                std::array<Entry, capacity_> entries_{};

                std::size_t firstEmptyEntry() const noexcept
                {
                    for (std::size_t i = 0; i < capacity_; ++i)
                    {
                        if (entries_[i].name_.empty())
                        {
                            return i;
                        }
                    }
            
                    return capacity_; // No empty slot found

                }
                
                std::size_t findEntryByName(std::string_view name) const noexcept
                {
                    for (std::size_t i = 0; i < capacity_; ++i)
                    {
                        if (entries_[i].name_ == name)
                            return i;
                    }

                    return capacity_; // Not found
                    
                }

            public:
                
                RegisterFile() = default;

                [[nodiscard]]
                std::expected<detail::Register*, errors::RegisterError>
                locate(std::string_view name) noexcept
                {
                    
                    auto entryIndex = findEntryByName(name);
                    
                    if (entryIndex == capacity_)
                    {
                        return std::unexpected(errors::RegisterError::NAME_NOT_FOUND);
                    }

                    return entries_[entryIndex].register_.get();

                }
                
                [[nodiscard]]
                std::expected<detail::Register*, errors::RegisterError>
                locate(types::register_id_t id) noexcept
                {
                    if (id >= capacity_)
                    {
                        return std::unexpected(errors::RegisterError::ID_NOT_FOUND);
                    }

                    if (!entries_[id].register_)
                    {
                        return std::unexpected(errors::RegisterError::NULL_REGISTER);
                    }

                    return entries_[id].register_.get();


                }

                [[nodiscard]]
                std::expected<void, errors::RegisterError>
                include(
                    std::string_view name,
                    std::unique_ptr<detail::Register>&& register_ptr)
                {
                    if (name.empty())
                    {
                        return std::unexpected(errors::RegisterError::INVALID);
                    }

                    if (!register_ptr)
                    {
                        return std::unexpected(errors::RegisterError::INVALID);
                    }

                    if (findEntryByName(name) != capacity_)
                    {
                        return std::unexpected(errors::RegisterError::DUPLICATE_NAME);
                    }

                    auto entry = firstEmptyEntry();

                    if (entry == capacity_)
                    {
                        return std::unexpected(errors::RegisterError::FILE_FULL);
                    }

                    std::string owned_name{name};

                    entries_[entry].name_     = std::move(owned_name);
                    entries_[entry].register_ = std::move(register_ptr);

                    return {}; // Success
                }

                [[nodiscard]]
                std::expected<void, errors::RegisterError>
                include(
                    std::string_view name,
                    types::byte_t width)
                {

                    auto reg = std::make_unique<detail::Register>(width);
                    return include(name, std::move(reg));
                }

                [[nodiscard]]
                std::expected<void, errors::RegisterError>
                remove(std::string_view name) noexcept
                {
                    if (name.empty())
                    {
                        return std::unexpected(errors::RegisterError::INVALID);
                    }

                    const auto entry = findEntryByName(name);
                    
                    if (entry == capacity_)
                    {
                        return std::unexpected(errors::RegisterError::NAME_NOT_FOUND);
                    }

                    entries_[entry].name_.clear();
                    entries_[entry].register_.reset();
                    
                    return {}; // Success
                }
                
                void clear() noexcept
                {
                    for (auto& entry : entries_)
                    {
                        entry.name_.clear();
                        entry.register_.reset();
                    }
                }

                [[nodiscard]]
                bool contains(std::string_view name) const noexcept
                {
                    return findEntryByName(name) != capacity_;
                }

                [[nodiscard]]
                std::expected<detail::Register*, errors::RegisterError>
                rename(std::string_view old_name, std::string_view new_name) noexcept
                {
                    if (old_name.empty() || new_name.empty())
                    {
                        return std::unexpected(errors::RegisterError::INVALID);
                    }

                    const auto entry = findEntryByName(old_name);

                    if (entry == capacity_)
                    {
                        return std::unexpected(errors::RegisterError::NAME_NOT_FOUND);
                    }

                    if (findEntryByName(new_name) != capacity_)
                    {
                        return std::unexpected(errors::RegisterError::DUPLICATE_NAME);
                    }

                    entries_[entry].name_ = std::string{new_name};

                    return {}; // Success
                }
        };
    }
    
    
}