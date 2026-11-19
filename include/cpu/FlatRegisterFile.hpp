
#pragma once

#include <cpu/Types.hpp>

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>

namespace cpu
{

// ============================================================
// Register IDs
// ============================================================

enum class FlatRegisterId : std::uint8_t
{
    R0, R1, R2, R3,
    R4, R5, R6, R7,
    R8, R9, R10, R11,
    R12, R13, R14, R15,

    Count
};

// ============================================================
// Compile-time register metadata
// ============================================================

namespace detail
{

inline constexpr std::size_t flat_register_count =
    static_cast<std::size_t>(FlatRegisterId::Count);

// ------------------------------------------------------------
// Logical register widths
// ------------------------------------------------------------

inline constexpr std::array<std::uint8_t, flat_register_count> flat_register_widths =
{
    128, 128, 128, 128,
    128, 128, 128, 128,
    128, 128, 128, 128,
    128, 128, 128, 128
};

[[nodiscard]]
consteval bool allRegistersFullWidth() noexcept
{
    for (const auto bits : flat_register_widths)
    {
        if (bits != 128)
            return false;
    }

    return true;
}

inline constexpr bool all_registers_full_width =
    allRegistersFullWidth();

// ------------------------------------------------------------
// Validate register widths
// ------------------------------------------------------------

[[nodiscard]]
consteval bool validFlatRegisterWidths() noexcept
{
    for (const auto bits : flat_register_widths)
    {
        if (bits == 0 || bits > 128)
            return false;
    }

    return true;
}



static_assert(
    validFlatRegisterWidths(),
    "Register widths must be between 1 and 128 bits"
);

// ------------------------------------------------------------
// Generate a mask for a register width
// ------------------------------------------------------------

[[nodiscard]]
constexpr qword makeRegisterMask(std::uint8_t bits) noexcept
{
    if (bits >= 128)
        return ~qword{0};

    if (bits == 0)
        return qword{0};

    return (qword{1} << bits) - 1;
}

// ------------------------------------------------------------
// Generate the complete mask table
// ------------------------------------------------------------

[[nodiscard]]
consteval auto makeRegisterMasks() noexcept
{
    std::array<qword, flat_register_count> result{};

    for (std::size_t i = 0; i < flat_register_count; ++i)
    {
        result[i] = makeRegisterMask(flat_register_widths[i]);
    }

    return result;
}

inline constexpr auto flat_register_masks = makeRegisterMasks();

}

// ============================================================
// Masked flat register file
// ============================================================

class MaskedFlatRegisterFile
{
public:
    using value_t = cpu::qword;
    using id_t = FlatRegisterId;

    static constexpr std::size_t count =
        detail::flat_register_count;

private:

    // --------------------------------------------------------
    // Register storage
    // --------------------------------------------------------

    std::array<value_t, count> values{};

public:

    // ========================================================
    // Runtime access
    // ========================================================

void set(id_t id, value_t value) noexcept
{
    const auto index = static_cast<std::size_t>(id);

    assert(index < count);

    if constexpr (detail::all_registers_full_width)
    {
        values[index] = value;
    }
    else
    {
        values[index] = value & detail::flat_register_masks[index];
    }
}
    
    [[nodiscard]]
    value_t get(id_t id) const noexcept
    {
        const auto index = static_cast<std::size_t>(id);

        assert(index < count);

        return values[index];
    }

    // ========================================================
    // Compile-time access
    // ========================================================

    template<id_t Id>
    void set(value_t value) noexcept
    {
        constexpr auto index = static_cast<std::size_t>(Id);

        static_assert(index < count);

        values[index] = value & detail::flat_register_masks[index];
    }

    template<id_t Id>
    [[nodiscard]]
    value_t get() const noexcept
    {
        constexpr auto index = static_cast<std::size_t>(Id);

        static_assert(index < count);

        return values[index];
    }

    // ========================================================
    // Metadata access
    // ========================================================

    [[nodiscard]]
    static constexpr std::uint8_t width(id_t id) noexcept
    {
        const auto index = static_cast<std::size_t>(id);

        assert(index < count);

        return detail::flat_register_widths[index];
    }

    [[nodiscard]]
    static constexpr value_t mask(id_t id) noexcept
    {
        const auto index = static_cast<std::size_t>(id);

        assert(index < count);

        return detail::flat_register_masks[index];
    }

    // ========================================================
    // Reset
    // ========================================================

    void reset() noexcept
    {
        values.fill(0);
    }
};

// ============================================================
// Register names
// ============================================================

inline constexpr std::array<std::string_view,
    MaskedFlatRegisterFile::count> RegisterNames =
{
    "R0", "R1", "R2", "R3",
    "R4", "R5", "R6", "R7",
    "R8", "R9", "R10", "R11",
    "R12", "R13", "R14", "R15"
};

// ============================================================
// Name resolution
// ============================================================

[[nodiscard]]
constexpr std::optional<FlatRegisterId>
resolveRegister(std::string_view name) noexcept
{
    if (name.size() == 2 && name[0] == 'R')
    {
        const char digit = name[1];

        if (digit >= '0' && digit <= '9')
        {
            return static_cast<FlatRegisterId>(digit - '0');
        }
    }

    if (name.size() == 3 &&
        name[0] == 'R' &&
        name[1] == '1')
    {
        const char digit = name[2];

        if (digit >= '0' && digit <= '5')
        {
            return static_cast<FlatRegisterId>(10 + digit - '0');
        }
    }

    return std::nullopt;
}

// Compile-time validation
static_assert(resolveRegister("R0").has_value());
static_assert(resolveRegister("R9").has_value());
static_assert(resolveRegister("R10").has_value());
static_assert(resolveRegister("R15").has_value());

static_assert(!resolveRegister("R16").has_value());
static_assert(!resolveRegister("R01").has_value());
static_assert(!resolveRegister("RX").has_value());
static_assert(!resolveRegister("").has_value());

} // namespace cpu