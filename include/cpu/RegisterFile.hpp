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
#include <ranges>
#include <algorithm>
#include <absl/container/flat_hash_map.h>

// Project includes
#include "cpu/Types.hpp"
#include "cpu/Register.hpp"

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

        struct StringHash
        {
            using is_transparent = void;

            std::size_t operator()(std::string_view value) const noexcept
            {
                return absl::Hash<std::string_view>{}(value);
            }
        };

        /// @brief null
        class RegisterFile
        {
            
            private:
                static constexpr std::size_t capacity_ = 128;
            

                std::array<std::unique_ptr<detail::Register>, capacity_> entries_;
                
                /// @brief null
                absl::flat_hash_map<std::string, std::size_t, StringHash, std::equal_to<>> map_;

                [[nodiscard]]
                detail::Register* giveEntryByName(std::string_view name) noexcept
                {
                    auto it = map_.find(name);

                    if (it == map_.end())
                        return nullptr;

                    return entries_[it->second].get();
                }

                [[nodiscard]]
                std::size_t findEntryIndex(std::string_view name) const noexcept
                {
                    auto it = map_.find(name);

                    if (it == map_.end())
                        return capacity_;

                    return it->second;
                }

                [[nodiscard]]
                std::size_t firstEmptyEntry() const noexcept
                {
                    for (std::size_t i = 0; i < capacity_; ++i)
                    {
                        if (!entries_[i])
                            return i;
                    }
                    
                    return capacity_;
                }

                [[nodiscard]]
                bool isValidName(std::string_view name) const noexcept
                {
                    return name.size() >= 2 && !std::isdigit(static_cast<unsigned char>(name.front()));
                }


            public:
                
                RegisterFile() = default;

                [[nodiscard]]
                std::expected<detail::Register*, errors::RegisterError>
                locate(std::string_view name) noexcept
                {
                    
                    auto entry = giveEntryByName(name);
                    
                    if (!entry)
                    {
                        return std::unexpected(errors::RegisterError::NAME_NOT_FOUND);
                    }

                    return entry;

                }

                [[nodiscard]]
                std::expected<void, errors::RegisterError>
                include(
                    std::string_view name,
                    std::unique_ptr<detail::Register>&& register_ptr)
                {
                    if (!isValidName(name))
                    {
                        return std::unexpected(errors::RegisterError::INVALID);
                    }

                    if (!register_ptr)
                    {
                        return std::unexpected(errors::RegisterError::INVALID);
                    }

                    auto entry = firstEmptyEntry();

                    if (entry == capacity_)
                    {
                        return std::unexpected(errors::RegisterError::FILE_FULL);
                    }

                    auto [it, inserted] = map_.emplace(std::string{name}, entry);

                    if (!inserted)
                    {
                        return std::unexpected(errors::RegisterError::DUPLICATE_NAME);
                    }

                    entries_[entry] = std::move(register_ptr);

                    return {}; // Success
                }

                [[nodiscard]]
                std::expected<void, errors::RegisterError>
                include(
                    std::string_view name,
                    types::byte_t width)
                {
                    if (!isValidName(name))
                    {
                        return std::unexpected(errors::RegisterError::INVALID);
                    }

                    auto entry = firstEmptyEntry();

                    if (entry == capacity_)
                    {
                        return std::unexpected(errors::RegisterError::FILE_FULL);
                    }
                    
                    auto reg = std::make_unique<detail::Register>(width);

                    auto [it, inserted] = map_.emplace(std::string{name}, entry);

                    if (!inserted)
                    {
                        return std::unexpected(errors::RegisterError::DUPLICATE_NAME);
                    }

                    entries_[entry] = std::move(reg);

                    return {}; // Success
                }

                [[nodiscard]]
                std::expected<void, errors::RegisterError>
                remove(std::string_view name)
                {
                    if (!isValidName(name))
                        return std::unexpected(errors::RegisterError::INVALID);

                    auto it = map_.find(name);

                    if (it == map_.end())
                        return std::unexpected(errors::RegisterError::NAME_NOT_FOUND);

                    entries_[it->second].reset();
                    map_.erase(it);

                    return {};
                }

                void clear() noexcept
                {
                    map_.clear();
                    
                    for (auto& entry : entries_)
                    {
                        entry.reset();
                    }

                }

                [[nodiscard]]
                bool contains(std::string_view name) const noexcept
                {
                    return findEntryIndex(name) != capacity_;
                }

                [[nodiscard]]
                std::expected<detail::Register *, errors::RegisterError>
                rename(std::string_view old_name, std::string_view new_name) noexcept
                {
                    if (!isValidName(old_name) || !isValidName(new_name))
                        return std::unexpected(errors::RegisterError::INVALID);

                    auto it = map_.find(old_name);

                    if (it == map_.end())
                        return std::unexpected(errors::RegisterError::NAME_NOT_FOUND);

                    if (map_.contains(new_name))
                        return std::unexpected(errors::RegisterError::DUPLICATE_NAME);

                    const auto entry = it->second;

                    auto [new_it, inserted] = map_.emplace(std::string{new_name}, entry);

                    if (!inserted)
                        return std::unexpected(errors::RegisterError::DUPLICATE_NAME);

                    map_.erase(it);

                    return entries_[entry].get();
                }
        };
    }
}