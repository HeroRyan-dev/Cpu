#include <meta>
#include <iostream>
#include <string_view>
#include <functional>
#include <optional>
#include <cstdint>
#include <cstddef>
#include <limits>

#include "Types.hpp"
namespace cpu {

    template<cpu::usize_t Bits>
    struct Register
    {

        static_assert(Bits > 0 && Bits <= 128, "Register size must be between 1 and 128 bits");

        using storage_t = cpu::qword;

        static constexpr cpu::usize_t width = Bits;

        static constexpr storage_t mask = ~storage_t{0} >> (128 - Bits);

        storage_t value{};

        constexpr void set(storage_t v) noexcept
        {
            value = v & mask;
        }
        
        [[nodiscard]]
        constexpr storage_t get() const noexcept
        {
            return value;
        }
    };

    struct RegisterFile
    {
        Register<32> R0, R1, R2, R3, R4, R5, R6, R7;
        Register<16> R8, R9, R10, R11, R12, R13, R14, R15;
    };

    template<typename T>
    inline constexpr auto members_v = std::define_static_array(
        std::meta::nonstatic_data_members_of(
            ^^T,
            std::meta::access_context::current()
        )
    );

    template<typename T>
    constexpr std::optional<cpu::register_id_t> resolveRegister(std::string_view name)
    {
        cpu::register_id_t index = 0;
        template for (constexpr auto member : members_v<T>) {
            if (name == std::meta::identifier_of(member)) {
                return index;
            }
            ++index;
        }
        return std::nullopt;
    }

    template<typename T, typename F>
    constexpr bool visitRegister(T& registers, cpu::register_id_t id, F&& callback)
    {
        cpu::register_id_t index = 0;
        template for (constexpr auto member : members_v<T>) {
            if (id == index) {
                std::invoke(std::forward<F>(callback), registers.[:member:]);
                return true;
            }
            ++index;
        }
        return false;
    }

    template<typename T, typename F>
    constexpr bool visitRegister(T& registers, std::string_view name, F&& callback)
    {
        if (auto id = resolveRegister<T>(name)) {
            return visitRegister(registers, *id, std::forward<F>(callback));
        }
        return false;
    }
} // namespace cpu